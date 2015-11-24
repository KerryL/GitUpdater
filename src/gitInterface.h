// File:  gitInterface.h
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Interface to git for updating and checking repository status.

#ifndef GIT_INTERFACE_H_
#define GIT_INTERFACE_H_

// Standard C++ headers
#include <string>
#include <vector>

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
	bool FetchAll(const std::string& path, std::string& errorString);
	bool UpdateRemote(const std::string& path,
		const std::string& remote, std::string& branch);// TODO:  Rename "push to remote?"

	enum RepositoryStatus
	{
		StatusUpToDate,
		StatusLocalAhead,
		StatusRemoteAhead,
		StatusRemoteMissingBranch,
		StatusLocalMissingBranch
	};

	static RepositoryStatus CompareHeads(const std::string& path,
		const RepositoryInfo& repoInfo, const std::string& remote,
		const std::string& branch);

private:
	static const std::string gitName;
	static const std::string gitDirectoryArgument;
	static const std::string gitWorkTreeArgument;
	static const std::string gitGetVersionCmd;
	static const std::string gitGetUntrackedFilesCmd;
	static const std::string gitGetUnstagedChangesCmd;
	static const std::string gitGetUncommittedChangesCmd;
	static const std::string gitListBranchesCmd;
	static const std::string gitListRemoteBranchesCmd;
	static const std::string gitGetCurrentBranchCmd;
	static const std::string gitListRemotesCmd;
	static const std::string gitFetchAllCmd;
	static const std::string gitFailMessage;

	static std::string GetLocalHead(const std::string& path,
		const std::string& branch);
	static std::string GetRemoteHead(const std::string& path,
		const std::string &remote, const std::string& branch);

	static std::string BuildCommand(const std::string& path,
		const std::string& command);

	static std::string ExtractLastDirectory(const std::string& path);
	static std::vector<std::string> SplitBufferByLine(const std::string& buffer);

	static std::vector<RemoteInfo> BuildRemotes(const std::string& path,
		const std::vector<std::string>& remotes);
	static RemoteInfo BuildRemote(const std::string& path,
		const std::string& remote);
	static std::vector<BranchInfo> BuildBranches(const std::string& path,
		const std::vector<std::string>& branches);
	static BranchInfo BuildBranch(const std::string& path,
		const std::string& branch);
	static BranchInfo BuildBranch(const std::string& path,
		const std::string& remote, const std::string& branch);
	static std::string CleanName(const std::string &name);
};

#endif