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
#ifdef __linux__
        printf("CLog::TraceLog not a value in eLogLevel range 1-2 current value %d", level);
#elif _WIN32
        printf_s("CLog::TraceLog not a value in eLogLevel range 1-2 current value %d", level);
#endif
        return;
    }

    auto currentTime = std::chrono::system_clock::now();
    std::time_t t_now = std::chrono::system_clock::to_time_t(currentTime);
    std::tm timeInfo;
#ifdef __linux__
    localtime_r(&t_now, &timeInfo);
#elif _WIN32
    localtime_s(&timeInfo, &t_now);
#endif

    std::string logFileName = (level == LOG_LEVEL_ERR) ? FILES[1] : FILES[0];

    std::filesystem::path logFilePath = CFileSystem::Instance().GetLogDirectory() / logFileName;

    CFileSystem::Instance().CreateFiles(logFileName);

    std::ofstream logFile(logFilePath, std::ios::app);

    if (!logFile)
    {
#ifdef __linux__
        printf("Failed to open log file: %s\n", logFilePath.string().c_str());
#elif _WIN32
        printf_s("Failed to open log file: %s\n", logFilePath.string().c_str());
#endif
        return;
    }

    char logMessage[1024];
    va_list args;
    va_start(args, format);

#ifdef __linux__
    vsprintf(logMessage, format, args);
#elif _WIN32
    vsprintf_s(logMessage, format, args);
#endif
    va_end(args);

#ifdef __linux__
    printf("[%04d-%02d-%02d %02d:%02d:%02d] %s %s\n",
#elif _WIN32
    printf_s("[%04d-%02d-%02d %02d:%02d:%02d] %s %s\n",
#endif
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

void CLog::SetLogLevel(const eLogLevel logLevel)
{
	m_logLevel = logLevel;
}