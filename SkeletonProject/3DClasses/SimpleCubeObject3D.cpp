#include "SimpleCubeObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"
#include "../d3dUtil.h"


SimpleCubeObject3D::SimpleCubeObject3D(float w, float h, float d)
{
    m_Width = w;
    m_Height = h;
    m_Depth = d;

	m_VertexCount = 8;
	m_TriCount = 12;
	m_IndexCount = 36;
	Create();
}

void SimpleCubeObject3D::buildMeshBuffers() 
{
    HR(D3DXCreateBox(gd3dDevice, m_Width, m_Height, m_Depth,  &m_Mesh, NULL));
}

void SimpleCubeObject3D::buildUVBuffer()
{
    VertexPos* verts;
    ID3DXMesh* mesh = 0;
    D3DVERTEXELEMENT9 elements[64];
    unsigned element_count = 0;
    VertexPos::Decl->GetDeclaration(elements, &element_count);
    m_Mesh->CloneMesh(D3DXMESH_SYSTEMMEM, elements, gd3dDevice, &mesh);
    ReleaseCOM(m_Mesh);
    HR(mesh->LockVertexBuffer(0, (void**)&verts));
    for(unsigned i = 0; i < mesh->GetNumVertices() / 4; ++i)
    {
        verts[i * 4].uv.x = 0;
        verts[i * 4].uv.y = 1;
        verts[i * 4 + 1].uv.x = 0;
        verts[i * 4 + 1].uv.y = 0;
        verts[i * 4 + 2].uv.x = 1;
        verts[i * 4 + 2].uv.y = 0;
        verts[i * 4 + 3].uv.x = 1;
        verts[i * 4 + 3].uv.y = 1;
    }
    HR(mesh->UnlockVertexBuffer());
    HR(mesh->CloneMesh(D3DXMESH_MANAGED, elements, gd3dDevice, &m_Mesh));
    ReleaseCOM(mesh);
}
