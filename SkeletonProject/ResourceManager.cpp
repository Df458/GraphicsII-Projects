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

	bool result = true;
	       
	result = LoadEffectData("Lighting.fx");
	if (!result)
	{
		std::string msg = "Cannot load default effect";
		MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}
	else
		m_DefaultEffectID = UID("Lighting.fx");

	result = LoadEffectData("skybox.fx");
	if (!result)
	{
		std::string msg = "Cannot load default skybox effect";
		MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}
	else
		m_DefaultSkyEffectID = UID("skybox.fx");

	result = LoadTextureData("MissingTexture.png");
	if (!result)
	{
		std::string msg = "Cannot load missing Texture";
		MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}
	else
		m_MissingTextureID = UID("MissingTexture.png");

	result = LoadTextureData("DefaultTexture.png");
	if (!result)
	{
		std::string msg = "Cannot load default Texture";
		MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}
	else
		m_DefaultTextureID = UID("DefaultTexture.png");

	result = LoadCubeTextureData("defaultSky.dds");
	if (!result)
	{
		std::string msg = "Cannot load cube map";
		MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}
	else
		m_DefaultCubeTextureID = UID("defaultSky.dds");

}

void ResourceManager::Terminate()
{
	textureMap.Clear();
	cubeTextureMap.Clear();
	effectMap.Clear();
}

void ResourceManager::OnLostDevice()
{
	for (auto& kv : effectMap.getMap())
		kv.second->OnLostDevice();
}

void ResourceManager::OnResetDevice()
{
	for (auto& kv : effectMap.getMap())
		kv.second->OnResetDevice();
}

