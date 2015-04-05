#include "ResourceManager.h"
#include "d3dApp.h"

#include <sstream>

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::Initalize()
{
	LocateDataFolders();

	//TODO::Set up default error things
	m_DefaultEffectID = GetEffectID("Lighting.fx");
}

void ResourceManager::Terminate()
{
	//Release Textures
	for (auto& kv : textureMap)
	{
		kv.second->Release();
		kv.second = NULL;
	}

	//Release Effects
	for (auto& kv : effectMap)
	{
		kv.second->Release();
		kv.second = NULL;
	}

	//Release Meshes
	for (auto& kv : meshMap)
	{
		kv.second->Release();
		kv.second = NULL;
	}
}

unsigned int ResourceManager::GetTextureID(std::string TextureName)
{
	unsigned int hashID = crc32(TextureName);
	//If texture is loaded
	if (TextureExists(hashID))
		return hashID;
	else //try to load texture
	{
		//if it can be loaded
		if (LoadTexture(TextureName))
		{
			return hashID;
		}
		else
		{
			//TODO: RETURN DEFAULT ERROR TEXTURE AND POST ERROR LOG AND DEBUG CONSOLE
			std::string msg = "Cannot find Texture file: " + TextureName;
			MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			return 0;
		}
	}
	return 0; //change to default error texture;
}

unsigned int ResourceManager::GetCubeTextureID(std::string TextureName)
{
	unsigned int hashID = crc32(TextureName);
	//If texture is loaded
	if (CubeTextureExists(hashID))
		return hashID;
	else //try to load texture
	{
		//if it can be loaded
		if (LoadCubeTexture(TextureName))
		{
			return hashID;
		}
		else
		{
			//TODO: RETURN DEFAULT ERROR TEXTURE AND POST ERROR LOG AND DEBUG CONSOLE
			std::string msg = "Cannot find Cube Texture file: " + TextureName;
			MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			return 0;
		}
	}
	return 0; //change to default error texture;
}

unsigned int ResourceManager::GetModelID(std::string ModelName)
{
	unsigned int hashID = crc32(ModelName);
	//If effect is loaded
	if (ModelExists(hashID))
		return hashID;
	else //try to load model
	{
		//if it can be loaded
		if (LoadMesh(ModelName))
		{
			return hashID;
		}
		else
		{
			//TODO: RETURN DEFAULT ERROR MODEL AND POST ERROR LOG AND DEBUG CONSOLE
			std::string msg = "Cannot find Model File: " + ModelName;
			MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			return 0;
		}
	}
	return 0; //change to default error model;
}

