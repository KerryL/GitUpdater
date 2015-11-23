// File:  sharedMemory.cpp
// Date:  11/19/2015
// Auth:  K. Loux
// Desc:  Object wrapping inter-process shared memory functions.

// Standard C++ headers
#include <cassert>

// Local headers
#include "sharedMemory.h"

SharedMemory::SharedMemory(const unsigned int& bufferSize) : bufferSize(bufferSize)
{
	mapFile = NULL;
}

SharedMemory::~SharedMemory()
{
	unsigned int i;
	for (i = 0; i < sharedObjects.size(); i++)
		UnshareObject(sharedObjects[i]);
	CloseHandle(mapFile);
}

bool SharedMemory::Initialize(const std::string& name)
{
	assert(!mapFile);
	mapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0, bufferSize, name.c_str());
	return mapFile != NULL;
}

void* SharedMemory::ShareObject(void* address, const unsigned int& offset)
{
	void *buffer = MapViewOfFile(mapFile, FILE_MAP_ALL_ACCESS, 0, offset, bufferSize);
	if (buffer)
		sharedObjects.push_back(buffer);
	return buffer;
}

void SharedMemory::UnshareObject(void* address)
{
	UnmapViewOfFile(address);
}

void SharedMemory::UpdateObjectState(void* address, void* data, size_t size)
{
	CopyMemory(address, data, size);
}