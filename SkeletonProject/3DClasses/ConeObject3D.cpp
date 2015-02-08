#include "ConeObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"
/*
#define _USE_MATH_DEFINES 
#include <math.h>*/

// Safety define
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif

ConeObject3D::ConeObject3D(float radius, unsigned radialSegments, float height)
	:BaseObject3D(),m_Radius(radius), m_RadialSegments(radialSegments), m_Height(height)
{
	m_VertexCount = m_RadialSegments + 2;
	m_TriCount = m_RadialSegments * 2;
	m_IndexCount = m_TriCount * 3;
}

void ConeObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice) 
{
	// Obtain a pointer to a new vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(m_VertexCount * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));


	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	//Bottom Center Vertex
	v[0] = VertexPos(0.0f, -m_Negative_Height, 0.0f);
	//Generate base vertices
	for (unsigned i = 0; i < m_RadialSegments; i++)
	{
		float angle = 2.0f * (float)M_PI * ((float)i / (float)m_RadialSegments);
		float x = sin(angle);
		float y = cos(angle);

		v[i + 1] = VertexPos(x * m_Radius, 0.0f, y * m_Radius);	
	}
	//Top vertex
	v[m_RadialSegments+1] = VertexPos(0,(float)m_Height,0);

	HR(m_VertexBuffer->Unlock());
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
