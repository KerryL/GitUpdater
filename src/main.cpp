// File:  main.cpp
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Application entry point for checking git repo status.

// Standard C++ headers
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <utility>

// Local headers
#include "gitInterface.h"
#include "fileSystemNavigator.h"

int main(int argc, char *argv[])
{
	std::string gitVersion = GitInterface::GetGitVersion();
	if (gitVersion.empty())
	{
		std::cerr << "Failed to find git.  Make sure it is installed and on your path." << std::endl;
		return 1;
	}
	std::cout << gitVersion << std::endl;

	if (argc < 2)
	{
		std::cerr << "" << std::endl;
		return 1;
	}
	std::string searchPath(argv[1]);
	if (searchPath.back() != '/')
		searchPath.append("/");

	std::vector<GitInterface::RepositoryInfo> repoInfo;
	std::vector<std::string> directories(
		FileSystemNavigator::GetAllSubdirectories(searchPath));
	unsigned int i, repoCount(0);
	const std::string ignoreFile(".ignore");
	for (i = 0; i < directories.size(); i++)
	{
		std::ifstream ignoreFile((searchPath + directories[i] + "/"
			+ ignoreFile).c_str());
		if (ignoreFile.is_open())
			continue;

		repoInfo.push_back(GitInterface::GetRepositoryInfo(
			searchPath + directories[i]));
		if (repoInfo.back().isGitRepository)
		{
			repoCount++;
			if (repoInfo.back().uncommittedChanges ||
				repoInfo.back().unstagedChanges ||
				repoInfo.back().untrackedFiles)
			{
				std::cout << repoInfo.back().name << "\n";
				if (repoInfo.back().uncommittedChanges)
					std::cout << "  -> Uncommitted changes" << std::endl;
				if (repoInfo.back().unstagedChanges)
					std::cout << "  -> Unstaged changes" << std::endl;
				if (repoInfo.back().untrackedFiles)
					std::cout << "  -> Untracked files" << std::endl;
			}
			else
			{
				// TODO:  Do pull/push stuff here
			}
		}
		else
			std::cout << repoInfo.back().name << " is not a git repository" << std::endl;
	}

	if (repoCount == 0)
	{
		std::cout << "Failed to find any git repositories under '" << searchPath << "'" << std::endl;
		return 0;
	}
	std::cout << "\nFound " << repoCount << " git repositories" << std::endl;

	GitInterface gitIface;

	return 0;
}