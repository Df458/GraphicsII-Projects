#pragma once

#include <d3dx9.h>

class BaseMaterial;
class LightSceneNode;
class Scene;

class MeshObject3D {
public:
	MeshObject3D();
	MeshObject3D(LPD3DXMESH pMesh);
	virtual void Create();
	virtual void Render(D3DXMATRIX& world, D3DXMATRIX& cview, D3DXMATRIX& view, D3DXMATRIX& projection, BaseMaterial* material, Scene* scene);

protected:
    LPD3DXMESH m_Mesh = 0;
	D3DXVECTOR3 vpos;
	unsigned m_VertexCount;
	unsigned m_IndexCount;
	unsigned m_TriCount;

	virtual void buildMeshBuffers() = 0;
	virtual void buildUVBuffer() = 0;
};
