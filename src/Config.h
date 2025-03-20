#pragma once

#include <string>
#include <map>

/* default values */
typedef struct sConfig
{
	int width = 800;
	int height = 600;
}TConfig;
/* - - - - - - - */

class CConfig
{
public:
	CConfig();
	bool SetupConfigFile(const std::string& fileName);
	void LoadDefaultConfig();
	bool WriteConfigToFile(const std::string& fileName);
	bool ReadConfig();
	inline const std::map<std::string, std::string>& GetConfigSettings() const { return m_configContent; }
	
private:
	std::map<std::string, std::string> m_configContent;
	
};