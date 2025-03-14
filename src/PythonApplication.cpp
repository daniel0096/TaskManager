#include "PythonApplication.h"
#include "PythonApplicationModule.h"
#include "FileSystem.h"
#include "Log.h"

#include <cstdlib>

PythonApplication::PythonApplication()
{
	std::string pythonHome, pythonExecutable, pythonLib, pythonDLLs, pythonSitePackages;

	FILE* pipe = _popen("python -c \"import sys; print(sys.executable)\"", "r");
	if (pipe)
	{
		char buffer[256];
		if (fgets(buffer, sizeof(buffer), pipe) != nullptr)
		{
			pythonExecutable = std::string(buffer);
			pythonExecutable.erase(std::remove(pythonExecutable.begin(), pythonExecutable.end(), '\n'), pythonExecutable.end());
		}
		_pclose(pipe);
	}

	if (pythonExecutable.empty())
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Error: Could not detect Python installation.");
		exit(1);
	}

	pythonHome = std::filesystem::path(pythonExecutable).parent_path().string();
	pythonLib = pythonHome + "\\Lib";
	pythonDLLs = pythonHome + "\\DLLs";
	pythonSitePackages = pythonLib + "\\site-packages";

	TRACE_LOG(LOG_LEVEL_LOG, "Detected Python 3.13 installation: %s", pythonExecutable.c_str());

	PyStatus status;
	PyConfig config;
	PyConfig_InitPythonConfig(&config);

	TRACE_LOG(LOG_LEVEL_LOG, "Setting PYTHONHOME to: %s", pythonHome.c_str());
	status = PyConfig_SetString(&config, &config.home, Py_DecodeLocale(pythonHome.c_str(), nullptr));
	if (PyStatus_Exception(status))
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Error setting PYTHONHOME: %s", status.err_msg);
		PyConfig_Clear(&config);
		exit(1);
	}

	TRACE_LOG(LOG_LEVEL_LOG, "Setting Python program name to: %s", pythonExecutable.c_str());
	status = PyConfig_SetString(&config, &config.program_name, Py_DecodeLocale(pythonExecutable.c_str(), nullptr));
	if (PyStatus_Exception(status))
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Error setting program name: %s", status.err_msg);
		PyConfig_Clear(&config);
		exit(1);
	}

	TRACE_LOG(LOG_LEVEL_LOG, "Appending module search paths...");
	status = PyWideStringList_Append(&config.module_search_paths, Py_DecodeLocale(pythonLib.c_str(), nullptr));
	if (PyStatus_Exception(status))
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Error setting Python Lib path: %s", status.err_msg);
		PyConfig_Clear(&config);
		exit(1);
	}

	TRACE_LOG(LOG_LEVEL_LOG, "Registering C++ module 'app'");
	PyImport_AppendInittab("app", &PyInit_app);

	TRACE_LOG(LOG_LEVEL_LOG, "Initializing Python 3.13...");
	status = Py_InitializeFromConfig(&config);
	if (PyStatus_Exception(status))
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Error initializing Python 3.13: %s", status.err_msg);
		PyConfig_Clear(&config);
		exit(1);
	}

	PyConfig_Clear(&config);
}

PythonApplication::~PythonApplication()
{
	TRACE_LOG(LOG_LEVEL_LOG, "Finalizing Python interpreter.");
	Py_Finalize();
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
	PyRun_SimpleFile(fp, script.c_str());
	fclose(fp);
}