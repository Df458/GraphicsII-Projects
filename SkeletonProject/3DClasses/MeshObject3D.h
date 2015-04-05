#pragma once
#include "../3DClasses/BaseObject3D.h"

class BaseMaterial;
class LightSceneNode;

class MeshObject3D : public BaseObject3D {
public:
    MeshObject3D(BaseMaterial* mat, ID3DXEffect* effect = NULL);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection, LightSceneNode* light);
	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual BaseMaterial* getMaterial(void);

protected:
    LPD3DXMESH m_Mesh = 0;
};
