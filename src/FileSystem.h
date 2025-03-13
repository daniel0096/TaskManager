#pragma once

#include "Windows.h"
#include <filesystem>

const int DIRECTORY_MAX_LEN = 2;

static std::string DIRECTORIES[DIRECTORY_MAX_LEN] =
{
	"root",
	"log"
};

class CFileSystem
{
public:
	/* rewrite this */

	void SetRootDirectory(std::string& rootDir);
	std::filesystem::path GetRootDirectory();

	void SetBaseOutputDir(std::string& outputDir);
	std::filesystem::path GetBaseOutputDirectory();

	void CreateFiles(const std::string& fileName);
	std::filesystem::path GetBaseDirectory();
	bool FileExists(const std::string& fileName);

	/* rewrite this */



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
	bool m_fileExists;
};