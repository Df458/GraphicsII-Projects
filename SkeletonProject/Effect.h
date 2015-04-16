/************************************************************************
							Effect.h
Author: Johnathan O'Malia © 2015
Description:	Effect Resource Container class.
Stores texture and provides data necessary for resource manager

TODO:
Test
/************************************************************************/
//Parent Class
#include "Resource.h"

//DirectX 9 Headers
#include <d3d9.h>
#include <d3dx9.h>

#ifndef EFFECT_H_
#define EFFECT_H_

class Effect : public Resource
{
	friend class ResourceManager;
public:
	Effect(){ mType = ResourceType::EFFECT; mEffectData = nullptr; };
	Effect(ID3DXEffect* pData, UniqueID pID, std::string pName){ mType = ResourceType::EFFECT; mEffectData = pData;  mID = pID; mName = pName; };

	void* GetData(){ return mEffectData; };
	void OnLostDevice(){ mEffectData->OnLostDevice(); };
	void OnResetDevice(){ mEffectData->OnResetDevice();	};

	~Effect(){ Release(); };
private:

	void Release(){ mEffectData->Release(); mEffectData = NULL; };

	ID3DXEffect* mEffectData;
};


#endif // !EFFECT_H_