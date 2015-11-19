// File:  credentialManager.h
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Manages sets of login credentials.

#ifndef CREDENTIAL_MANAGER_H_
#define CREDENTIAL_MANAGER_H_

// Standard C++ headers
#include <string>
#include <vector>

class CredentialManager
{
public:

private:
	struct Credentials
	{
		std::string userString;
		std::string passString;
	};

	std::vector<Credentials> credentials;
};

#endif// CREDENTIAL_MANAGER_H_