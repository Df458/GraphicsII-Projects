#include "CylinderObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"

#define _USE_MATH_DEFINES 
#include <math.h>


CylinderObject3D::CylinderObject3D(float radius, unsigned radialSegments, float height)
	:BaseObject3D(),m_Radius(radius), m_RadialSegments(radialSegments), m_Height(height)
{
	// Determine vertex count;
	m_VertexCount = (m_RadialSegments * 2) + 2;
	//Determine intex count;
	m_TriCount = m_RadialSegments * 4;
	m_IndexCount = m_TriCount * 3;
}

void CylinderObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	// Obtain a pointer to a new vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(m_VertexCount * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));


	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	v[0] = VertexPos(0.0f, 0.0f, 0.0f);
	v[1] = VertexPos(0,(float)m_Height,0);

	for (unsigned i = 0; i < m_RadialSegments; i++)
	{
		float angle = 2.0f * (float)M_PI * ((float)i / (float)m_RadialSegments);
		float x = sin(angle);
		float y = cos(angle);

		v[i+2] = VertexPos(x, 0.0f, y);
		v[m_RadialSegments+i+2] = VertexPos(x, (float)m_Height, y);
	}

	
	HR(m_VertexBuffer->Unlock());
}

void CylinderObject3D::buildIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	// Obtain a pointer to a new index buffer.
	HR(gd3dDevice->CreateIndexBuffer(m_IndexCount * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	WORD* k = 0;
	HR(m_IndexBuffer->Lock(0, 0, (void**)&k, 0));

	for (unsigned i = 0; i < m_RadialSegments; i++)
	{
        k[(i * 12)] = i + 2;
        k[(i * 12) + 1] = 0;
        k[(i * 12) + 2] = i + 3;

        k[(i * 12) + 3] = 1;
        k[(i * 12) + 4] = i + m_RadialSegments + 2;
        k[(i * 12) + 5] = i + m_RadialSegments + 3;

        k[(i * 12) + 6] = i + 2;
        k[(i * 12) + 7] = i + 3;
        k[(i * 12) + 8] = i + m_RadialSegments + 2;

        k[(i * 12) + 9] = i + m_RadialSegments + 3;
        k[(i * 12) + 10] = i + m_RadialSegments + 2;
        k[(i * 12) + 11] = i + 3;

        if( i == m_RadialSegments - 1) {
            k[(i * 12) + 1] = 2;
            k[(i * 12) + 5] = m_RadialSegments + 2;
            k[(i * 12) + 7] = 2;
            k[(i * 12) + 9] = m_RadialSegments + 2;
            k[(i * 12) + 11] = 2;
        }
	}

	HR(m_IndexBuffer->Unlock());
}
