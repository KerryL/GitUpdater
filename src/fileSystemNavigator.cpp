// File:  fileSystemNavigator.cpp
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Navigates file system.

// Standard C++ headers
#include <algorithm>

#ifdef _WIN32
// Windows headers
#include "windirent.h"// Local, but 3rd party
#else
#include <unistd.h>
#include <dirent.h>
#endif

// Local headers
#include "fileSystemNavigator.h"

std::vector<std::string> FileSystemNavigator::GetAllFiles(
	const std::string& path, const bool& includeSymLinks)
{
	int typeMask(DT_REG);
	if (includeSymLinks)
		typeMask |= DT_LNK;
	return GetAll(path, typeMask);
}

std::vector<std::string> FileSystemNavigator::GetAllSubdirectories(
	const std::string& path)
{
	return RemoveDotDirectories(GetAll(path, DT_DIR));
}

std::vector<std::string> FileSystemNavigator::GetAll(const std::string& path,
	const int& typeMask)
{
	DIR *dp;
	dirent *d;

	std::vector<std::string> list;
	dp = opendir(path.c_str());
	while ((d = readdir(dp)) != NULL)
	{
		if (d->d_type & typeMask)
			list.push_back(d->d_name);
	}
		
	std::sort(list.begin(), list.end());
	return list;
}

std::vector<std::string> FileSystemNavigator::RemoveDotDirectories(
	std::vector<std::string>& v)
{
	unsigned int i;
	for (i = 0; i < v.size(); i++)
	{
		if (v[i].compare(".") == 0 || v[i].compare("..") == 0)
		{
			v.erase(v.begin() + i);
			i--;
		}
	}

	return v;
}