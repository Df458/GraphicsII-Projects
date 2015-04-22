/************************************************************************
							Model.h
Author: Johnathan O'Malia © 2015
Description:	Model Resource Container class.
				Stores texture and provides data necessary for resource manager

TODO:
/************************************************************************/
//Parent Class
#include "Resource.h"

//DirectX 9 Headers
#include <d3d9.h>
#include <d3dx9.h>

#ifndef MODEL_H_
#define MODEL_H_

class Model : public Resource
{
	friend class ResourceManager;
public:
	Model(){ mType = ResourceType::MODEL; mModelData = nullptr; };
	Model(LPD3DXMESH pData, UniqueID pID, std::string pName){ mType = ResourceType::MODEL; mModelData = pData;  mID = pID; mName = pName; };

	void* GetData(){ return mModelData; };

	~Model(){ Release(); };
private:

	void Release(){ mModelData->Release(); mModelData = NULL; };

	LPD3DXMESH mModelData;
};


#endif // !MODEL_H_