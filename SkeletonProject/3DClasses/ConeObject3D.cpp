#include "ConeObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"


ConeObject3D::ConeObject3D(float radius, unsigned radialSegments, float height, BaseMaterial* mat, ID3DXEffect* effect)
	:MeshObject3D(mat, effect),m_Radius(radius), m_RadialSegments(radialSegments), m_Height(height)
{
	m_VertexCount = m_RadialSegments + 2;
	m_TriCount = m_RadialSegments * 2;
	m_IndexCount = m_TriCount * 3;
}

void ConeObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice) 
{
    HR(D3DXCreateCylinder(gd3dDevice, m_Radius, 0, m_Height, m_RadialSegments, 1, &m_Mesh, NULL));
}
