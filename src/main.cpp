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
#include "credentialManager.h"

int main(int argc, char *argv[])
{
	CredentialManager credentials;
	if (argc == 3 && credentials.IsPasswordRequestFlag(argv[1]))
	{
		CredentialManager* shCreds = credentials.GetSharedManager();
		std::cerr << "found shCreds at " << (void*) shCreds << std::endl;
		std::string pw = shCreds->GetCredentials(argv[2]);
		std::cerr << "  pw = '" << pw << "'" << std::endl;
		if (pw.empty())
		{
			std::cerr << argv[2];
			std::cin >> pw;
			shCreds->AddCredentials(argv[2], pw);
		}
		else
			std::cout << pw << std::endl;

		return 0;
	}

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

	GitInterface gitIface;
	std::string repoPath;
	std::vector<GitInterface::RepositoryInfo> repoInfo;
	std::vector<std::string> directories(
		FileSystemNavigator::GetAllSubdirectories(searchPath));
	unsigned int i, repoCount(0);
	const std::string ignoreFileName(".ignore");
	for (i = 0; i < directories.size(); i++)
	{
		repoPath = searchPath + directories[i] + "/";
		std::ifstream ignoreFile((repoPath + ignoreFileName).c_str());
		if (ignoreFile.is_open())
			continue;

		repoInfo.push_back(GitInterface::GetRepositoryInfo(repoPath));
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
			else if (repoInfo.back().remotes.size() == 0)
				std::cout << "No remotes for " << repoInfo.back().name << std::endl;
			else
			{
				std::string errorList;
				if (gitIface.FetchAll(repoPath, errorList))
				{
					// TODO:  Check for mismatch between local heads and remote heads
					// For each branch:
					//   Case:  remote is behind, FF possible
					//   Case:  local is behind FF possible
					//   Case:  FF not possible
				}
				else
				{
					std::cout << "Failed to fetch remotes for " << repoInfo.back().name << "\n";
					std::cout << errorList << std::endl;
				}
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

	return 0;
}
