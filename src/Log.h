#pragma once

#include <iostream>
#include <unordered_map>
#include <cstdarg>

/* set of log_level CLog::Instance().SetLogLevel(LOG_LEVEL_LOG); */

#define TRACE_LOG(log_level, format, ...) \
	CLog::Instance().TraceLog(log_level, format, ##__VA_ARGS__)

const uint8_t FILE_MAX_LEN = 2;

enum eLogLevel
{
	LOG_LEVEL_NONE,
	LOG_LEVEL_LOG,
	LOG_LEVEL_ERR,
	LOG_LEVEL_MAX_NUM
};

static std::string FILES[FILE_MAX_LEN] = {
	"syslog.txt",
	"sysser.txt"
};

class CLog
{
public:
	void TraceLog(eLogLevel level, const char* format, ...);

	void SetLogLevel(eLogLevel logLevel);
	inline eLogLevel GetLogLevel() { return m_logLevel; }

	static CLog& Instance()
	{
		static CLog s_instance;
		return s_instance;
	}

	CLog(const CLog&) = delete;

private:
	CLog() : m_logLevel(LOG_LEVEL_LOG) {}
	~CLog() = default;

	eLogLevel m_logLevel;

	std::unordered_map<eLogLevel, std::string> logLevelMap = {
		{ LOG_LEVEL_NONE, "<NONE>" },
		{ LOG_LEVEL_LOG, "<LOG>" },
		{ LOG_LEVEL_ERR, "<ERROR>" }
	};
};