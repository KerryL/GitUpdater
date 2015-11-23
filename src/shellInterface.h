// File:  shellInterface.h
// Date:  11/20/2015
// Auth:  K. Loux
// Desc:  Interface to system shell.

#ifndef SHELL_INTERFACE_H_
#define SHELL_INTERFACE_H_

// Standard C/C++ headers
#include <string>
#include <stdio.h>

class ShellInterface
{
public:
	ShellInterface();

	enum RedirectFlags
	{
		RedirectNone = 0,
		RedirectErrToOut = 1,
		RedirectOutToNull = 2,
		RedirectErrToNull = 4,
		RedirectAllToNull = 6
	};

	int ExecuteCommand(const std::string& command,
		const RedirectFlags& f = RedirectAllToNull);
	bool ExecuteCommand(const std::string& command, std::string& stdOut,
		const RedirectFlags& f = RedirectNone);

	int GetExitCode() const { return exitCode; }

private:
	static const std::string stderrToStdout;
	static const std::string stdoutToNullFile;
	static const std::string stderrToNullFile;
	static const std::string allToNullFile;

	FILE* cmdFile;
	int exitCode;

	std::string BuildRedirectString(const RedirectFlags& f) const;
};

#endif// SHELL_INTERFACE_H_