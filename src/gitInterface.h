// File:  gitInterface.h
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Interface to git for updating and checking repository status.

#ifndef GIT_INTERFACE_H_
#define GIT_INTERFACE_H_

// Standard C++ headers
#include <string>
#include <vector>

// Local headers
#include "credentialManager.h"

class GitInterface
{
public:

	struct BranchInfo
	{
		std::string name;
		std::string hash;
	};

	struct RemoteInfo
	{
		std::string name;
		std::vector<BranchInfo> branches;
	};

	struct RepositoryInfo
	{
		std::string name;
		bool isGitRepository;

		bool untrackedFiles;
		bool unstagedChanges;
		bool uncommittedChanges;

		std::vector<BranchInfo> branches;
		std::vector<RemoteInfo> remotes;

		std::vector<RepositoryInfo> subModules;
	};

	static std::string GetGitVersion();
	static RepositoryInfo GetRepositoryInfo(const std::string& path);
	bool UpdateLocal(const std::string& repositoryPath,
		const std::string& remote, std::string& branch);
	bool UpdateRemote(const std::string& repositoryPath,
		const std::string& remote, std::string& branch);

private:
	static const std::string stderrToNullFile;
	static const std::string gitName;
	static const std::string gitDirectoryArgument;
	static const std::string gitWorkTreeArgument;
	static const std::string gitGetVersionCmd;
	static const std::string gitGetUntrackedFilesCmd;
	static const std::string gitGetUnstagedChangesCmd;
	static const std::string gitGetUncommittedChangesCmd;
	static const std::string gitListBranchesCmd;
	static const std::string gitGetCurrentBranchCmd;
	static const std::string gitListRemotesCmd;

	static std::string GetLocalHead(const std::string& path,
		const std::string& branch);
	static std::string GetRemoteHead(const std::string& path,
		const std::string &remote, const std::string& branch);

	static std::string BuildCommand(const std::string& path,
		const std::string& command);
	static bool ExecuteCommand(const std::string& command, char* buffer,
		const unsigned int& bufferSize, const bool& suppressStderr = true);
	static int ExecuteCommand(const std::string& command, const bool& suppressStderr = true);

	static std::string ExtractLastDirectory(const std::string& path);

	CredentialManager credentials;
};

#endif