unsigned int ResourceManager::GetEffectID(std::string EffectName)
{
	unsigned int hashID = crc32(EffectName);
	//If effect is loaded
	if (EffectExists(hashID))
		return hashID;
	else //try to load effect
	{
		//if it can be loaded
		if (LoadEffect(EffectName))
		{
			return hashID;
		}
		else
		{
			//TODO: RETURN DEFAULT ERROR EFFECT AND POST ERROR LOG AND DEBUG CONSOLE
			std::string msg = "Cannot find Effect File: " + EffectFilePath + EffectName;
			MessageBox(NULL, msg.c_str() , "Error", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			return 0;
		}
	}
	return 0; //change to default error effect;
}

IDirect3DTexture9* ResourceManager::GetTexture(std::string TextureName)
{
	return textureMap.at(GetTextureID(TextureName));
}

IDirect3DTexture9* ResourceManager::GetTexture(unsigned int TextureID)
{
	//If texture is loaded
	if (TextureExists(TextureID))
		return textureMap.at(TextureID);
	else 
	{
		//TODO: RETURN DEFAULT ERROR TEXTURE AND POST ERROR LOG AND DEBUG CONSOLE
		std::string msg = "Cannot find Texture file with ID : " + std::to_string(TextureID);
		MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
		return nullptr;
	}
	return nullptr; //change to default error texture;
}

IDirect3DCubeTexture9* ResourceManager::GetCubeTexture(std::string TextureName)
{
	return cubeTextureMap.at(GetCubeTextureID(TextureName));
}

IDirect3DCubeTexture9* ResourceManager::GetCubeTexture(unsigned int TextureID)
{
	//If texture is loaded
	if (CubeTextureExists(TextureID))
		return cubeTextureMap.at(TextureID);
	else 
	{
		//TODO: RETURN DEFAULT ERROR TEXTURE AND POST ERROR LOG AND DEBUG CONSOLE
		std::string msg = "Cannot find Cube Texture file with ID : " + std::to_string(TextureID);
		MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
		return nullptr;
	}
	return nullptr; //change to default error texture;
}

ID3DXEffect* ResourceManager::GetEffect(std::string  EffectName)
{
	return effectMap.at(GetEffectID(EffectName));
}

ID3DXEffect* ResourceManager::GetEffect(unsigned int EffectID)
{
	//If texture is loaded
	if (EffectExists(EffectID))
		return effectMap.at(EffectID);
	else
	{
		//TODO: RETURN DEFAULT ERROR EFFECT AND POST ERROR LOG AND DEBUG CONSOLE
		std::string msg = "Cannot find Effect file with ID : " + std::to_string(EffectID);
		MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
		return nullptr;
	}
	return nullptr; //change to default error effect;
}

LPD3DXMESH ResourceManager::GetMesh(std::string MeshName)
{
	return meshMap.at(GetModelID(MeshName));
}

LPD3DXMESH ResourceManager::GetMesh(unsigned int MeshID)
{
	//If mesh is loaded
	if (ModelExists(MeshID))
		return meshMap.at(MeshID);
	else
	{
		//TODO: RETURN DEFAULT ERROR MODEL AND POST ERROR LOG AND DEBUG CONSOLE
		std::string msg = "Cannot find Model file with ID : " + std::to_string(MeshID);
		MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
		return nullptr;
	}
	return nullptr; //change to default error effect;
}

bool ResourceManager::LoadTexture(std::string TextureName)
{
	HRESULT result = S_OK;
	IDirect3DTexture9* pTexture;
	result = D3DXCreateTextureFromFile(gd3dDevice, (TextureFilePath + TextureName).c_str(), &pTexture);
	if (FAILED(result))
	{
		MessageBox(NULL, ("Error loading Texture " + TextureName + "Add Defaults Later.").c_str(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);

		return false;
	}	
	textureMap.insert(std::pair<unsigned int, IDirect3DTexture9*>(crc32(TextureName), pTexture));
	return true;
}

bool ResourceManager::LoadCubeTexture(std::string TextureName)
{
	HRESULT result = S_OK;
	IDirect3DCubeTexture9* pTexture;
	result = D3DXCreateCubeTextureFromFile(gd3dDevice, (TextureFilePath + TextureName).c_str(), &pTexture);
	if (FAILED(result))
	{
		MessageBox(NULL, ("Error loading Cube Texture " + TextureName + "Add Defaults Later.").c_str(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);

		return false;
	}	
    cubeTextureMap.insert(std::pair<unsigned int, IDirect3DCubeTexture9*>(crc32(TextureName), pTexture));
	return true;
}

bool ResourceManager::LoadEffect(std::string EffectName)
{
	HRESULT result = S_OK;
	ID3DXEffect* effect;

	LPD3DXBUFFER error_buf = 0;
	fprintf(stderr, "Compiling shader...\n");
	std::string effectPath = EffectFilePath + EffectName;

	result = D3DXCreateEffectFromFile(gd3dDevice, effectPath.c_str(), NULL, NULL, 0, NULL, &effect, &error_buf);
	if (FAILED(result))
	{
		char* errlog = (char*)error_buf->GetBufferPointer();
		fprintf(stderr, "Errors:\n%s\n", (char*)error_buf->GetBufferPointer());
		MessageBox(NULL, "Errors:", errlog, MB_OK | MB_ICONERROR);
		return false;
	}
	if (error_buf)
	{
		fprintf(stderr, "Errors:\n%s\n", (char*)error_buf->GetBufferPointer());
	}
	fprintf(stderr, "done.\n");

	effectMap.insert(std::pair<unsigned int, ID3DXEffect*>(crc32(EffectName), effect));

	return true;
}

bool ResourceManager::LoadMesh(std::string MeshName)
{
	//TODO: MESH LOADING
	return false;
}

char* ResourceManager::loadSceneData(std::string SceneName)//Calls Hugues ASCII data loader 
{
	char* value = loadFileContents(std::string(SceneFilePath + SceneName).c_str());
	if (value == NULL)
	{
		std::string msg = "Cannot find Scene named " + SceneName;
		MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}
	return value;
}

bool ResourceManager::TextureExists(std::string TextureName)
{
	return TextureExists(crc32(TextureName));
}

bool ResourceManager::TextureExists(unsigned int TextureUID)
{
	if (textureMap.find(TextureUID) != textureMap.end())
	{
		return true;
	}
	return false;
}

bool ResourceManager::CubeTextureExists(std::string TextureName)
{
	return CubeTextureExists(crc32(TextureName));
}

bool ResourceManager::CubeTextureExists(unsigned int TextureUID)
{
	if (cubeTextureMap.find(TextureUID) != cubeTextureMap.end())
	{
		return true;
	}
	return false;
}


bool ResourceManager::ModelExists(std::string ModelName)
{
	return ModelExists(crc32(ModelName));
}

bool ResourceManager::ModelExists(unsigned int MeshID)
{
	if (meshMap.find(MeshID) != meshMap.end())
	{
		return true;
	}
	return false;
}

bool ResourceManager::EffectExists(std::string EffectName)
{
	return EffectExists(crc32(EffectName));
}

bool ResourceManager::EffectExists(unsigned int EffectID)
{
	if (effectMap.find(EffectID) != effectMap.end())
	{
		return true;
	}
	return false;
}

void ResourceManager::UnloadTexture(unsigned int TextureUID)
{
	if (TextureExists(TextureUID))
	{
		textureMap.at(TextureUID)->Release();
		textureMap.at(TextureUID) = NULL;

		textureMap.erase(TextureUID);
	}
	else
	{
		//TODO: Log Resource not found Error Here.
	}
}

void ResourceManager::UnloadTexture(std::string TextureName)
{
	UnloadTexture(crc32(TextureName));
}

void ResourceManager::UnloadCubeTexture(unsigned int TextureUID)
{
	if (CubeTextureExists(TextureUID))
	{
		cubeTextureMap.at(TextureUID)->Release();
		cubeTextureMap.at(TextureUID) = NULL;

		cubeTextureMap.erase(TextureUID);
	}
	else
	{
		//TODO: Log Resource not found Error Here.
	}
}

void ResourceManager::UnloadCubeTexture(std::string TextureName)
{
	UnloadCubeTexture(crc32(TextureName));
}

void ResourceManager::UnloadEffect(unsigned int EffectUID)
{
	if (EffectExists(EffectUID))
	{
		effectMap.at(EffectUID)->Release();
		effectMap.at(EffectUID) = NULL;

		effectMap.erase(EffectUID);
	}
	else
	{
		//TODO: Log Resource not found Error Here.
	}
}

void ResourceManager::UnloadEffect(std::string EffectName)
{
	UnloadEffect(crc32(EffectName));
}

void ResourceManager::UnloadMesh(unsigned int MeshUID)
{
	if (ModelExists(MeshUID))
	{
		meshMap.at(MeshUID)->Release();
		meshMap.at(MeshUID) = NULL;

		meshMap.erase(MeshUID);
	}
	else
	{
		//TODO: Log Resource not found Error Here.
	}
}

void ResourceManager::UnloadMesh(std::string MeshName)
{
	UnloadMesh(crc32(MeshName));
}

void ResourceManager::LocateDataFolders()
{
	//Find Data Path

	if (DirectoryExists("Data"))
	{
		DataRootFilePath = "Data\\";
	}
	else
	{
		if (DirectoryExists("..\\Data"))
		{
			DataRootFilePath = "..\\Data\\";
		}
		else
		{
			MessageBox(NULL, "Cannot find Data Folder", "Error", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
		}
	}

	//Confirm Sub Folders Exist
	if (!DirectoryExists(std::string(DataRootFilePath) + "Textures"))
	{
		MessageBox(NULL, "Cannot find Textures Folder", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}
	TextureFilePath = std::string(std::string(DataRootFilePath) + "Textures\\").c_str();

	if (!DirectoryExists(std::string(DataRootFilePath) + "Effects"))
	{
		MessageBox(NULL, "Cannot find Effects Folder", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}
	EffectFilePath = std::string(std::string(DataRootFilePath) + "Effects\\").c_str();

	if (!DirectoryExists(std::string(DataRootFilePath) + "Models"))
	{
		MessageBox(NULL, "Cannot find Models Folder", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}
	ModelFilePath = std::string(std::string(DataRootFilePath) + "Models\\").c_str();

	if (!DirectoryExists(std::string(DataRootFilePath) + "Scenes"))
	{
		MessageBox(NULL, "Cannot find Scenes Folder", "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}
	SceneFilePath = std::string(std::string(DataRootFilePath) + "Scenes\\").c_str();
}

bool ResourceManager::DirectoryExists(const std::string& dirName)
{
	DWORD ftyp = GetFileAttributes(dirName.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true; 

	return false;
}