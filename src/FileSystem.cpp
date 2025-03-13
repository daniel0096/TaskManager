#include "FileSystem.h"
#include <fstream>

/* rewrite this */

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
    if (m_baseOutputDir.empty())
        return std::filesystem::path(GetBaseDirectory()) / DIRECTORIES[0];

    return std::filesystem::path(m_baseOutputDir);
}

void CFileSystem::SetBaseOutputDir(std::string& outputDir)
{
    if (outputDir.empty())
    {
        outputDir = DIRECTORIES[1];
        return;
    }
    m_baseOutputDir = outputDir;
}

std::filesystem::path CFileSystem::GetBaseOutputDirectory()
{
    if (m_baseOutputDir.empty())
        return std::filesystem::path(GetBaseDirectory()) / DIRECTORIES[1];

    return std::filesystem::path(m_baseOutputDir);
}

void CFileSystem::CreateFiles(const std::string& fileName)
{
    if (fileName.empty())
        return;

    if (!FileExists(fileName))
    {
        std::filesystem::path filePath = GetBaseDirectory() / GetBaseOutputDirectory() / fileName;

        std::filesystem::create_directories(filePath.parent_path());

        std::ofstream outFile(filePath);
    }
}

std::filesystem::path CFileSystem::GetBaseDirectory()
{
    char path[MAX_PATH];

    if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0)
        return "";

    return std::filesystem::path(path).parent_path();
}

bool CFileSystem::FileExists(const std::string& fileName)
{
    if (!std::filesystem::exists(GetRootDirectory()))
    {
        printf_s("Directory %s does not exist, application terminated.\n", GetRootDirectory().string().c_str());
        return false;
    }

	if (fileName.empty())
		return false;

    std::filesystem::path filePath = GetBaseDirectory() / GetBaseOutputDirectory() / fileName;
    return std::filesystem::exists(filePath);
}

/* rewrite this */