UniqueID ResourceManager::LoadResource(std::string filename, ResourceType resourceType, bool Preserve, bool count)
{
	switch (resourceType)
	{
	case TEXTURE:
		return LoadTextureResource(filename, Preserve, count);
		break;
	case CUBETEXTURE:
		return LoadCubeTextureResource(filename, Preserve, count);
		break;
	case EFFECT:
		return LoadEffectResource(filename, Preserve, count);
		break;	
	case MODEL:
		return LoadMeshResource(filename, Preserve, count);
		break;
	}
	std::string msg = "Unrecognized resource type! " + resourceType;
	MessageBox(NULL, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
	PostQuitMessage(0);
	return NULL;
}

UniqueID ResourceManager::LoadTextureResource(std::string filename, bool Preserve, bool count)
{
	UniqueID uid = internString(filename);
	Texture* tmpTexture = nullptr;
	//Check to see it texture is loaded and get it if so.
	tmpTexture = textureMap.GetValue(uid);

	//if it does exist
	if (tmpTexture != nullptr) 
	{
		if (count)
			tmpTexture->instanceCount++;
		if (Preserve)
		{
			tmpTexture->preserveCount++;
		}
		return uid;
	}
	else //if not
	{
		if (LoadTextureData(filename))
		{
			return uid;
		}
		else
		{
			//TODO: log texture load failure error;
			return m_MissingTextureID;
		}
	}
	//TODO: log missing texture error;
	return m_MissingTextureID;
}

UniqueID ResourceManager::LoadCubeTextureResource(std::string filename, bool Preserve, bool count)
{
	UniqueID uid = internString(filename);
	CubeTexture* tmpTexture = nullptr;
	//Check to see it texture is loaded and get it if so.
	tmpTexture = cubeTextureMap.GetValue(uid);

	//if it does exist
	if (tmpTexture != nullptr)
	{
		if (count)
			tmpTexture->instanceCount++;
		if (Preserve)
		{
			tmpTexture->preserveCount++;
		}
		return uid;
	}
	else //if not
	{
		if (LoadCubeTextureData(filename))
		{
			return uid;
		}
		else
		{
			//TODO: log texture load failure error;
			return m_DefaultCubeTextureID;
		}
	}
	//TODO: log missing texture error;
	return m_DefaultCubeTextureID;
}

UniqueID ResourceManager::LoadEffectResource(std::string filename, bool Preserve, bool count)
{
	UniqueID uid = internString(filename);
	Effect* tmpEffect = nullptr;
	//Check to see it texture is loaded and get it if so.
	tmpEffect = effectMap.GetValue(uid);

	//if it does exist
	if (tmpEffect != nullptr)
	{
		if (count)
			tmpEffect->instanceCount++;
		if (Preserve)
		{
			tmpEffect->preserveCount++;
		}
		return uid;
	}
	else //if not
	{
		if (LoadEffectData(filename))
		{
			return uid;
		}
		else
		{
			//TODO: log effect load failure error;
			return m_DefaultEffectID;
		}
	}
	//TODO: log effect texture error;
	return m_DefaultEffectID;
}

UniqueID ResourceManager::LoadMeshResource(std::string filename, bool Preserve, bool count)
{
	UniqueID uid = internString(filename);
	Model* tmpMesh = nullptr;

	tmpMesh = meshMap.GetValue(uid);

	if (tmpMesh != nullptr)
	{
		if(count)
			tmpMesh->instanceCount++;
		if (Preserve)
		{
			tmpMesh->preserveCount++;
		}
		return uid;
	}
	else //if not
	{
		if (LoadModelData(filename))
		{
			return uid;
		}
		else
		{
			//TODO: log effect load failure error;
			return NULL;
		}
	}
	//TODO: log effect texture error;
	return NULL;
}

Resource* ResourceManager::GetResource(UniqueID pID, ResourceType resourceType)
{
	switch (resourceType)
	{
	case TEXTURE:
		return GetTexture(pID);
		break;
	case CUBETEXTURE:
		return GetCubeTexture(pID);
		break;
	case EFFECT:
		return GetEffect(pID);
		break;
	}
	return nullptr;
}

Texture* ResourceManager::GetTexture(UniqueID pID)
{
	return textureMap.GetValue(pID);
}

CubeTexture* ResourceManager::GetCubeTexture(UniqueID pID)
{
	return cubeTextureMap.GetValue(pID);
}

Effect* ResourceManager::GetEffect(UniqueID pID)
{
	return effectMap.GetValue(pID);
}

Model* ResourceManager::GetMesh(UniqueID pID)
{
	return meshMap.GetValue(pID);
}

//Release String Accessors
#ifndef _DEBUG

Resource* ResourceManager::GetResource(std::string filename, ResourceType resourceType)
{
	switch (resourceType)
	{
	case TEXTURE:
		return GetTexture(filename);
		break;
	case CUBETEXTURE:
		return GetCubeTexture(filename);
		break;
	case EFFECT:
		return GetEffect(filename);
		break;
	}
	return nullptr;
}

Texture* ResourceManager::GetTexture(std::string filename)
{
	return textureMap.GetValue(internString(filename));
}

CubeTexture* ResourceManager::GetCubeTexture(std::string filename)
{
	return cubeTextureMap.GetValue(internString(filename));
}

Effect* ResourceManager::GetEffect(std::string filename)
{
	return effectMap.GetValue(internString(filename));
}

Model* ResourceManager::GetMesh(std::string filename)
{
	return meshMap.GetValue(internString(filename));
}

#endif

bool ResourceManager::LoadTextureData(std::string TextureName)
{
	HRESULT result = S_OK;
	IDirect3DTexture9* pTexture;
	result = D3DXCreateTextureFromFile(gd3dDevice, (TextureFilePath + TextureName).c_str(), &pTexture);
	if (FAILED(result))
	{
		MessageBox(NULL, ("Error loading Texture " + TextureName).c_str(), "Error", MB_OK | MB_ICONERROR);
		return false;
	}	
	UniqueID textID = internString(TextureName);
	textureMap.Insert(textID, new Texture(pTexture, textID, TextureName));
	return true;
}

bool ResourceManager::LoadCubeTextureData(std::string TextureName)
{
	HRESULT result = S_OK;
	IDirect3DCubeTexture9* pTexture;
	result = D3DXCreateCubeTextureFromFile(gd3dDevice, (TextureFilePath + "Skyboxes//" + TextureName).c_str(), &pTexture);
	if (FAILED(result))
	{
		MessageBox(NULL, ("Error loading Cube Texture " + TextureName).c_str(), "Error", MB_OK | MB_ICONERROR);
		return false;
	}	
	UniqueID cubeID = internString(TextureName);
	cubeTextureMap.Insert(cubeID, new CubeTexture(pTexture, cubeID, TextureName));
	return true;
}

bool ResourceManager::LoadEffectData(std::string EffectName)
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
		MessageBox(NULL, errlog, "Shader Error", MB_OK | MB_ICONERROR);
		return false;
	}
	if (error_buf)
	{
		fprintf(stderr, "Errors:\n%s\n", (char*)error_buf->GetBufferPointer());
	}
	fprintf(stderr, "done.\n");

	UniqueID effectID = internString(EffectName);
	effectMap.Insert(effectID, new Effect(effect, effectID, EffectName));

	return true;
}


