/************************************************************************
							Texture.h
Author: Johnathan O'Malia © 2015
Description:	Texture Resource Container class.
				Stores texture and provides data necessary for resource manager

TODO:
Test
/************************************************************************/
//Parent Class
#include "Resource.h"

//DirectX 9 Headers
#include <d3d9.h>
#include <d3dx9.h>

#ifndef TEXTURE_H_
#define TEXTURE_H_

class Texture : public Resource
{
	friend class ResourceManager;
public:
	Texture(){ mType = ResourceType::TEXTURE; mTextureData = nullptr; };
	Texture(IDirect3DTexture9* pData, UniqueID pID, std::string pName){ mType = ResourceType::TEXTURE; mTextureData = pData;  mID = pID; mName = pName; };

	void* GetData(){ return mTextureData; };

	~Texture(){ Release(); };
private:

	void Release(){ mTextureData->Release(); mTextureData = NULL; };

	IDirect3DTexture9* mTextureData;
};


#endif // !TEXTURE_H_