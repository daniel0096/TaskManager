#include "Application.h"
#include "Log.h"
#include "FileSystem.h"
#include "PythonApplication.h"

#include "Python.h"

void CApplication::Run()
{
    PythonApplication pyApp;

    bool initSuccess = OnInit();

    if (!initSuccess)
    {
        TRACE_LOG(LOG_LEVEL_ERR, "Initial check failed, please close the application.");
        return;
    }

    TRACE_LOG(LOG_LEVEL_LOG, "Initial check succeeded.");

    pyApp.RunPythonScript("main.py");

    while (m_isRunning)
    {
        OnUpdate();
    }
}


void CApplication::OnUpdate()
{

}

bool CApplication::OnInit()
{
    bool allFilesWerePresent = true;

    for (int iter = 0; iter < FILE_MAX_LEN; ++iter)
    {
        if (!CFileSystem::Instance().FileExists(FILES[iter]))
        {
            CFileSystem::Instance().CreateFiles(FILES[iter]);
            TRACE_LOG(LOG_LEVEL_LOG, "File %s created.", FILES[iter]);
            allFilesWerePresent = false;
        }
    }

    bool allFilesNowExist = true;
    for (int iter = 0; iter < FILE_MAX_LEN; ++iter)
    {
        if (!CFileSystem::Instance().FileExists(FILES[iter]))
            allFilesNowExist = false;
    }

    return allFilesNowExist;
}
