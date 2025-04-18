#include "Config.h"
#include "FileSystem.h"
#include "Log.h"

#include <fstream>
#include <sstream>

CConfig::CConfig()
{
	/*todo*/
	/*rewrite -> LoadDefaultConfig only in case the SET config is not available!*/
	LoadDefaultConfig();
	/*read set config*/
	//ReadConfig();
}

bool CConfig::SetupConfigFile(const std::string& fileName)
{
//	LoadDefaultConfig();

	if (!CFileSystem::Instance().FileExists(fileName))
	{
#ifdef _DEBUG
		TRACE_LOG(LOG_LEVEL_LOG, "Config file does not exist. Creating %s.", fileName.c_str());
#endif
		CFileSystem::Instance().CreateFiles(fileName);

		if (!CFileSystem::Instance().FileExists(fileName))
		{
			TRACE_LOG(LOG_LEVEL_ERR, "Failed to create config file: %s", fileName.c_str());
			return false;
		}
#ifdef _DEBUG
		TRACE_LOG(LOG_LEVEL_LOG, "Config file created, attempting to write...");
	}
	else
		TRACE_LOG(LOG_LEVEL_LOG, "Config file %s already exists, checking writability...", fileName.c_str());
#else
	}
#endif

	std::ofstream testWrite(fileName, std::ios::app);
	if (!testWrite)
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Config file %s exists but is NOT writable!", fileName.c_str());
		return false;
	}
	else
	{
#ifdef _DEBUG
		TRACE_LOG(LOG_LEVEL_LOG, "Config file %s is writable, writing config...", fileName.c_str());
#endif
		testWrite.close();
	}

	if (!WriteConfigToFile(fileName))
	{
		TRACE_LOG(LOG_LEVEL_ERR, "Failed to write to config file: %s", fileName.c_str());
		return false;
	}
#ifdef _DEBUG
	TRACE_LOG(LOG_LEVEL_LOG, "Config file successfully written.");
#endif
	return true;
}

//bool CConfig::ConfigFileSet()
//{
//
//}

void CConfig::LoadDefaultConfig()
{
#ifdef _DEBUG
	TRACE_LOG(LOG_LEVEL_LOG, "Loading default config...");
#endif

	m_configContent["res_x"] = std::to_string(800);
	m_configContent["res_y"] = std::to_string(600);

#ifdef _DEBUG
	for (const auto& [key, value] : m_configContent)
		TRACE_LOG(LOG_LEVEL_LOG, "Default config: %s = %s", key.c_str(), value.c_str());
#endif
}

//bool CConfig::ReadConfig()
//{
//
//}

bool CConfig::WriteConfigToFile(const std::string& fileName)
{
	if (m_configContent.empty())
	{
#ifdef _DEBUG
		TRACE_LOG(LOG_LEVEL_LOG, "Config map is empty, loading defaults.");
#endif
		//		LoadDefaultConfig();
	}

	std::stringstream configStream;
	for (const auto& [key, value] : m_configContent)
		configStream << key << "=" << value << std::endl;

	bool success = CFileSystem::Instance().FileWrite(fileName, configStream.str());

	if (!success)
		TRACE_LOG(LOG_LEVEL_ERR, "Failed to write to config file: %s", fileName.c_str());

	return success;
}