bool ResourceManager::LoadModelData(std::string ModelName)
{
	HRESULT result = S_OK;

	LPD3DXBUFFER materialBuffer;
	DWORD numMaterials;
	LPD3DXMESH mesh;

	result = D3DXLoadMeshFromX((ModelFilePath + ModelName).c_str(), D3DXMESH_SYSTEMMEM,
		gd3dDevice, NULL,
		&materialBuffer, NULL, &numMaterials,
		&mesh);

	if (FAILED(result))
	{
		MessageBox(NULL, ("Error loading Model " + ModelName).c_str(), "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	UniqueID modelID = internString(ModelName);
	meshMap.Insert(modelID, new Model(mesh, modelID, ModelName));
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

void ResourceManager::UnloadResource(UniqueID pID, ResourceType resourceType, bool Preserve, bool count)
{
	switch (resourceType)
	{
	case TEXTURE:
		UnloadTexture(pID, Preserve, count);
		break;
	case CUBETEXTURE:
		UnloadCubeTexture(pID, Preserve, count);
		break;
	case EFFECT:
		UnloadEffect(pID, Preserve, count);
		break;
	default:
		break;
		//improper type.
	}
}

void ResourceManager::UnloadTexture(UniqueID pID, bool Preserve, bool count)
{
	Texture* tmp = textureMap.GetValue(pID);
	if (tmp!= nullptr)
	{
		if (Preserve)
			tmp->preserveCount--;
		if (count)
			tmp->instanceCount--;
		if (tmp->instanceCount <= 0 && tmp->preserveCount <=0)
			textureMap.Remove(pID);
	}
	else
	{
		//TODO: Log Resource not found Error Here.
	}
}

void ResourceManager::UnloadCubeTexture(UniqueID pID, bool Preserve, bool count)
{
	CubeTexture* tmp = cubeTextureMap.GetValue(pID);
	if (tmp != nullptr)
	{
		if (Preserve)
			tmp->preserveCount--;
		if (count)
			tmp->instanceCount--;
		if (tmp->instanceCount <= 0 && tmp->preserveCount <= 0)
			cubeTextureMap.Remove(pID);
	}
	else
	{
		//TODO: Log Resource not found Error Here.
	}
}

void ResourceManager::UnloadEffect(UniqueID pID, bool Preserve, bool count)
{
	Effect* tmp = effectMap.GetValue(pID);
	if (tmp != nullptr)
	{
		if (Preserve)
			tmp->preserveCount--;
		if (count)
			tmp->instanceCount--;
		if (tmp->instanceCount <= 0 && tmp->preserveCount <= 0)
			effectMap.Remove(pID);
	}
	else
	{
		//TODO: Log Resource not found Error Here.
	}
}

void ResourceManager::UnloadMesh(UniqueID pID, bool Preserve /*= false*/, bool count /*= true*/)
{
	Model* tmp = meshMap.GetValue(pID);
	if (tmp != nullptr)
	{
		if (Preserve)
			tmp->preserveCount--;
		if (count)
			tmp->instanceCount--;
		if (tmp->instanceCount <= 0 && tmp->preserveCount <= 0)
			meshMap.Remove(pID);
	}
	else
	{
		//TODO: Log Resource not found Error Here.
	}
}

#ifndef _DEBUG
void ResourceManager::UnloadResource(std::string TextureName, ResourceType resourceType, bool Preserve, bool count)
{
	UnloadResource(internString(TextureName), resourceType, Preserve, count);
}

void ResourceManager::UnloadTexture(std::string TextureName, bool Preserve, bool count)
{
	UnloadTexture(internString(TextureName), Preserve, count);
}

void ResourceManager::UnloadCubeTexture(std::string TextureName, bool Preserve, bool count)
{
	UnloadCubeTexture(internString(TextureName), Preserve, count);
}

void ResourceManager::UnloadEffect(std::string EffectName, bool Preserve, bool count)
{
	UnloadEffect(internString(EffectName),Preserve, count);
}

void ResourceManager::UnloadMesh(std::string filename, bool Preserve /*= false*/, bool count /*= true*/)
{
	UnloadMesh(internString(filename),Preserve, count);
}

#endif

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

CubeTexture* ResourceManager::getDefaultSkyTexture()
{
	return GetCubeTexture(m_DefaultCubeTextureID);
}

Texture* ResourceManager::getDefaultTexture()
{
	return GetTexture(m_DefaultTextureID);
}

Effect* ResourceManager::getDefaultSkyEffect()
{
	return GetEffect(m_DefaultSkyEffectID);
}

Effect* ResourceManager::getDefaultEffect()
{
	return GetEffect(m_DefaultEffectID);
}

std::string ResourceManager::getSceneDataFilePath()
{
	return SceneFilePath;
}
