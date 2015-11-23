// File:  gitInterface.cpp
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Interface to git for updating and checking repository status.

// Standard C/C++ headers
#include <sstream>
#include <iostream>
#include <cassert>
#include <cstdlib>

// Local headers
#include "gitInterface.h"
#include "shellInterface.h"
#include "stringUtilities.h"

#ifdef _WIN32
#define putenv _putenv
#endif

const std::string GitInterface::gitName("git");
const std::string GitInterface::gitDirectoryArgument("--git-dir=");
const std::string GitInterface::gitWorkTreeArgument("--work-tree=");
const std::string GitInterface::gitGetVersionCmd("version");
const std::string GitInterface::gitGetUntrackedFilesCmd("ls-files --other --error-unmatch --exclude-standard");
const std::string GitInterface::gitGetUnstagedChangesCmd("diff --shortstat");
const std::string GitInterface::gitGetUncommittedChangesCmd("diff --cached --quiet HEAD");
const std::string GitInterface::gitListBranchesCmd("branch");
const std::string GitInterface::gitListRemoteBranchesCmd("branch -r");
const std::string GitInterface::gitGetCurrentBranchCmd("rev-parse --abbrev-ref HEAD");
const std::string GitInterface::gitListRemotesCmd("remote");
const std::string GitInterface::gitFetchAllCmd("fetch --all --prune --tags");
const std::string GitInterface::gitFailMessage("fatal:");

std::string GitInterface::GetGitVersion()
{
	ShellInterface shell;
	std::string version;
	if (!shell.ExecuteCommand(gitName + " " + gitGetVersionCmd, version))
		return "";
	return version;
}

std::string GitInterface::ExtractLastDirectory(const std::string& path)
{
	std::string d(path);
	if (d.back() == '\\' || d.back() == '/')
		d = d.substr(0, d.length() - 1);

	size_t slashLocation;
	slashLocation = d.find_last_of('/');
	if (slashLocation == std::string::npos)
		slashLocation = d.find_last_of('\\');

	if (slashLocation != std::string::npos)
		d = d.substr(slashLocation + 1);

	return d;
}

GitInterface::RepositoryInfo GitInterface::GetRepositoryInfo(
	const std::string& path)
{
	RepositoryInfo info;
	info.name = ExtractLastDirectory(path);

	ShellInterface shell;
	shell.ExecuteCommand(BuildCommand(path, gitGetUncommittedChangesCmd));
	if (shell.GetExitCode() == 129)
	{
		info.isGitRepository = false;
		return info;
	}
	else
		info.isGitRepository = true;
	info.uncommittedChanges = shell.GetExitCode() != 0;

	std::string stdOut;
	if (!shell.ExecuteCommand(BuildCommand(path, gitGetUnstagedChangesCmd),
		stdOut, ShellInterface::RedirectErrToOut))
		std::cerr << "Failed to check for unstaged changes" << std::endl;
	info.unstagedChanges = !stdOut.empty();

	if (!shell.ExecuteCommand(BuildCommand(path, gitGetUntrackedFilesCmd),
		stdOut, ShellInterface::RedirectErrToOut))
		std::cerr << "Failed to check for untracked files" << std::endl;
	info.untrackedFiles = !stdOut.empty();

	if (!shell.ExecuteCommand(BuildCommand(path, gitListRemotesCmd), stdOut,
		ShellInterface::RedirectErrToOut))
		std::cerr << "Failed to list remotes" << std::endl;
	info.remotes = BuildRemotes(path, SplitBufferByLine(stdOut));

	if (!shell.ExecuteCommand(BuildCommand(path, gitListBranchesCmd), stdOut,
		ShellInterface::RedirectErrToOut))
		std::cerr << "Failed to list branches" << std::endl;
	info.branches = BuildBranches(path, SplitBufferByLine(stdOut));

	// TODO:  Implement
	//info.subModules;

	return info;
}

std::vector<GitInterface::RemoteInfo> GitInterface::BuildRemotes(
	const std::string& path, const std::vector<std::string>& remotes)
{
	std::vector<RemoteInfo> info(remotes.size());
	unsigned int i;
	for (i = 0; i < remotes.size(); i++)
		info[i] = BuildRemote(path, remotes[i]);
	return info;
}

GitInterface::RemoteInfo GitInterface::BuildRemote(const std::string& path,
	const std::string& remote)
{
	RemoteInfo info;
	info.name = remote;

	ShellInterface shell;
	std::string stdOut;
	if (!shell.ExecuteCommand(BuildCommand(path, gitListRemoteBranchesCmd), stdOut))
		std::cerr << "Failed to list remote branches" << std::endl;
	std::vector<std::string> branches = SplitBufferByLine(stdOut);
	unsigned int i;
	size_t slashLocation;
	for (i = 0; i < branches.size(); i++)
	{
		slashLocation = branches[i].find('/');
		assert(slashLocation != std::string::npos);

		if (slashLocation < remote.length())
			continue;
		else if (branches[i].substr(slashLocation - remote.length(), remote.length()).compare(remote) == 0)
		{
			// Hack to fix problems that can occur when a repo has an entry like "origin/HEAD -> origin/master"
			slashLocation = branches[i].find_last_of('/');
			assert(slashLocation != std::string::npos);
			info.branches.push_back(BuildBranch(path, remote, branches[i].substr(slashLocation + 1)));
		}
	}

	
	//info.branches// TODO

	return info;
}

