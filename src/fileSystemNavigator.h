// File:  fileSystemNavigator.h
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Navigates file system.

#ifndef FILE_SYSTEM_NAVIGATOR_H_
#define FILE_SYSTEM_NAVIGATOR_H_

// Standard C++ headers
#include <vector>
#include <string>

class FileSystemNavigator
{
public:
	static std::vector<std::string> GetAllFiles(const std::string& path,
		const bool& includeSymLinks = true);
	static std::vector<std::string> GetAllSubdirectories(const std::string& path);

private:
	static std::vector<std::string> GetAll(const std::string& path,
		const int& typeMask);
	static std::vector<std::string> RemoveDotDirectories(std::vector<std::string>& v);
};

#endif// FILE_SYSTEM_NAVIGATOR_H_