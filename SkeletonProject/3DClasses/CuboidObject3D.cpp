#include "CuboidObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"

CuboidObject3D::CuboidObject3D(float length, float width, float height, unsigned X_Resolution, unsigned Y_Resolution, unsigned Z_Resolution)
	:BaseObject3D(),m_Length(length), m_Width(width), m_Height(height), mX_Resolution(X_Resolution), mY_Resolution(Y_Resolution), mZ_Resolution(Z_Resolution)
{
	//Calculate Vertex Count
	m_VertexCount = 
		8 
		+ (X_Resolution - 1) * 4 
		+ (Y_Resolution - 1) * 4 + (Y_Resolution - 1) * (X_Resolution - 1) * 2
		+ (Z_Resolution - 1) * 4 + (Z_Resolution - 1) * (Y_Resolution - 1) * 2 + (Z_Resolution - 1) * (X_Resolution - 1) * 2;

	//Calculate Triangle Count
	m_TriCount =
		12
		+ (X_Resolution - 1) * 8
		+ (Y_Resolution - 1) * 8 + (Y_Resolution - 1) * (X_Resolution - 1) * 4
		+ (Z_Resolution - 1) * 8 + (Z_Resolution - 1) * (Y_Resolution - 1) * 4 + (Z_Resolution - 1) * (X_Resolution - 1) * 4;

	m_IndexCount = m_TriCount * 3;
}

CuboidObject3D::CuboidObject3D(float size, unsigned X_Resolution, unsigned Y_Resolution, unsigned Z_Resolution)
	:CuboidObject3D(size, size, size, X_Resolution, Y_Resolution, Z_Resolution)
{

}

void CuboidObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice) 
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
		for (unsigned y = 0; y < 1 + mY_Resolution; y++)
		{
			float Y_Location = ((float)y / (float)mY_Resolution);
			for (unsigned z = 0; z < 1 + mZ_Resolution; z++)
			{
				float Z_Location = ((float)z / (float)mZ_Resolution);

				if (X_Location == 1 || X_Location == 0 || Y_Location == 1 || Y_Location == 0 || Z_Location == 1 || Z_Location == 0)//if not an internal spot
				{
					v[currentVert] = VertexPos(X_Location * m_Length - m_Length * 0.5f, Y_Location  * m_Height - m_Height * 0.5f, Z_Location * m_Width - m_Width * 0.5f);
					currentVert++;
				}
			}
		}
	}
}

void CuboidObject3D::buildIndexBuffer(IDirect3DDevice9* gd3dDevice) 
{

}