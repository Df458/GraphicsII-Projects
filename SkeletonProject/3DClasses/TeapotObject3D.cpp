#include "TeapotObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"


TeapotObject3D::TeapotObject3D(BaseMaterial* mat, ID3DXEffect* effect)
	:MeshObject3D(mat, effect)
{
	m_VertexCount = 8;
	m_TriCount = 12;
	m_IndexCount = 36;
}

void TeapotObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice) 
{
    HR(D3DXCreateTeapot(gd3dDevice, &m_Mesh, NULL));
}

void TeapotObject3D::buildUVBuffer(IDirect3DDevice9* gd3dDevice)
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
		// Convert to spherical coordinates.
		D3DXVECTOR3 p = verts[i].pos;
	
		float theta = atan2f(p.z, p.x);
		float phi   = acosf(p.y / sqrtf(p.x*p.x+p.y*p.y+p.z*p.z));

		// Phi and theta give the texture coordinates, but are not in 
		// the range [0, 1], so scale them into that range.

		float u = theta / (2.0f*D3DX_PI);
		float v = phi   / D3DX_PI;
		
		// Save texture coordinates.
		
		verts[i].uv.x = u;
		verts[i].uv.y = v;
    }

    HR(mesh->UnlockVertexBuffer());
    HR(mesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, elements, gd3dDevice, &m_Mesh));
    ReleaseCOM(mesh);
}
