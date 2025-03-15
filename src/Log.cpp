#include "Log.h"
#include "FileSystem.h" 

#include <chrono>
#include <iomanip>
#include <ctime>

#include <fstream>
#include <filesystem>
#include <cstdarg>

void CLog::TraceLog(eLogLevel level, const char* format, ...)
{
    if ((level >= LOG_LEVEL_MAX_NUM) || (level <= LOG_LEVEL_NONE))
    {
        printf_s("CLog::TraceLog not a value in eLogLevel range 1-2 current value %d", level);
        return;
    }

    auto currentTime = std::chrono::system_clock::now();
    std::time_t t_now = std::chrono::system_clock::to_time_t(currentTime);
    std::tm timeInfo;
    localtime_s(&timeInfo, &t_now);

    std::string logFileName = (level == LOG_LEVEL_ERR) ? FILES[1] : FILES[0];

    std::filesystem::path logFilePath = CFileSystem::Instance().GetLogDirectory() / logFileName;

    CFileSystem::Instance().CreateFiles(logFileName);

    std::ofstream logFile(logFilePath, std::ios::app);

    if (!logFile)
    {
        printf_s("Failed to open log file: %s\n", logFilePath.string().c_str());
        return;
    }

    char logMessage[1024];
    va_list args;
    va_start(args, format);
    vsprintf_s(logMessage, format, args);
    va_end(args);

    printf_s("[%04d-%02d-%02d %02d:%02d:%02d] %s %s\n",
        timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
        timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec,
        logLevelMap[level].c_str(), logMessage);

    logFile << "[" << timeInfo.tm_year + 1900 << "-"
        << timeInfo.tm_mon + 1 << "-"
        << timeInfo.tm_mday << " "
        << timeInfo.tm_hour << ":"
        << timeInfo.tm_min << ":"
        << timeInfo.tm_sec << "] "
        << logLevelMap[level] << " "
        << logMessage << std::endl;

    logFile.close();
}

void CLog::SetLogLevel(eLogLevel logLevel)
{
	m_logLevel = logLevel;
}