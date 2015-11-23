// File:  sharedMemory.h
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Object wrapping inter-process shared memory functions.

#ifndef SHARED_MEMORY_H_
#define SHARED_MEMORY_H_

// Standard C++ headers
#include <string>
#include <vector>

// Windows headers
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class SharedMemory
{
public:
	SharedMemory(const unsigned int& bufferSize);
	~SharedMemory();

	bool Initialize(const std::string& name);
	void* ShareObject(void* address, const unsigned int& offset = 0);
	void UpdateObjectState(void* address, void* data, size_t size);

	void* GetObject(const unsigned int& i) const { return sharedObjects[i]; }

private:
	const unsigned int bufferSize;
	HANDLE mapFile;

	std::vector<void*> sharedObjects;

	void UnshareObject(void* address);
};

#endif