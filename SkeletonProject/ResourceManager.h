/************************************************************************
							ResourceManager.h
Author: Johnathan O'Malia © 2015
Description: File and resource manager

TODO:
Debug Error Logging for resources not found
Make defaults and error defaults data driven
Add proper default and error resource usage.

Custom Resource Management
Model Loading
Proper material handling
Implement fast unsigned integer key hash table. 

/************************************************************************/

#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

//C++ Headers
#include <string>

//Windows
#include <windows.h>

//Hashing
#include "CRC32.h"
#include "HashTable.h"

//DirectX 9 Headers
#include <d3d9.h>
#include <d3dx9.h>

//Custom Data Headers
#include "Resource.h"
#include "Texture.h"
#include "CubeTexture.h"
#include "Effect.h"
#include "Model.h"
#include "Materials/BaseMaterial.h"
#include "Utils.h"					//Used for Hugues ASCII data loader 

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void Initalize();
	void Terminate();

	//Resource Loaders
	UniqueID				LoadResource(std::string filename, ResourceType resourceType, bool Preserve = false, bool count = true);

	//Added for people who don't like LoadResource
	UniqueID				LoadTextureResource(std::string filename, bool Preserve = false, bool count = true);
	UniqueID				LoadCubeTextureResource(std::string filename, bool Preserve = false, bool count = true);
	UniqueID				LoadEffectResource(std::string filename, bool Preserve = false, bool count = true);
	UniqueID				LoadMeshResource(std::string filename, bool Preserve = false, bool count = true);

	//Resource Getters
	Resource*				GetResource(UniqueID pID, ResourceType resourceType);

	//Added for people who don't like GetResource
	Texture*				GetTexture(UniqueID pID);
	CubeTexture*			GetCubeTexture(UniqueID pID);
	Effect*					GetEffect(UniqueID pID);
	Model*					GetMesh(UniqueID pID);

	//Provide String lookup for Release mode
#ifndef _DEBUG
	Resource*				GetResource(std::string filename, ResourceType resourceType);

	//Added for people who don't like GetResource
	Texture*				GetTexture(std::string filename);
	CubeTexture*			GetCubeTexture(std::string filename);
	Effect*					GetEffect(std::string filename);
	Model*					GetMesh(std::string filename);
#endif // !_DEBUG

	//Resource Unloaders
	void					UnloadResource(UniqueID pID, ResourceType resourceType, bool Preserve = false, bool count = true);

	//Added for people who don't like UnloadResource
	void					UnloadTexture(UniqueID pID, bool Preserve = false, bool count = true);
	void					UnloadCubeTexture(UniqueID pID, bool Preserve = false, bool count = true);
	void					UnloadEffect(UniqueID pID, bool Preserve = false, bool count = true);
	void					UnloadMesh(UniqueID pID, bool Preserve = false, bool count = true);

	//Provide String Unloading for Release mode
#ifndef _DEBUG
	void					UnloadResource(std::string filename, ResourceType resourceType, bool Preserve = false, bool count = true);

	//Added for people who don't like UnloadResource
	void					UnloadTexture(std::string filename, bool Preserve = false, bool count = true);
	void					UnloadCubeTexture(std::string filename, bool Preserve = false, bool count = true);
	void					UnloadEffect(std::string filename, bool Preserve = false, bool count = true);
	void					UnloadMesh(std::string filename, bool Preserve = false, bool count = true);
#endif // !_DEBUG

	void					OnLostDevice();
	void					OnResetDevice();

	//Custom Resource Management
	char*					loadSceneData(std::string name);//Calls Hugues ASCII data loader 
	std::string				getSceneDataFilePath();;

	//Default getters
	Effect*					getDefaultEffect();;
	Effect*					getDefaultSkyEffect();;
	Texture*				getDefaultTexture();; 
	CubeTexture*			getDefaultSkyTexture();;

private:
	//Resources
	HashTable<UniqueID, Texture*> textureMap;
	HashTable<UniqueID, CubeTexture*> cubeTextureMap;
	HashTable<UniqueID, Effect*> effectMap;
	HashTable<UniqueID, Model*> meshMap;
	
	//Default ObjectsID
	UniqueID				m_DefaultEffectID;
	UniqueID				m_MissingTextureID;
	UniqueID				m_DefaultTextureID;
	UniqueID				m_DefaultCubeTextureID;
	UniqueID				m_DefaultSkyEffectID;

	//Resource Management
	bool					LoadTextureData(std::string TextureName);
	bool					LoadCubeTextureData(std::string TextureName);
	bool					LoadEffectData(std::string EffectName);
	bool					LoadModelData(std::string ModelName);


	//File Paths
	std::string				DataRootFilePath;
	std::string				EffectFilePath;
	std::string				TextureFilePath;
	std::string				ModelFilePath;
	std::string				SceneFilePath;

	//Private Utilities
	void					LocateDataFolders();
	bool					DirectoryExists(const std::string& dirName);
};

extern ResourceManager* gResourceManager;
#endif // !RESOURCE_MANAGER_H_
