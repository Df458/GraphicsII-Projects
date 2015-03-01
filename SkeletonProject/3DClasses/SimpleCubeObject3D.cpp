#include "SimpleCubeObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"


SimpleCubeObject3D::SimpleCubeObject3D(BaseMaterial* mat, float w, float h, float d, ID3DXEffect* effect)
	:MeshObject3D(mat, effect)
{
    m_Width = w;
    m_Height = h;
    m_Depth = d;

	m_VertexCount = 8;
	m_TriCount = 12;
	m_IndexCount = 36;
}

void SimpleCubeObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice) 
{
    HR(D3DXCreateBox(gd3dDevice, m_Width, m_Height, m_Depth,  &m_Mesh, NULL));
}

