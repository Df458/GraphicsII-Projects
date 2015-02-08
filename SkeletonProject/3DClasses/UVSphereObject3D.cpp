#include "UVSphereObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"

#define _USE_MATH_DEFINES 
#include <math.h>


UVSphereObject3D::UVSphereObject3D(float radius, unsigned rings, unsigned radialSegments)
{
    m_Radius = radius;
    m_Rings = rings;
    m_RadialSegments = radialSegments;

    m_VertexCount = m_Rings * m_RadialSegments + 2;
    m_TriCount = radialSegments * 2 + (m_RadialSegments * (m_Rings-1) * 2);
    m_IndexCount = m_TriCount * 3;
}


UVSphereObject3D::~UVSphereObject3D()
{
}

void UVSphereObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	// Obtain a pointer to a new vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(m_VertexCount * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	v[0] = VertexPos(0.0f, m_Radius, 0.0f);
	v[1] = VertexPos(0.0f, -m_Radius, 0.0f);

    for(unsigned i = 0; i < m_Rings; ++i)
    {
		float angle1 = 2.0f * (float)M_PI * ((float)(i + 1) / (float)(m_Rings));
        float ringRadius = sin(angle1 * 0.5) * m_Radius;
        for(unsigned j = 0; j < m_RadialSegments; ++j)
        {
            float angle2 = 2.0f * (float)M_PI * ((float)j / (float)m_RadialSegments);
            v[(i * m_RadialSegments) + j + 2] = VertexPos(sin(angle2) * ringRadius,
                                                     m_Radius * cos(angle1 * 0.5),
                                                     cos(angle2) * ringRadius);
        }
    }
	HR(m_VertexBuffer->Unlock());
}

void UVSphereObject3D::buildIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	// Obtain a pointer to a new index buffer.
	HR(gd3dDevice->CreateIndexBuffer(m_IndexCount * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	WORD* k = 0;
	HR(m_IndexBuffer->Lock(0, 0, (void**)&k, 0));

    // Cap indices
    for(unsigned i = 0; i < m_RadialSegments; ++i)
    {
        k[i * 6] = 0;
        k[i * 6 + 1] = i + 2;
        k[i * 6 + 2] = i + 3;

        k[i * 6 + 3] = 1;
        k[i * 6 + 4] = m_VertexCount - m_RadialSegments + i + 1;
        k[i * 6 + 5] = m_VertexCount - m_RadialSegments + i;

        if(i == m_RadialSegments - 1)
        {
            k[i * 6 + 2] = 2;
            k[i * 6 + 4] = m_VertexCount - m_RadialSegments;
        }
    }

    unsigned capoffset = m_RadialSegments * 6;

    //Other indices
    for(unsigned i = 0; i < m_Rings - 1; ++i)
    {
        unsigned leveloffset = i * m_RadialSegments * 6;
        for(unsigned j = 0; j < m_RadialSegments; ++j)
        {
            unsigned voffset = i * m_RadialSegments + j;
            k[capoffset + leveloffset + (j * 6)] = voffset + 2;
            k[capoffset + leveloffset + (j * 6) + 1] = voffset + (m_RadialSegments) + 2;
            k[capoffset + leveloffset + (j * 6) + 2] = voffset + (m_RadialSegments) + 3;

            k[capoffset + leveloffset + (j * 6) + 3] = voffset + 2;
            k[capoffset + leveloffset + (j * 6) + 4] = voffset + (m_RadialSegments) + 3;
            k[capoffset + leveloffset + (j * 6) + 5] = voffset + 3;

            if(j == m_RadialSegments - 1)
            {
                k[capoffset + leveloffset + (j * 6) + 2] = (i + 1) * m_RadialSegments + 2;
                k[capoffset + leveloffset + (j * 6) + 4] = (i + 1) * m_RadialSegments + 2;
                k[capoffset + leveloffset + (j * 6) + 5] = i * m_RadialSegments + 2;
            }
        }
    }
    
    k[0] = 0;
    k[1] = 2;
    k[2] = 3;

	HR(m_IndexBuffer->Unlock());
}
