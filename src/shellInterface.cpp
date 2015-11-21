// File:  shellInterface.cpp
// Date:  11/20/2015
// Auth:  K. Loux
// Desc:  Interface to system shell.

// Standard C/C++ headers
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <cassert>

// Local headers
#include "shellInterface.h"

const std::string ShellInterface::stderrToStdout(" 2>&1");
#ifdef _WIN32
#define popen _popen
#define pclose _pclose
const std::string ShellInterface::stdoutToNullFile("> nul");
const std::string ShellInterface::stderrToNullFile("2> nul");
const std::string ShellInterface::allToNullFile("> nul 2>&1");
#else
const std::string ShellInterface::stdoutToNullFile(" > /dev/null");
const std::string ShellInterface::stderrToNullFile(" 2> /dev/null");
const std::string ShellInterface::allToNullFile(" > /dev/null 2>&1");
#endif

ShellInterface::ShellInterface()
{
	exitCode = 0;
	cmdFile = NULL;
}

ShellInterface::~ShellInterface()
{
	if (cmdFile)
		StopInteractive();
}

int ShellInterface::ExecuteCommand(const std::string& command,
	const RedirectFlags& f)
{
//std::cout << "exec1 " << command << std::endl;
	std::string cmdString(command + BuildRedirectString(f));
	exitCode = system(cmdString.c_str());
#ifndef _WIN32
	exitCode = WEXITSTATUS(exitCode);
#endif
	return exitCode;
}

bool ShellInterface::ExecuteCommand(const std::string& command,
	std::string& stdOut, const RedirectFlags& f)
{
//std::cout << "exec " << command << std::endl;
	stdOut.clear();
	std::string cmdString(command + BuildRedirectString(f));

	assert(!cmdFile);
	cmdFile = popen(cmdString.c_str(), "r");
	if (!cmdFile)
		return false;

	char buffer[2048];
	buffer[0] = '\0';
	while (std::fgets(buffer, sizeof(buffer), cmdFile))
		stdOut.append(buffer);

	exitCode = pclose(cmdFile);
	cmdFile = NULL;

	return true;
}

bool ShellInterface::RedirectTTY()
{
	return false;
}

bool ShellInterface::StartInteractive(const std::string& command)
{
//std::cout << "execi " << command << std::endl;
	assert(!cmdFile);
	cmdFile = popen(command.c_str(), "r");
	if (!cmdFile)
		return false;
	return true;
}

std::string ShellInterface::ExecuteInteractive(const std::string& command,
	const RedirectFlags& f)
{
	if (!cmdFile)
	{
		assert(!command.empty());
		if (!StartInteractive((command + BuildRedirectString(f))))
		{
			std::cerr << "Failed to start interactive shell" << std::endl;
			return "";
		}
	}
	else
		std::fprintf(cmdFile, (command + "\n").c_str());

	// TODO:  If TTY redirected and asking for PW, we won't have an EOL, so we'll get stuck in the loop below

	char buffer[2048];
	buffer[0] = '\0';
	std::string stdOut;
	while (std::fgets(buffer, sizeof(buffer), cmdFile))
		stdOut.append(buffer);
	return stdOut;
}

int ShellInterface::StopInteractive()
{
	assert(cmdFile);
	exitCode = pclose(cmdFile);
	cmdFile = NULL;
	return exitCode;
}

std::string ShellInterface::BuildRedirectString(const RedirectFlags& f) const
{
	if (f == RedirectErrToOut)
		return " " + stderrToStdout;
	else if (f == RedirectOutToNull)
		return " " + stdoutToNullFile;
	else if (f == RedirectErrToNull)
		return " " + stderrToNullFile;
	else if (f == RedirectAllToNull)
		return " " + allToNullFile;

	assert(f == RedirectNone && "unknown combination of redirect flags");
	return "";
}
