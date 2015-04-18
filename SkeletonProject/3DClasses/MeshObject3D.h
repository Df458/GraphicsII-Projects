#pragma once

#include <d3dx9.h>

#include "../d3dUtil.h"

class BaseMaterial;
class LightSceneNode;

class MeshObject3D {
public:
    MeshObject3D();
	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& world, D3DXMATRIX& cview, D3DXMATRIX& view, D3DXMATRIX& projection, LightSceneNode* light, IDirect3DCubeTexture9* cube, BaseMaterial* material);

protected:
    LPD3DXMESH m_Mesh = 0;
	D3DXVECTOR3 vpos;
	unsigned m_VertexCount;
	unsigned m_IndexCount;
	unsigned m_TriCount;

	virtual void buildMeshBuffers(IDirect3DDevice9* gd3dDevice) = 0;
	virtual void buildUVBuffer(IDirect3DDevice9* gd3dDevice) = 0;
};
