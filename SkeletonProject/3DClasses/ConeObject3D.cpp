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

void ConeObject3D::buildIndexBuffer(IDirect3DDevice9* gd3dDevice) 
{
	// Obtain a pointer to a new index buffer.
	HR(gd3dDevice->CreateIndexBuffer(m_IndexCount * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	WORD* k = 0;
	HR(m_IndexBuffer->Lock(0, 0, (void**)&k, 0));
	//Generate base circle
	for (unsigned i = 0; i < m_RadialSegments; i++)
	{
		k[i * 3] = 0;
		k[i * 3 + 1] = m_RadialSegments - i;
		k[i * 3 + 2] = m_RadialSegments - (i + 1);
		//getting last base triangle
		if (k[i * 3 + 2] == 0)
		{
			k[i * 3 + 2] = m_RadialSegments;
		}
	}

	//Generate vertical segments
	for (unsigned i = m_RadialSegments; i < m_TriCount; i++)
	{
		k[i * 3] = m_VertexCount - 1;
		k[i * 3 + 1] = m_RadialSegments - (i + 1 - m_RadialSegments) ;
		k[i * 3 + 2] = m_RadialSegments - (i - m_RadialSegments);
		//getting last cone triangle
		if (k[i * 3 + 1] == 0)
		{
			k[i * 3 + 1] = m_RadialSegments;
		}		
	}

	HR(m_IndexBuffer->Unlock());
}
