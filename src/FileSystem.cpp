#include "FileSystem.h"
#include <fstream>

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
	char path[MAX_PATH];

	if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0)
		return "";

	return std::filesystem::path(path).parent_path();
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
		printf_s("CreateFiles failed: fileName is empty\n");
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
		printf_s("Skipping creation: File %s already exists.\n", filePath.string().c_str());
		return;
	}

	printf_s("Attempting to create file: %s\n", filePath.string().c_str());

	try
	{
		std::ofstream outFile(filePath, std::ios::trunc);
		if (!outFile)
		{
			printf_s("Failed to create file: %s\n", filePath.string().c_str());
			return;
		}
		outFile.close();
		printf_s("File successfully created: %s\n", filePath.string().c_str());
	}
	catch (const std::exception& e)
	{
		printf_s("Failed to create file: %s\nError: %s\n", filePath.string().c_str(), e.what());
	}
}

bool CFileSystem::FileExists(const std::string& fileName)
{
	if (fileName.empty())
	{
		printf_s("FileExists failed: fileName is empty\n");
		return false;
	}

	std::filesystem::path filePath;

	if (fileName.find(".cfg") != std::string::npos && fileName.find(".cfg") == fileName.size() - 4)
		filePath = GetConfigDirectory() / fileName;
	else if (fileName == "syslog.txt" || fileName == "sysser.txt")
		filePath = GetLogDirectory() / fileName;

	printf_s("Checking if file exists: %s\n", filePath.string().c_str());

	bool exists = std::filesystem::exists(filePath);

	printf_s("File %s exists: %s\n", filePath.string().c_str(), exists ? "YES" : "NO");

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
		printf_s("Failed to open file: %s\n", filePath.string().c_str());
		return false;
	}

	outFile << content;
	outFile.close();

	printf_s("Successfully wrote to file: %s\n", filePath.string().c_str());
	return true;
}

/* todo */
void CFileSystem::RegisterPythonFile(const std::string& py_fileName)
{

}
