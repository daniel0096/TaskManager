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
	bool SetupConfigFile(const std::string& fileName);
	void LoadDefaultConfig();
	bool WriteConfigToFile(const std::string& fileName);
private:
	std::map<std::string, std::string> m_configContent;
};