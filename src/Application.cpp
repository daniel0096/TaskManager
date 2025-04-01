#include "Application.h"
#include "Log.h"
#include "FileSystem.h"
#include "PythonApplication.h"
#include "Config.h"

#include "Python.h"

void CApplication::Run()
{
#if !defined(_DEBUG) && defined(_WIN32)
	HideDebugBuildConsole();
#endif

    PythonApplication pyApp;

    bool initSuccess = OnInit();

    if (!initSuccess)
    {
        TRACE_LOG(LOG_LEVEL_ERR, "Initial check failed, please close the application.");
        return;
    }
#ifdef _DEBUG
    TRACE_LOG(LOG_LEVEL_LOG, "Initial check succeeded.");
#endif
    pyApp.RunPythonScript("main.py");

    while (m_isRunning)
    {
        OnUpdate();
    }
}

#if !defined(_DEBUG) && defined(_WIN32)
void CApplication::HideDebugBuildConsole()
{
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);
}
#endif

void CApplication::OnUpdate()
{
	/*todo*/
}

bool CApplication::OnInit()
{
	CConfig config;

	bool success = config.SetupConfigFile("settings.cfg");
	if (!success)
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Failed to setup config file!");
		return false;
	}

	bool allFilesWerePresent = true;

	if (!CFileSystem::Instance().FileExists("sysser.txt"))
	{
#ifdef _DEBUG
		TRACE_LOG(LOG_LEVEL_LOG, "File %s does not exist, creating...", "sysser.txt");
#endif
		CFileSystem::Instance().CreateFiles("sysser.txt");
		TRACE_LOG(LOG_LEVEL_LOG, "File %s created.", "sysser.txt");
		allFilesWerePresent = false;
	}

	if (!CFileSystem::Instance().FileExists("syslog.txt"))
	{
#ifdef _DEBUG
		TRACE_LOG(LOG_LEVEL_LOG, "File %s does not exist, creating...", "syslog.txt");
#endif
		CFileSystem::Instance().CreateFiles("syslog.txt");
		TRACE_LOG(LOG_LEVEL_LOG, "File %s created.", "syslog.txt");
		allFilesWerePresent = false;
	}

	bool allFilesNowExist = true;

	for (int iter = 0; iter < FILE_MAX_LEN; ++iter)
	{
		if (!CFileSystem::Instance().FileExists(FILES[iter]))
		{
#ifdef _DEBUG
			TRACE_LOG(LOG_LEVEL_LOG, "File %s does not exist, creating...", FILES[iter]);
#endif
			CFileSystem::Instance().CreateFiles(FILES[iter]);
			TRACE_LOG(LOG_LEVEL_LOG, "File %s created.", FILES[iter]);
			allFilesNowExist = false;
		}
	}

	return allFilesNowExist;
}
