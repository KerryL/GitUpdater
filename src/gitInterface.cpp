// File:  gitInterface.cpp
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Interface to git for updating and checking repository status.

// Standard C/C++ headers
#include <stdio.h>
#include <sstream>
#include <iostream>

// Local headers
#include "gitInterface.h"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
const std::string GitInterface::stderrToNullFile(" 2> nul");
#else
const std::string GitInterface::stderrToNullFile(" 2> /dev/null");
#endif

const std::string GitInterface::gitName("git");
const std::string GitInterface::gitDirectoryArgument("--git-dir=");
const std::string GitInterface::gitWorkTreeArgument("--work-tree=");
const std::string GitInterface::gitGetVersionCmd("version");
const std::string GitInterface::gitGetUntrackedFilesCmd("ls-files --other --error-unmatch --exclude-standard");
const std::string GitInterface::gitGetUnstagedChangesCmd("diff --shortstat");
const std::string GitInterface::gitGetUncommittedChangesCmd("diff --cached --quiet HEAD");
const std::string GitInterface::gitListBranchesCmd("");
const std::string GitInterface::gitGetCurrentBranchCmd("rev-parse --abbrev-ref HEAD");
const std::string GitInterface::gitListRemotesCmd("");

std::string GitInterface::GetGitVersion()
{
	char buffer[1024];
	buffer[0] = '\0';
	if (!ExecuteCommand(gitName + " " + gitGetVersionCmd, buffer, sizeof(buffer)))
		return "";

	return buffer;
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

	int retVal = ExecuteCommand(BuildCommand(path, gitGetUncommittedChangesCmd));
	if (retVal == 129)
	{
		info.isGitRepository = false;
		return info;
	}
	info.uncommittedChanges = retVal != 0;

	char buffer[4096];
	buffer[0] = '\0';
	if (!ExecuteCommand(BuildCommand(path, gitGetUnstagedChangesCmd), buffer, sizeof(buffer)))
		std::cerr << "Failed to check for unstaged changes" << std::endl;
	info.unstagedChanges = !std::string(buffer).empty();

	buffer[0] = '\0';
	if (!ExecuteCommand(BuildCommand(path, gitGetUntrackedFilesCmd), buffer, sizeof(buffer)))
		std::cerr << "Failed to check for untracked files" << std::endl;
	info.untrackedFiles = !std::string(buffer).empty();

	// TODO:  Implement
	//info.branches;
	//info.remotes;
	//info.subModules;

	return info;
}

bool GitInterface::UpdateLocal(const std::string& repositoryPath,
	const std::string& remote, std::string& branch)
{
	// TODO:  Implement
	return false;
}

bool GitInterface::UpdateRemote(const std::string& repositoryPath,
	const std::string& remote, std::string& branch)
{
	// TODO:  Implement
	return false;
}

std::string GitInterface::GetLocalHead(const std::string& path,
	const std::string& branch)
{
	char buffer[1024];
	buffer[0] = '\0';
	if (!ExecuteCommand(BuildCommand(path, "rev-parse " + branch)))
		return "";
	return buffer;
}

std::string GitInterface::GetRemoteHead(const std::string& path,
	const std::string &remote, const std::string& branch)
{
	char buffer[1024];
	buffer[0] = '\0';
	if (!ExecuteCommand(BuildCommand(path,
		"rev-parse refs/remotes/" + remote + "/" + branch)))
		return "";
	return buffer;
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

bool GitInterface::ExecuteCommand(const std::string& command, char* buffer,
	const unsigned int& bufferSize, const bool& suppressStderr)
{
	std::string cmdString(command);
	if (suppressStderr)
		cmdString.append(stderrToNullFile);

	FILE* cmdFile = popen(cmdString.c_str(), "r");
	if (!cmdFile)
		return false;

	fgets(buffer, bufferSize, cmdFile);
	pclose(cmdFile);

	return true;
}

int GitInterface::ExecuteCommand(const std::string& command,
	const bool& suppressStderr)
{
	std::string cmdString(command);
	if (suppressStderr)
		cmdString.append(stderrToNullFile);

	return system(cmdString.c_str());
}