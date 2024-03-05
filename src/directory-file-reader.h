#pragma once
#include <string>

class DirectoryFileReader
{
public:
	DirectoryFileReader(const std::string& rootFolder);
	bool FileExists(const std::string& fileName);
	std::string ReadAllFile(const std::string& fileName);
private:
	std::string m_rootDirectory;
};