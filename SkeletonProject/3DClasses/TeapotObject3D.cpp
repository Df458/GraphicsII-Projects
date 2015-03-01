#include "TeapotObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"


TeapotObject3D::TeapotObject3D(BaseMaterial* mat, ID3DXEffect* effect)
	:MeshObject3D(mat, effect)
{
	m_VertexCount = 8;
	m_TriCount = 12;
	m_IndexCount = 36;
}

void TeapotObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice) 
{
    HR(D3DXCreateTeapot(gd3dDevice, &m_Mesh, NULL));
}
