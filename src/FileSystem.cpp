#include "FileSystem.h"
#include <fstream>

#ifdef __linux__
#	include <linux/limits.h>
#endif

void CFileSystem::SetRootDirectory(std::string& rootDir)
{
	if (rootDir.empty())
	{
		rootDir = DIRECTORIES[0];
		return;
	}
	m_rootDir = rootDir;
}

std::filesystem::path CFileSystem::GetRootDirectory()
{
	std::filesystem::path rootPath = GetBaseDirectory() / "root";
	std::filesystem::create_directories(rootPath);
	return rootPath;
}

std::filesystem::path CFileSystem::GetBaseDirectory()
{
#ifdef __linux__
	// On Linux, use /proc/self/exe
	char result[PATH_MAX];
	size_t count = readlink("/proc/self/exe", result, PATH_MAX);
	return std::filesystem::path(std::string(result, (count > 0) ? count : 0)).parent_path();
#elif _WIN32

	char path[MAX_PATH];
	if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0)
		return "";
	return std::filesystem::path(path).parent_path();
#endif
}

std::filesystem::path CFileSystem::GetConfigDirectory()
{
	std::filesystem::path configPath = GetBaseDirectory() / "config";
	std::filesystem::create_directories(configPath);
	return configPath;
}

std::filesystem::path CFileSystem::GetLogDirectory()
{
	std::filesystem::path logPath = GetBaseDirectory() / "log";
	std::filesystem::create_directories(logPath);
	return logPath;
}

void CFileSystem::CreateFiles(const std::string& fileName)
{
	if (fileName.empty())
	{
#ifdef __linux__
		printf("CreateFiles failed: fileName is empty\n");
#elif _WIN32
		printf_s("CreateFiles failed: fileName is empty\n");
#endif
		return;
	}

	std::filesystem::path basePath;

	if (fileName.find(".cfg") != std::string::npos && fileName.find(".cfg") == fileName.size() - 4)
	{
		basePath = GetConfigDirectory();
	}
	else if (fileName == "syslog.txt" || fileName == "sysser.txt")
	{
		basePath = GetLogDirectory();
	}

	std::filesystem::path filePath = basePath / fileName;

	if (std::filesystem::exists(filePath))
	{
#ifdef _DEBUG
#	ifdef __linux__
		printf("Skipping creation: File %s already exists.\n", filePath.string().c_str());
#	elif _WIN32
		printf_s("Skipping creation: File %s already exists.\n", filePath.string().c_str());
#	endif
#endif
		return;
	}
#ifdef _DEBUG
#	ifdef __linux__
	printf("Attempting to create file: %s\n", filePath.string().c_str());
#	elif _WIN32
	printf_s("Attempting to create file: %s\n", filePath.string().c_str());
#	endif
#endif
	try
	{
		std::ofstream outFile(filePath, std::ios::trunc);
		if (!outFile)
		{
#ifdef __linux__
			printf("Failed to create file: %s\n", filePath.string().c_str());
#elif _WIN32
			printf_s("Failed to create file: %s\n", filePath.string().c_str());
#endif
			return;
		}
		outFile.close();
#ifdef _DEBUG
#	ifdef __linux__
		printf("File successfully created: %s\n", filePath.string().c_str());
#	elif _WIN32
		printf_s("File successfully created: %s\n", filePath.string().c_str());
#	endif
#endif
	}
	catch (const std::exception& e)
	{
#ifdef __linux__
		printf("Failed to create file: %s\nError: %s\n", filePath.string().c_str(), e.what());
#elif _WIN32
		printf_s("Failed to create file: %s\nError: %s\n", filePath.string().c_str(), e.what());
#endif
	}
}

bool CFileSystem::FileExists(const std::string& fileName)
{
	if (fileName.empty())
	{

#ifdef __linux__
		printf("FileExists failed: fileName is empty\n");
#elif _WIN32
		printf_s("FileExists failed: fileName is empty\n");
#endif
		return false;
	}

	std::filesystem::path filePath;

	if (fileName.find(".cfg") != std::string::npos && fileName.find(".cfg") == fileName.size() - 4)
		filePath = GetConfigDirectory() / fileName;
	else if (fileName == "syslog.txt" || fileName == "sysser.txt")
		filePath = GetLogDirectory() / fileName;

#ifdef _DEBUG
#	ifdef __linux__
	printf("Checking if file exists: %s\n", filePath.string().c_str());
#	elif _WIN32
	printf_s("Checking if file exists: %s\n", filePath.string().c_str());
#	endif
#endif

	bool exists = std::filesystem::exists(filePath);
#ifdef _DEBUG
#	ifdef __linux__
	printf("File %s exists: %s\n", filePath.string().c_str(), exists ? "YES" : "NO");
#	elif _WIN32
	printf_s("File %s exists: %s\n", filePath.string().c_str(), exists ? "YES" : "NO");
#	endif
#endif
	return exists;
}

bool CFileSystem::FileWrite(const std::string& fileName, const std::string& content)
{
	if (fileName.empty())
		return false;

	std::filesystem::path filePath;

	if (fileName.find(".cfg") != std::string::npos && fileName.find(".cfg") == fileName.size() - 4)
		filePath = GetConfigDirectory() / fileName;
	else if (fileName == "syslog.txt" || fileName == "sysser.txt")
		filePath = GetLogDirectory() / fileName;

	std::ofstream outFile(filePath, std::ios::out);
	if (!outFile)
	{
#ifdef __linux__
		printf("Failed to open file: %s\n", filePath.string().c_str());
#elif _WIN32
		printf_s("Failed to open file: %s\n", filePath.string().c_str());
#endif
		return false;
	}

	outFile << content;
	outFile.close();
#ifdef _DEBUG
#	ifdef __linux__
	printf("Successfully wrote to file: %s\n", filePath.string().c_str());
#	elif _WIN32
	printf_s("Successfully wrote to file: %s\n", filePath.string().c_str());
#	endif
#endif
	return true;
}

/* todo */
void CFileSystem::RegisterPythonFile(const std::string& py_fileName)
{

}
