#include <d3dx9shape.h>
#include "CylinderObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"


CylinderObject3D::CylinderObject3D(float radius, unsigned radialSegments, float height)
	:MeshObject3D(), m_Height(height), m_Radius(radius), m_RadialSegments(radialSegments){
	// Determine vertex count;
	m_VertexCount = (m_RadialSegments * 2) + 2;
	//Determine intex count;
	m_TriCount = m_RadialSegments * 4;
	m_IndexCount = m_TriCount * 3;
}

void CylinderObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
    HR(D3DXCreateCylinder(gd3dDevice, m_Radius, m_Radius, m_Height, m_RadialSegments, 1, &m_Mesh, NULL));
}
