//=============================================================================
//                              BaseObject3D
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that can handle 3D rendergin via Vertex and Index buffer
//=============================================================================
#include "BaseObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"
#include "../Materials/BaseMaterial.h"
//=============================================================================
BaseObject3D::BaseObject3D(BaseMaterial* mat, ID3DXEffect* effect) : m_Material(mat)
{
    m_VertexBuffer = NULL;
    m_IndexBuffer = NULL;
    if(effect)
        mat->ConnectToEffect(effect);
}

//-----------------------------------------------------------------------------
BaseObject3D::~BaseObject3D(void)
{
    ReleaseCOM(m_VertexBuffer);
	ReleaseCOM(m_IndexBuffer);
    delete m_Material;
}

void BaseObject3D::attachShader(ID3DXEffect* effect)
{
    m_Material->ConnectToEffect(effect);
}

void BaseObject3D::Create(IDirect3DDevice9* gd3dDevice)
{
    buildVertexBuffer( gd3dDevice);
    buildIndexBuffer( gd3dDevice);
}

//-----------------------------------------------------------------------------
void BaseObject3D::Render( IDirect3DDevice9* gd3dDevice,
    D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection)
{
    // Update the statistics singlton class
    GfxStats::GetInstance()->addVertices(m_VertexCount);
    GfxStats::GetInstance()->addTriangles(m_TriCount);
    unsigned passes = m_Material->PreRender();
    D3DXMATRIX vp = view * projection;
    for(unsigned i = 0; i < passes; ++i)
    {
        m_Material->Render(world, vp, i);

        // Set the buffers and format
        HR(gd3dDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VertexPos)));
        HR(gd3dDevice->SetIndices(m_IndexBuffer));
        HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));
        
        // Send to render
        HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexCount, 0, m_TriCount));
        m_Material->PostPass();
    }
    m_Material->PostRender();
}
//-----------------------------------------------------------------------------
void BaseObject3D::Update(float deltaTime)
{
	//TODO: add updating for rotation, transformation, etc...
}

//-----------------------------------------------------------------------------
/*
void BaseObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice, unsigned int resolution)
{
	// Obtain a pointer to a new vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(8 * sizeof(VertexPos), D3DUSAGE_WRITEONLY,
		0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// cube's vertex data.

	VertexPos* v = 0;
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	v[0] = VertexPos(-1.0f, -1.0f, -1.0f);
	v[1] = VertexPos(-1.0f,  1.0f, -1.0f);
	v[2] = VertexPos( 1.0f,  1.0f, -1.0f);
	v[3] = VertexPos( 1.0f, -1.0f, -1.0f);
	v[4] = VertexPos(-1.0f, -1.0f,  1.0f);
	v[5] = VertexPos(-1.0f,  1.0f,  1.0f);
	v[6] = VertexPos( 1.0f,  1.0f,  1.0f);
	v[7] = VertexPos( 1.0f, -1.0f,  1.0f);

	HR(m_VertexBuffer->Unlock());
    m_VertexCount = 8;
}*/

//-----------------------------------------------------------------------------
/*
void BaseObject3D::buildIndexBuffer(IDirect3DDevice9* gd3dDevice, unsigned int resolution)
{
	// Obtain a pointer to a new index buffer.
	HR(gd3dDevice->CreateIndexBuffer(36 * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// cube's index data.

	WORD* k = 0;

	HR(m_IndexBuffer->Lock(0, 0, (void**)&k, 0));

	// Front face.
	k[0] = 0; k[1] = 1; k[2] = 2;
	k[3] = 0; k[4] = 2; k[5] = 3;

	// Back face.
	k[6] = 4; k[7]  = 6; k[8]  = 5;
	k[9] = 4; k[10] = 7; k[11] = 6;

	// Left face.
	k[12] = 4; k[13] = 5; k[14] = 1;
	k[15] = 4; k[16] = 1; k[17] = 0;

	// Right face.
	k[18] = 3; k[19] = 2; k[20] = 6;
	k[21] = 3; k[22] = 6; k[23] = 7;

	// Top face.
	k[24] = 1; k[25] = 5; k[26] = 6;
	k[27] = 1; k[28] = 6; k[29] = 2;

	// Bottom face.
	k[30] = 4; k[31] = 0; k[32] = 3;
	k[33] = 4; k[34] = 3; k[35] = 7;

	HR(m_IndexBuffer->Unlock());
    m_IndexCount = 36;
    m_TriCount = m_IndexCount / 3;
}*/
//=============================================================================
