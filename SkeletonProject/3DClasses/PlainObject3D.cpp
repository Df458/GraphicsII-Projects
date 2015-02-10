#include "PlainObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"

PlainObject3D::PlainObject3D(float length, float width, unsigned X_Resolution, unsigned Z_Resolution)
	:BaseObject3D(),m_Length(length), m_Width(width), mX_Resolution(X_Resolution),  mZ_Resolution(Z_Resolution)
{
	//Calculate Vertex Count
	m_VertexCount =
		4
		+ (X_Resolution - 1) * 2
		+ (Z_Resolution - 1) * 2 + (Z_Resolution - 1) * (X_Resolution - 1);

		//Calculate Triangle Count
	m_TriCount = X_Resolution * Z_Resolution * 2;

	m_IndexCount = m_TriCount * 3;
}

PlainObject3D::PlainObject3D(float size, unsigned X_Resolution, unsigned Z_Resolution)
	:PlainObject3D(size, size, X_Resolution, Z_Resolution)
{

}

void PlainObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
	// Obtain a pointer to a new vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(m_VertexCount * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
	0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	unsigned currentVert = 0;
	//Top left back
	for (unsigned x = 0; x < 1 + mX_Resolution; x++)
	{
		float X_Location = ((float)x / (float)mX_Resolution) ;
		for (unsigned z = 0; z < 1 + mZ_Resolution; z++)
		{
			float Z_Location = ((float)z / (float)mZ_Resolution);

			v[currentVert] = VertexPos(X_Location * m_Length - m_Length * 0.5f, 0.0f, Z_Location * m_Width - m_Width * 0.5f);
			currentVert++;
		}
	}

	HR(m_VertexBuffer->Unlock());
}

void PlainObject3D::buildIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
	// Obtain a pointer to a new index buffer.
	HR(gd3dDevice->CreateIndexBuffer(m_IndexCount * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	WORD* k = 0;
	HR(m_IndexBuffer->Lock(0, 0, (void**)&k, 0));

	//do stuff
	
	//Generate Left Face
	int zdepth = mZ_Resolution;
	int xUp = zdepth + 1;
	int currentDepth = 0;
	int totalIndices = 0;
	int currentVertIndex = 0;
	for (unsigned tris = 0; tris < mX_Resolution * mZ_Resolution * 2; tris += 2)
	{
		k[totalIndices] = currentVertIndex;
		k[totalIndices + 1] = currentVertIndex + 1 + xUp;
		k[totalIndices + 2] = currentVertIndex + xUp;

		k[totalIndices + 3] = currentVertIndex + 1;
		k[totalIndices + 4] = currentVertIndex + 1 + xUp;
		k[totalIndices + 5] = currentVertIndex;
		totalIndices += 6;

		currentVertIndex += 1;//to go next sub box
		if ((currentVertIndex + 1) % xUp == 0)//if its the end 
		{
			currentVertIndex++;//go up a row
		}
	}

	HR(m_IndexBuffer->Unlock());
}