#include "directory-file-reader.h"
#include <filesystem>
#include <fstream>

DirectoryFileReader::DirectoryFileReader(const std::string& rootDirectory)
{
	if (std::filesystem::is_directory(rootDirectory))
	{
		m_rootDirectory = rootDirectory;
	}
}

bool DirectoryFileReader::FileExists(const std::string& fileName)
{
	return std::filesystem::exists(m_rootDirectory + '/' + fileName);
}

std::string DirectoryFileReader::ReadAllFile(const std::string& fileName)
{
	std::ifstream file = std::ifstream(m_rootDirectory + '/' + fileName);
	if (not file.is_open())
	{
		return "";
	}

	std::stringstream fileContent;
	std::string line;

	while (std::getline(file, line))
		fileContent << line;
	
	file.close();

	return fileContent.str();
}
