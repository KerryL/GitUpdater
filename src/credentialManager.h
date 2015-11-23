// File:  credentialManager.h
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Manages sets of login credentials.

#ifndef CREDENTIAL_MANAGER_H_
#define CREDENTIAL_MANAGER_H_

// Standard C++ headers
#include <string>
#include <map>

// Local headers
#include "sharedMemory.h"

class CredentialManager
{
public:
	CredentialManager();

	CredentialManager* GetSharedManager() const;

	std::string GetCredentials(const std::string& requestString) const;
	void AddCredentials(const std::string& requestString, const std::string& password);

	static bool IsPasswordRequestFlag(const std::string& s);

private:
	static const std::string passwordRequestFlag;
	typedef std::map<std::string, std::string> StringMap;
	StringMap credentials;

	static const std::string memoryName;
	SharedMemory shMem;
};

#endif// CREDENTIAL_MANAGER_H_