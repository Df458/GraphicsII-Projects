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

	HR(m_VertexBuffer->Unlock());
}

void CuboidObject3D::buildIndexBuffer(IDirect3DDevice9* gd3dDevice) 
{
	// Obtain a pointer to a new index buffer.
	HR(gd3dDevice->CreateIndexBuffer(m_IndexCount * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	WORD* k = 0;
	HR(m_IndexBuffer->Lock(0, 0, (void**)&k, 0));

	//do stuff
	
	unsigned yUp = mZ_Resolution + 1;
	unsigned totalIndices = 0;
	unsigned currentVertIndex = 0;

	//Generate Left/Right Face
	for (unsigned tris = 0; tris < mZ_Resolution * mY_Resolution * 2; tris += 2)
	{
		k[totalIndices] = currentVertIndex;
		k[totalIndices + 1] = currentVertIndex + 1 + yUp;
		k[totalIndices + 2] = currentVertIndex + yUp;

		k[totalIndices + 3] = currentVertIndex + 1;
		k[totalIndices + 4] = currentVertIndex + 1 + yUp;
		k[totalIndices + 5] = currentVertIndex;

		//right side
		k[m_IndexCount - 1 - totalIndices]		 = m_VertexCount - 1 - (currentVertIndex);
		k[m_IndexCount - 1 - (totalIndices + 1)] = m_VertexCount - 1 - (currentVertIndex + 1 + yUp);
		k[m_IndexCount - 1 - (totalIndices + 2)] = m_VertexCount - 1 - (currentVertIndex + yUp);

		k[m_IndexCount - 1 - (totalIndices + 3)] = m_VertexCount - 1 - (currentVertIndex + 1);
		k[m_IndexCount - 1 - (totalIndices + 4)] = m_VertexCount - 1 - (currentVertIndex + 1 + yUp);
		k[m_IndexCount - 1 - (totalIndices + 5)] = m_VertexCount - 1 - (currentVertIndex);


		totalIndices += 6;
		currentVertIndex += 1;//to go next sub box
		if ((currentVertIndex + 1) % yUp == 0)//if its the end 
		{
			currentVertIndex++;//go up a row
		}
	}
	//Generate Stuff
	//face segment shift
	int leftFaceIndexSize = totalIndices;
	unsigned leftFaceVertSize =
		4
		+ (mZ_Resolution - 1) * 2
		+ (mY_Resolution - 1) * 2 + (mY_Resolution - 1) * (mZ_Resolution - 1);

	//ring segment shift
	unsigned leftShift = (mZ_Resolution + 1) * 2 +  (mY_Resolution - 1) * 2;
	currentVertIndex = 0 + leftFaceVertSize;

	do
	{
		//Make Ring Sections
		//Determine if face is to the left and set left traversing value
		int Ringleft = leftShift;
		int leftsideUp = 2;
		int rightsideUp = 2;

		//if left edge
		if (currentVertIndex == leftFaceVertSize)
		{
			Ringleft = leftFaceVertSize;
			leftsideUp = mZ_Resolution + 1;
		}
		//if right edge
		if (currentVertIndex == m_VertexCount - leftFaceVertSize)
			rightsideUp = mZ_Resolution + 1; //traversing up is 3

		//Make Top/Bottom side
		for (unsigned i = 0; i < mZ_Resolution; i++)
		{
			//Bottom
			k[totalIndices] = currentVertIndex - Ringleft + i;
			k[totalIndices + 2] = currentVertIndex + 1 + i;
			k[totalIndices + 1] = currentVertIndex + i;

			k[totalIndices + 3] = currentVertIndex - Ringleft + i;
			k[totalIndices + 4] = currentVertIndex + 1 + i;
			k[totalIndices + 5] = currentVertIndex - Ringleft + 1 + i;

			totalIndices += 6;

			//Top
			k[totalIndices] = currentVertIndex - Ringleft + i + (leftsideUp * (mY_Resolution - 1) + mZ_Resolution + 1);// left back 
			k[totalIndices + 1] = currentVertIndex + 1 + i + (rightsideUp * (mY_Resolution - 1) + mZ_Resolution + 1);//right front
			k[totalIndices + 2] = currentVertIndex + i +(rightsideUp * (mY_Resolution - 1) + mZ_Resolution + 1);

			k[totalIndices + 3] = currentVertIndex - Ringleft + i + (leftsideUp * (mY_Resolution - 1) + mZ_Resolution + 1);
			k[totalIndices + 5] = currentVertIndex + 1 + i + (rightsideUp * (mY_Resolution - 1) + mZ_Resolution + 1);
			k[totalIndices + 4] = currentVertIndex - Ringleft + 1 + i + (leftsideUp * (mY_Resolution - 1) + mZ_Resolution + 1);

			totalIndices += 6;
		}
		
		//Make Front/Back side
		//default for nonend ring
		int forwardUp = 2;

		for (unsigned i = 0; i < mY_Resolution; i++)
		{		

			//back end
			if (i == 0)
			{
				k[totalIndices] = currentVertIndex;
				k[totalIndices + 2] = currentVertIndex + mZ_Resolution + 1;
				k[totalIndices + 1] = currentVertIndex - Ringleft + mZ_Resolution +1;
				
				k[totalIndices + 3] = currentVertIndex - Ringleft;
				k[totalIndices + 5] = currentVertIndex;
				k[totalIndices + 4] = currentVertIndex - Ringleft + mZ_Resolution + 1;
			}
			else
			{
				k[totalIndices] = currentVertIndex + mZ_Resolution + 1 + (i - 1) * rightsideUp;
				k[totalIndices + 2] = currentVertIndex + mZ_Resolution + 1 + (i) * rightsideUp;
				k[totalIndices + 1] = currentVertIndex - Ringleft + mZ_Resolution + 1 + (i)* leftsideUp;
				
				k[totalIndices + 3] = currentVertIndex - Ringleft + mZ_Resolution + 1 + (i)* leftsideUp;
				k[totalIndices + 4] = currentVertIndex + mZ_Resolution + 1 + (i - 1) * rightsideUp;

				//edge block. traverse down by zdepth + 1
				if (currentVertIndex == leftFaceVertSize)
				{
					k[totalIndices + 5] = currentVertIndex - Ringleft + mZ_Resolution + 1 + (i)* leftsideUp - (mZ_Resolution + 1);
				}
				else
				{
					k[totalIndices + 5] = currentVertIndex - Ringleft + mZ_Resolution + 1 + (i)* leftsideUp -2;
				}
			}
			
			totalIndices += 6;

			//front end
			if (i == 0)
			{
				k[totalIndices] = currentVertIndex + mZ_Resolution;
				k[totalIndices + 2] = currentVertIndex + mZ_Resolution - Ringleft;
				k[totalIndices + 1] = currentVertIndex + mZ_Resolution + rightsideUp;

				k[totalIndices + 3] = currentVertIndex + mZ_Resolution - Ringleft + leftsideUp;
				k[totalIndices + 5] = currentVertIndex + mZ_Resolution + rightsideUp;
				k[totalIndices + 4] = currentVertIndex + mZ_Resolution - Ringleft;
			}
			else
			{
				k[totalIndices] = currentVertIndex + mZ_Resolution + rightsideUp * i; //13 @ 2,2,2
				k[totalIndices + 2] = currentVertIndex - Ringleft + mZ_Resolution + leftsideUp * i;//evil one
				k[totalIndices + 1] = currentVertIndex + mZ_Resolution + rightsideUp * i + (rightsideUp);


				k[totalIndices + 4] = currentVertIndex - Ringleft + mZ_Resolution + leftsideUp * i;
				k[totalIndices + 5] = currentVertIndex + mZ_Resolution + rightsideUp * i + (rightsideUp);
				k[totalIndices + 3] = k[totalIndices + 4] + leftsideUp;

				if (i == mY_Resolution - 1)
				{
					k[totalIndices + 1] = currentVertIndex + mZ_Resolution + rightsideUp * i + (mZ_Resolution + 1);
					k[totalIndices + 5] = currentVertIndex + mZ_Resolution + rightsideUp * i + (mZ_Resolution + 1);
					k[totalIndices + 3] = k[totalIndices + 4] + (mZ_Resolution + 1);
				}
			}

			totalIndices += 6;			
		}
		currentVertIndex += (mZ_Resolution + 1) * 2 + (mY_Resolution - 1) * 2;
	} while (totalIndices < m_IndexCount - leftFaceIndexSize);

	HR(m_IndexBuffer->Unlock());
}