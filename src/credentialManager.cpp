// File:  credentialManager.cpp
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Manages sets of login credentials.

// Local headers
#include "credentialManager.h"

std::string CredentialManager::GetCredentials(
	const std::string& requestString) const
{
	StringMap::const_iterator it;
	it = credentials.find(requestString);
	if (it == credentials.end())
		return "";

	return it->second;
}

void CredentialManager::AddCredentials(const std::string& requestString,
	const std::string& password)
{
	credentials[requestString] = password;
}