/************************************************************************
							ResourceManager.h
Author: Johnathan O'Malia © 2015
Description: File and resource manager tailored for DirectX 9

TODO:
Debug Error Logging for resources not found
Add Error defaults

Custom Resource Management
Model Loading
Proper material handling
Implement fast unsigned integer key hash table. 

/************************************************************************/

#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

//C++ Headers
#include <map>
#include <string>

//Windows
#include <windows.h>

//Hasher
#include "CRC32.h"

//DirectX 9 Headers
#include <d3d9.h>
#include <d3dx9.h>

//Custom Data Headers
#include "Materials/BaseMaterial.h"
#include "Utils.h"					//Used for Hugues ASCII data loader 

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void Initalize();
	void Terminate();

	//DirectX 9 Resource Management
	unsigned int		GetTextureID(std::string TextureName);
	unsigned int		GetCubeTextureID(std::string TextureName);
	unsigned int		GetModelID(std::string ModelName);
	unsigned int		GetEffectID(std::string EffectName);
	
	IDirect3DTexture9*	GetTexture(std::string TextureName);
	IDirect3DTexture9*	GetTexture(unsigned int TextureUID);
	IDirect3DCubeTexture9*	GetCubeTexture(std::string TextureName);
	IDirect3DCubeTexture9*	GetCubeTexture(unsigned int TextureUID);
	ID3DXEffect*		GetEffect(std::string  EffectName);
	ID3DXEffect*		GetEffect(unsigned int EffectUID);
	LPD3DXMESH			GetMesh(std::string MeshName);
	LPD3DXMESH			GetMesh(unsigned int  MeshUID);

	bool				TextureExists(std::string TextureName);
	bool				TextureExists(unsigned int TextureUID);
	bool				CubeTextureExists(std::string TextureName);
	bool				CubeTextureExists(unsigned int TextureUID);
	bool				ModelExists(std::string ModelName);
	bool				ModelExists(unsigned int MeshUID);
	bool				EffectExists(std::string EffectName);
	bool				EffectExists(unsigned int EffectUID);

	void				UnloadTexture(unsigned int TextureUID);
	void				UnloadTexture(std::string TextureName);
	void				UnloadCubeTexture(unsigned int TextureUID);
	void				UnloadCubeTexture(std::string TextureName);
	void				UnloadEffect(unsigned int EffectUID);
	void				UnloadEffect(std::string EffectName);
	void				UnloadMesh(unsigned int MeshUID);
	void				UnloadMesh(std::string MeshName);

	//Custom Resource Management
	char*				loadSceneData(std::string name);//Calls Hugues ASCII data loader 
	std::string			getSceneDataFilePath(){ return SceneFilePath; };

	//Default getters
	ID3DXEffect*		getDefaultEffect(){ return GetEffect(m_DefaultEffectID); };

private:
	//DirectX 9 Resources
	std::map<unsigned int, IDirect3DTexture9*> textureMap;
	std::map<unsigned int, IDirect3DCubeTexture9*> cubeTextureMap;
	std::map<unsigned int, ID3DXEffect*> effectMap;
	std::map<unsigned int, LPD3DXMESH> meshMap;
	
	//Default ObjectsID
	unsigned int m_DefaultEffectID;

	//DirectX 9 Resource Management
	bool LoadTexture(std::string TextureName);
	bool LoadCubeTexture(std::string TextureName);
	bool LoadEffect(std::string EffectName);
	bool LoadMesh(std::string MeshName);

	//File Paths
	std::string DataRootFilePath;
	std::string EffectFilePath;
	std::string TextureFilePath;
	std::string ModelFilePath;
	std::string SceneFilePath;

	//Private Utilities
	void LocateDataFolders();
	bool DirectoryExists(const std::string& dirName);

};

extern ResourceManager* gResourceManager;
#endif // !RESOURCE_MANAGER_H_