std::string GitInterface::CleanBranchName(const std::string &name)
{
	std::string cleanName(Trim(name));
	assert(name.length() > 0);
	if (name[0] == '*')
		cleanName = Trim(cleanName.substr(1));
	return cleanName;
}

std::vector<GitInterface::BranchInfo> GitInterface::BuildBranches(
	const std::string& path, const std::vector<std::string>& branches)
{
	std::vector<BranchInfo> info(branches.size());
	unsigned int i;
	for (i = 0; i < branches.size(); i++)
		info[i] = BuildBranch(path, branches[i]);
	return info;
}

GitInterface::BranchInfo GitInterface::BuildBranch(const std::string& path,
	const std::string& branch)
{
	BranchInfo info;
	info.name = branch;
	info.hash = GetLocalHead(path, CleanBranchName(branch));
	return info;
}

GitInterface::BranchInfo GitInterface::BuildBranch(const std::string& path,
	const std::string& remote, const std::string& branch)
{
	BranchInfo info;
	info.name = branch;
	info.hash = GetRemoteHead(path, remote, branch);
	return info;
}

std::vector<std::string> GitInterface::SplitBufferByLine(const std::string& buffer)
{
	std::vector<std::string> lines;
	std::istringstream ss(buffer);
	std::string token;
	while (std::getline(ss, token))
		lines.push_back(token);
	return lines;
}

bool GitInterface::FetchAll(const std::string& path, std::string& errorString)
{
	ShellInterface shell;
	std::string stdOut;

	const std::string gitAskPassVar("GIT_ASKPASS");
	char *origGitAskpass = std::getenv(gitAskPassVar.c_str());
	const std::string sshAskPassVar("SSH_ASKPASS");
	char *origSshAskpass = std::getenv(sshAskPassVar.c_str());
	std::string cmd(gitAskPassVar + std::string("=C:\\Users\\kloux\\Documents\\Visual Studio 2010\\Projects\\GitUpdater\\Debug\\GitUpdater.exe --pwRequest"));
	putenv(cmd.c_str());
	cmd =sshAskPassVar + std::string("=C:\\Users\\kloux\\Documents\\Visual Studio 2010\\Projects\\GitUpdater\\Debug\\GitUpdater.exe --pwRequest");
	putenv(cmd.c_str());

	if (!shell.ExecuteCommand(BuildCommand(path, gitFetchAllCmd), stdOut,
		ShellInterface::RedirectErrToOut))
	{
		std::cerr << "Failed to execute fetch command" << std::endl;
		return false;
	}

	if (origGitAskpass)
		putenv(std::string(gitAskPassVar + "=" + origGitAskpass).c_str());
	else
	{
		//unsetenv(
	}

	if (origSshAskpass)
		putenv(std::string(sshAskPassVar + "=" + origSshAskpass).c_str());
	else
	{
		//unsetenv(
	}

	std::istringstream ss(stdOut);
	if (ss.str().length() == 0)
	{
		std::cerr << "No remotes to fetch" << std::endl;
		return false;
	}

	bool fetchedAll(true);
	std::string response, remoteName;
	const std::string fetching("Fetching");
	while (std::getline(ss, response))
	{
		if (response.length() > fetching.length() &&
			response.substr(0, fetching.length()).compare(fetching) == 0)
			remoteName = Trim(response.substr(fetching.length()));
		else if (response.length() > gitFailMessage.length() &&
			response.substr(0, gitFailMessage.length()).compare(gitFailMessage) == 0)
		{
			std::ostringstream errorStream;
			errorStream << "  Failed to fetch from " << remoteName << "\n";
			errorString.append(errorStream.str());
			fetchedAll = false;
		}
	}

	return fetchedAll;
}

bool GitInterface::UpdateRemote(const std::string& /*path*/,
	const std::string& /*remote*/, std::string& /*branch*/)
{
	// TODO:  Implement
	return false;
}

std::string GitInterface::GetLocalHead(const std::string& path,
	const std::string& branch)
{
	ShellInterface shell;
	std::string stdOut;
	if (!shell.ExecuteCommand(BuildCommand(path, "rev-parse " + branch), stdOut))
		return "";
	return stdOut;
}

std::string GitInterface::GetRemoteHead(const std::string& path,
	const std::string &remote, const std::string& branch)
{
	ShellInterface shell;
	std::string stdOut;
	if (!shell.ExecuteCommand(BuildCommand(path,
		"rev-parse refs/remotes/" + remote + "/" + branch), stdOut))
		return "";
	return stdOut;
}

std::string GitInterface::BuildCommand(const std::string &path,
	const std::string &command)
{
	std::string gitDirPath(path);
#ifdef _WIN32
	if (gitDirPath.back() != '\\')
		gitDirPath.append("\\");
#else
	if (gitDirPath.back() != '/')
		gitDirPath.append("/");
#endif
	gitDirPath.append(".git");
	return gitName + " " + gitDirectoryArgument + "\"" + gitDirPath + "\" "
		+ gitWorkTreeArgument + "\"" + path + "\" " + command;
}
