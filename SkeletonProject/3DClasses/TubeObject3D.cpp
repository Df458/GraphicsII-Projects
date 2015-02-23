#include "TubeObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"

TubeObject3D::TubeObject3D(float radius, float width, unsigned radialSegments, float height, BaseMaterial* mat, ID3DXEffect* effect)
	:BaseObject3D(mat, effect),m_Radius(radius), m_Width(width), m_RadialSegments(radialSegments), m_Height(height)
{
	// Determine vertex count;
	m_VertexCount = (m_RadialSegments * 4);
	//Determine index count;
	m_TriCount = m_RadialSegments * 8;
	m_IndexCount = m_TriCount * 3;
}

void TubeObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	// Obtain a pointer to a new vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(m_VertexCount * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));


	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	for (unsigned i = 0; i < m_RadialSegments; i++)
	{
		float angle = 2.0f * (float)M_PI * ((float)i / (float)m_RadialSegments);
		float x = sin(angle);
		float y = cos(angle);

		v[i] = VertexPos(x * (m_Radius), 0.0f, y * (m_Radius));
		v[m_RadialSegments+i] = VertexPos(x * m_Radius, (float)m_Height, y * m_Radius);
		
		v[(m_RadialSegments*2)+i] = VertexPos(x * (m_Radius - m_Width), 0.0f, y * (m_Radius - m_Width));
		v[(m_RadialSegments*3)+i] = VertexPos(x * (m_Radius - m_Width), (float)m_Height, y * (m_Radius - m_Width));
	}

	
	HR(m_VertexBuffer->Unlock());
}

void TubeObject3D::buildIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	// Obtain a pointer to a new index buffer.
	HR(gd3dDevice->CreateIndexBuffer(m_IndexCount * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	WORD* k = 0;
	HR(m_IndexBuffer->Lock(0, 0, (void**)&k, 0));

	for (unsigned i = 0; i < m_RadialSegments; i++)
	{
		// Outer Ring
        k[(i * 24) + 0] = i;
        k[(i * 24) + 1] = i + 1;
        k[(i * 24) + 2] = i + m_RadialSegments;

        k[(i * 24) + 3] = i + m_RadialSegments + 1;
        k[(i * 24) + 4] = i + m_RadialSegments;
        k[(i * 24) + 5] = i + 1;

		// Inner Ring
		k[(i * 24) + 6] = i + m_RadialSegments * 2;
        k[(i * 24) + 7] = i + m_RadialSegments * 3;
        k[(i * 24) + 8] = i + m_RadialSegments * 2 + 1;

        k[(i * 24) + 9] = i + m_RadialSegments * 3 + 1;
        k[(i * 24) + 10] = i + m_RadialSegments * 2 + 1;
        k[(i * 24) + 11] = i + m_RadialSegments * 3;
		
		// Bottom Cap
        k[(i * 24) + 12] = i;
        k[(i * 24) + 13] = i + m_RadialSegments * 2;
        k[(i * 24) + 14] = i + 1;

        k[(i * 24) + 15] = i + m_RadialSegments * 2 + 1;
        k[(i * 24) + 16] = i + 1;
        k[(i * 24) + 17] = i + m_RadialSegments * 2;
		
		// Top Cap
        k[(i * 24) + 18] = i + m_RadialSegments * 3;
        k[(i * 24) + 19] = i + m_RadialSegments;
        k[(i * 24) + 20] = i + m_RadialSegments * 3 + 1;

        k[(i * 24) + 21] = i + m_RadialSegments + 1;
        k[(i * 24) + 22] = i + m_RadialSegments * 3 + 1;
        k[(i * 24) + 23] = i + m_RadialSegments;
		
        if( i == m_RadialSegments - 1) {
            k[(i * 24) + 1] = 0;
            k[(i * 24) + 3] = m_RadialSegments;
            k[(i * 24) + 5] = 0;
			
			k[(i * 24) + 8] = m_RadialSegments * 2;
            k[(i * 24) + 9] = m_RadialSegments * 3;
            k[(i * 24) + 10] = m_RadialSegments * 2;
			
			k[(i * 24) + 14] = 0;
            k[(i * 24) + 15] = m_RadialSegments * 2;
            k[(i * 24) + 16] = 0;
			
			k[(i * 24) + 20] = m_RadialSegments * 3;
            k[(i * 24) + 21] = m_RadialSegments;
            k[(i * 24) + 22] = m_RadialSegments * 3;
        }
	}

	HR(m_IndexBuffer->Unlock());
}
