/************************************************************************
							CubeTexture.h
Author: Johnathan O'Malia © 2015
Description:	CubeTexture Resource Container class.
				Stores texture and provides data necessary for resource manager

TODO:
/************************************************************************/
//Parent Class
#include "Resource.h"

//DirectX 9 Headers
#include <d3d9.h>
#include <d3dx9.h>

#ifndef CUBE_TEXTURE_H_
#define CUBE_TEXTURE_H_

class CubeTexture : public Resource
{
	friend class ResourceManager;
public:
	CubeTexture(){ mType = ResourceType::CUBETEXTURE; mCubeTextureData = nullptr; };
	CubeTexture(IDirect3DCubeTexture9* pData, UniqueID pID, std::string pName){ mType = ResourceType::CUBETEXTURE; mCubeTextureData = pData;  mID = pID; mName = pName; };

	void* GetData(){ return mCubeTextureData; };

	~CubeTexture(){ Release(); };
private:

	void Release(){ mCubeTextureData->Release(); mCubeTextureData = NULL; };

	IDirect3DCubeTexture9* mCubeTextureData;
};


#endif // !CUBE_TEXTURE_H_