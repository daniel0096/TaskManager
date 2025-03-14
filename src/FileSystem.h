#pragma once

#include "Windows.h"
#include <filesystem>

enum eFileDirType
{
	FILE_DIR_NONE,
	FILE_DIR_ROOT,
	FILE_DIR_LOG,
	FYLE_DIR_CONFIG,
	FILE_DIR_MAX_NUM
};

enum ePythonFile
{
	PY_FILE_NONE,
	PY_FILE_MAIN,
	PY_FILE_APPLICATION,
	PY_FILE_MAX_NUM
};

static std::string DIRECTORIES[FILE_DIR_MAX_NUM - 1] =
{
	"root",
	"log",
	"config"
};

static std::string PYTHON_FIlES[PY_FILE_MAX_NUM - 1] =
{
	"main",
	"application",
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
	void RegisterPythonFile(const std::string& py_fileName);

	static CFileSystem& Instance()
	{
		static CFileSystem s_instance;
		return s_instance;
	}

	CFileSystem(const CFileSystem&) = delete;
private:
	CFileSystem() = default;

	/*todo when destructor is called remove log files*/
	~CFileSystem() = default;

	std::string m_rootDir;
	std::string m_baseOutputDir;
	std::string m_configDir;
	bool m_fileExists;
};