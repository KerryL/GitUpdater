// File:  credentialManager.cpp
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Manages sets of login credentials.

// Local headers
#include "credentialManager.h"

const std::string CredentialManager::passwordRequestFlag("--pwRequest");
const std::string CredentialManager::memoryName("Global\\gitUpdaterSharedMemory");

CredentialManager::CredentialManager() : shMem(4096)
{
	shMem.Initialize(memoryName);
	shMem.ShareObject(this);
	shMem.UpdateObjectState(shMem.GetObject(0), this, sizeof(*this));
}

CredentialManager* CredentialManager::GetSharedManager() const
{
	return static_cast<CredentialManager*>(shMem.GetObject(0));
}

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
	shMem.UpdateObjectState(shMem.GetObject(0), this, sizeof(*this));
}

bool CredentialManager::IsPasswordRequestFlag(const std::string& s)
{
	return s.compare(passwordRequestFlag) == 0;
}
