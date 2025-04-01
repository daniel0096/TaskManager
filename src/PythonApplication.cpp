#include "PythonApplication.h"
#include "PythonApplicationModule.h"
#include "FileSystem.h"
#include "Log.h"

#ifdef __linux__
#	include <algorithm>
#	include <cstdio>
#	include <cstdlib>
#endif

PythonApplication::PythonApplication()
{
	std::string pythonHome, pythonExecutable, pythonLib, pythonSitePackages;

#ifdef __linux__
	const char* pythonCmd = std::getenv("PYTHON_EXECUTABLE");
	std::string pythonDetectCmd;

	if (pythonCmd)
		pythonDetectCmd = std::string(pythonCmd) + " -c \"import sys; print(sys.executable)\"";
	else
		pythonDetectCmd = "python3 -c \"import sys; print(sys.executable)\"";

	FILE* pipe = popen(pythonDetectCmd.c_str(), "r");
#elif _WIN32
	FILE* pipe = _popen("python -c \"import sys; print(sys.executable)\"", "r");
#endif

	if (pipe)
	{
		char buffer[256];
		if (fgets(buffer, sizeof(buffer), pipe) != nullptr)
		{
			pythonExecutable = std::string(buffer);
			pythonExecutable.erase(std::remove(pythonExecutable.begin(), pythonExecutable.end(), '\n'), pythonExecutable.end());
		}
#ifdef __linux__
		pclose(pipe);
#elif _WIN32
		_pclose(pipe);
#endif
	}

	if (pythonExecutable.empty())
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Error: Could not detect Python installation.");
		exit(1);
	}

#ifdef __linux__
	std::string getHomeCmd = pythonExecutable + " -c \"import sys; print(sys.prefix)\"";
	FILE* pipeHome = popen(getHomeCmd.c_str(), "r");
	if (pipeHome)
	{
		char buffer[256];
		if (fgets(buffer, sizeof(buffer), pipeHome) != nullptr)
		{
			pythonHome = std::string(buffer);
			pythonHome.erase(std::remove(pythonHome.begin(), pythonHome.end(), '\n'), pythonHome.end());
		}
		pclose(pipeHome);
	}

	std::string getLibCmd = pythonExecutable + " -c \"import sysconfig; print(sysconfig.get_path('stdlib'))\"";
	FILE* pipeLib = popen(getLibCmd.c_str(), "r");
	if (pipeLib)
	{
		char buffer[256];
		if (fgets(buffer, sizeof(buffer), pipeLib) != nullptr)
		{
			pythonLib = std::string(buffer);
			pythonLib.erase(std::remove(pythonLib.begin(), pythonLib.end(), '\n'), pythonLib.end());
		}
		pclose(pipeLib);
	}
#elif _WIN32
	pythonHome = std::filesystem::path(pythonExecutable).parent_path().string();
	pythonLib = pythonHome + "\\Lib";
	pythonSitePackages = pythonLib + "\\site-packages";
#endif

	if (pythonHome.empty() || pythonLib.empty())
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Error: Could not detect Python paths.");
		exit(1);
	}

#ifdef _DEBUG
	TRACE_LOG(LOG_LEVEL_LOG, "Detected Python executable: %s", pythonExecutable.c_str());
	TRACE_LOG(LOG_LEVEL_LOG, "Setting PYTHONHOME to: %s", pythonHome.c_str());
	TRACE_LOG(LOG_LEVEL_LOG, "Setting Python Lib path to: %s", pythonLib.c_str());
#endif

	PyStatus status;
	PyConfig config;
	PyConfig_InitPythonConfig(&config);

	status = PyConfig_SetString(&config, &config.home, Py_DecodeLocale(pythonHome.c_str(), nullptr));
	if (PyStatus_Exception(status))
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Error setting PYTHONHOME: %s", status.err_msg);
		PyConfig_Clear(&config);
		exit(1);
	}

	status = PyConfig_SetString(&config, &config.program_name, Py_DecodeLocale(pythonExecutable.c_str(), nullptr));
	if (PyStatus_Exception(status))
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Error setting program name: %s", status.err_msg);
		PyConfig_Clear(&config);
		exit(1);
	}

	status = PyWideStringList_Append(&config.module_search_paths, Py_DecodeLocale(pythonLib.c_str(), nullptr));
	if (PyStatus_Exception(status))
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Error appending Python Lib path: %s", status.err_msg);
		PyConfig_Clear(&config);
		exit(1);
	}

	std::string pythonScriptsPath = (std::filesystem::current_path() / "Release" / "root").string();
	std::wstring wideScriptsPath = std::filesystem::path(pythonScriptsPath).wstring();
	status = PyWideStringList_Append(&config.module_search_paths, wideScriptsPath.c_str());
	if (PyStatus_Exception(status))
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Error appending script path: %s", pythonScriptsPath.c_str());
		PyConfig_Clear(&config);
		exit(1);
	}
#ifdef _DEBUG
	TRACE_LOG(LOG_LEVEL_LOG, "Registering C++ module 'app'");
#endif
	PyImport_AppendInittab("app", &PyInit_app);
#ifdef _DEBUG
	TRACE_LOG(LOG_LEVEL_LOG, "Initializing Python...");
#endif
	status = Py_InitializeFromConfig(&config);

	if (PyStatus_Exception(status))
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Error initializing Python: %s", status.err_msg);
		PyConfig_Clear(&config);
		exit(1);
	}

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
#ifdef _DEBUG
	TRACE_LOG(LOG_LEVEL_LOG, "Running Python script: %s", script.c_str());
#endif
	PyRun_SimpleFile(fp, runScript.string().c_str());

	if (PyErr_Occurred())
		PyErr_Print();

	fclose(fp);
}
