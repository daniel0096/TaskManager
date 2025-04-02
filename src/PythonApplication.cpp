#include "PythonApplication.h"
#include "PythonApplicationModule.h"
#include "FileSystem.h"
#include "Log.h"

#ifdef __linux__
    #include <algorithm>
    #include <cstdio>
    #include <cstdlib>
    #include <cstring>
#endif

PythonApplication::PythonApplication()
{
    std::string pythonHome, pythonExecutable, pythonLib, pythonSitePackages;
    std::string pythonExecutablePath;

#ifdef __linux__
    const char* virtualEnv = std::getenv("VIRTUAL_ENV");
    if (virtualEnv)
    {
        pythonExecutablePath = std::string(virtualEnv) + "/bin/python";
    }
    else
    {
        FILE* whichPipe = popen("which python3", "r");
        if (whichPipe)
        {
            char buffer[256] = {};
            if (fgets(buffer, sizeof(buffer), whichPipe))
            {
                pythonExecutablePath = std::string(buffer);
                pythonExecutablePath.erase(std::remove(pythonExecutablePath.begin(), pythonExecutablePath.end(), '\n'),
                                             pythonExecutablePath.end());
            }
            pclose(whichPipe);
        }
        if (pythonExecutablePath.empty())
        {
            TRACE_LOG(LOG_LEVEL_ERR, "Error: Could not detect Python executable using 'which python3'.");
            exit(1);
        }
    }

    {
        std::string detectCmd = pythonExecutablePath + " -c \"import sys; print(sys.executable)\"";
        FILE* pipe = popen(detectCmd.c_str(), "r");
        if (pipe)
        {
            char buffer[256] = {};
            if (fgets(buffer, sizeof(buffer), pipe))
            {
                pythonExecutable = std::string(buffer);
                pythonExecutable.erase(std::remove(pythonExecutable.begin(), pythonExecutable.end(), '\n'),
                                         pythonExecutable.end());
            }
            pclose(pipe);
        }
    }
#elif _WIN32
    {
        FILE* pipe = _popen("python -c \"import sys; print(sys.executable)\"", "r");
        if (pipe)
        {
            char buffer[256] = {};
            if (fgets(buffer, sizeof(buffer), pipe))
            {
                pythonExecutable = std::string(buffer);
                pythonExecutable.erase(std::remove(pythonExecutable.begin(), pythonExecutable.end(), '\n'),
                                         pythonExecutable.end());
            }
            _pclose(pipe);
        }
    }
#endif

    if (pythonExecutable.empty())
    {
        TRACE_LOG(LOG_LEVEL_ERR, "Error: Could not detect Python executable.");
        exit(1);
    }

#ifdef __linux__
    {
        std::string getBasePrefixCmd = pythonExecutable + " -c \"import sys; print(sys.base_prefix)\"";
        FILE* pipeBase = popen(getBasePrefixCmd.c_str(), "r");
        if (pipeBase)
        {
            char buffer[256] = {};
            if (fgets(buffer, sizeof(buffer), pipeBase))
            {
                pythonHome = std::string(buffer);
                pythonHome.erase(std::remove(pythonHome.begin(), pythonHome.end(), '\n'),
                                 pythonHome.end());
            }
            pclose(pipeBase);
        }
    }

    {
        std::string getLibCmd = pythonExecutable + " -c \"import sysconfig; print(sysconfig.get_path('stdlib'))\"";
        FILE* pipeLib = popen(getLibCmd.c_str(), "r");
        if (pipeLib)
        {
            char buffer[256] = {};
            if (fgets(buffer, sizeof(buffer), pipeLib))
            {
                pythonLib = std::string(buffer);
                pythonLib.erase(std::remove(pythonLib.begin(), pythonLib.end(), '\n'),
                                pythonLib.end());
            }
            pclose(pipeLib);
        }
    }
#elif _WIN32
    {
        pythonHome = std::filesystem::path(pythonExecutable).parent_path().string();
        pythonLib = pythonHome + "\\Lib";
        pythonSitePackages = pythonLib + "\\site-packages";
    }
#endif

    if (pythonHome.empty() || pythonLib.empty())
    {
        TRACE_LOG(LOG_LEVEL_ERR, "Error: Could not detect Python paths.");
        exit(1);
    }

    TRACE_LOG(LOG_LEVEL_LOG, "Detected Python executable: %s", pythonExecutable.c_str());
    TRACE_LOG(LOG_LEVEL_LOG, "Setting PYTHONHOME to: %s", pythonHome.c_str());
    TRACE_LOG(LOG_LEVEL_LOG, "Setting Python Lib path to: %s", pythonLib.c_str());

    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    // Nastavení PYTHONHOME a program_name pomocí převedených řetězců
    wchar_t* w_home = Py_DecodeLocale(pythonHome.c_str(), nullptr);
    wchar_t* w_prog = Py_DecodeLocale(pythonExecutable.c_str(), nullptr);
    status = PyConfig_SetString(&config, &config.home, w_home);
    if (PyStatus_Exception(status))
    {
        TRACE_LOG(LOG_LEVEL_ERR, "Error setting PYTHONHOME: %s", status.err_msg);
        PyConfig_Clear(&config);
        PyMem_RawFree(w_home);
        PyMem_RawFree(w_prog);
        exit(1);
    }
    status = PyConfig_SetString(&config, &config.program_name, w_prog);
    if (PyStatus_Exception(status))
    {
        TRACE_LOG(LOG_LEVEL_ERR, "Error setting program name: %s", status.err_msg);
        PyConfig_Clear(&config);
        PyMem_RawFree(w_home);
        PyMem_RawFree(w_prog);
        exit(1);
    }
    PyMem_RawFree(w_home);
    PyMem_RawFree(w_prog);

    // Přidání cesty ke standardní knihovně
    status = PyWideStringList_Append(&config.module_search_paths, Py_DecodeLocale(pythonLib.c_str(), nullptr));
    if (PyStatus_Exception(status))
    {
        TRACE_LOG(LOG_LEVEL_ERR, "Error appending stdlib path: %s", status.err_msg);
        PyConfig_Clear(&config);
        exit(1);
    }
     status = PyWideStringList_Append(&config.module_search_paths, Py_DecodeLocale(sitePackagesPath.c_str(), nullptr));
    if (PyStatus_Exception(status))
    {
        TRACE_LOG(LOG_LEVEL_ERR, "Error appending site-packages path: %s", sitePackagesPath.c_str());
        PyConfig_Clear(&config);
        exit(1);
    }

    std::string scriptPath = (std::filesystem::current_path() / "Release" / "root").string();
    status = PyWideStringList_Append(&config.module_search_paths, Py_DecodeLocale(scriptPath.c_str(), nullptr));
    if (PyStatus_Exception(status))
    {
        TRACE_LOG(LOG_LEVEL_ERR, "Error appending script path: %s", scriptPath.c_str());
        PyConfig_Clear(&config);
        exit(1);
    }

    TRACE_LOG(LOG_LEVEL_LOG, "Registering C++ module 'app'");
    PyImport_AppendInittab("app", &PyInit_app);

    TRACE_LOG(LOG_LEVEL_LOG, "Initializing Python...");
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status))
    {
        TRACE_LOG(LOG_LEVEL_ERR, "Error initializing Python: %s", status.err_msg);
        PyConfig_Clear(&config);
        exit(1);
    }

    PyRun_SimpleString("import sys; print('[C++] Python version:', sys.version)");
    PyRun_SimpleString("import sys; print('[C++] sys.path:', sys.path)");

    PyConfig_Clear(&config);
}

PythonApplication::~PythonApplication()
{
    if (Py_IsInitialized())
    {
        TRACE_LOG(LOG_LEVEL_LOG, "Finalizing Python interpreter.");
        Py_Finalize();
    }
}

void PythonApplication::RunPythonScript(const std::string& script)
{
    std::filesystem::path runScript = CFileSystem::Instance().GetRootDirectory() / script;
    FILE* fp = fopen(runScript.string().c_str(), "r");
    if (!fp)
    {
        TRACE_LOG(LOG_LEVEL_ERR, "Failed to open script: %s", script.c_str());
        return;
    }
    TRACE_LOG(LOG_LEVEL_LOG, "Running Python script: %s", script.c_str());
    PyRun_SimpleFile(fp, runScript.string().c_str());
    if (PyErr_Occurred())
        PyErr_Print();
    fclose(fp);
}
