#include "SimpleCubeObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"
#include "../d3dUtil.h"


SimpleCubeObject3D::SimpleCubeObject3D(BaseMaterial* mat, float w, float h, float d, ID3DXEffect* effect)
	:MeshObject3D(mat, effect)
{
    m_Width = w;
    m_Height = h;
    m_Depth = d;

	m_VertexCount = 8;
	m_TriCount = 12;
	m_IndexCount = 36;
}

void SimpleCubeObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice) 
{
    HR(D3DXCreateBox(gd3dDevice, m_Width, m_Height, m_Depth,  &m_Mesh, NULL));
}

void SimpleCubeObject3D::buildUVBuffer(IDirect3DDevice9* gd3dDevice)
{
    VertexPos* verts;
    ID3DXMesh* mesh = 0;
    D3DVERTEXELEMENT9 elements[64];
    unsigned element_count = 0;
    VertexPos::Decl->GetDeclaration(elements, &element_count);
    m_Mesh->CloneMesh(D3DXMESH_SYSTEMMEM, elements, gd3dDevice, &mesh);
    ReleaseCOM(m_Mesh);
    HR(mesh->LockVertexBuffer(0, (void**)&verts));
    for(unsigned i = 0; i < mesh->GetNumVertices(); ++i)
    {
        verts[i].uv.x = (float)(rand() % 10) * 0.1f;
        verts[i].uv.y = (float)(rand() % 10) * 0.1f;
    }
    HR(mesh->UnlockVertexBuffer());
    HR(mesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, elements, gd3dDevice, &m_Mesh));
    ReleaseCOM(mesh);
}
