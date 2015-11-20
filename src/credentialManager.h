// File:  credentialManager.h
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Manages sets of login credentials.

#ifndef CREDENTIAL_MANAGER_H_
#define CREDENTIAL_MANAGER_H_

// Standard C++ headers
#include <string>
#include <map>

class CredentialManager
{
public:
	std::string GetCredentials(const std::string& requestString) const;
	void AddCredentials(const std::string& requestString, const std::string& password);

private:
	typedef std::map<std::string, std::string> StringMap;
	StringMap credentials;
};

#endif// CREDENTIAL_MANAGER_H_