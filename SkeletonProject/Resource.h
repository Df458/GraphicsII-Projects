/************************************************************************
							Resource.h
Author: Johnathan O'Malia © 2015
Description: Base class for common functionality for resource types

TODO:
everything

/************************************************************************/
#ifndef RESOURCE_H_
#define RESOURCE_H_

#ifdef _DEBUG
#include <string>
typedef std::string UniqueID;
#else
typedef unsigned int UniqueID;
#endif // _DEBUG

enum ResourceType
{
	ERR = 0,
	TEXTURE,
	CUBETEXTURE,
	EFFECT,
};

class ResourceManager;
class Resource
{
	friend ResourceManager;
public:
	ResourceType GetType(){ return mType; };
	UniqueID GetID() { return mID; };
	std::string GetName() { return mName; };
	virtual void* GetData() = 0;

protected:
	virtual void Release() = 0;
	

	ResourceType mType;
	UniqueID mID;
	std::string mName;
	int instanceCount;
	int preserveCount;

};

#endif // !RESOURCE_H_
