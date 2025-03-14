#pragma once

#include "Windows.h"
#include <filesystem>

const int DIRECTORY_MAX_LEN = 3;

static std::string DIRECTORIES[DIRECTORY_MAX_LEN] =
{
	"root",
	"log",
	"config"
};

class CFileSystem
{
public:
	/* rewrite this */

	std::filesystem::path GetLogDirectory();

	void SetRootDirectory(std::string& rootDir);
	std::filesystem::path GetRootDirectory();

	std::filesystem::path GetConfigDirectory();

	void CreateFiles(const std::string& fileName);
	std::filesystem::path GetBaseDirectory();
	bool FileExists(const std::string& fileName);

	/* rewrite this */

	bool FileWrite(const std::string& fileName, const std::string& content);

	static CFileSystem& Instance()
	{
		static CFileSystem s_instance;
		return s_instance;
	}

	CFileSystem(const CFileSystem&) = delete;
private:
	CFileSystem() = default;
	~CFileSystem() = default;

	std::string m_rootDir;
	std::string m_baseOutputDir;
	std::string m_configDir;
	bool m_fileExists;
};