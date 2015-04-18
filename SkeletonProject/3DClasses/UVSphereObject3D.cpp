#include "UVSphereObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"


UVSphereObject3D::UVSphereObject3D(float radius, unsigned rings, unsigned radialSegments)
{
    m_Radius = radius;
    m_Rings = rings;
    m_RadialSegments = radialSegments;

    m_VertexCount = m_Rings * m_RadialSegments + 2;
    m_TriCount = radialSegments * 2 + (m_RadialSegments * (m_Rings-1) * 2);
    m_IndexCount = m_TriCount * 3;

	Create();
}


UVSphereObject3D::~UVSphereObject3D()
{
}

void UVSphereObject3D::buildMeshBuffers()
{
    HR(D3DXCreateSphere(gd3dDevice, m_Radius, m_Rings, m_RadialSegments, &m_Mesh, NULL));
}

void UVSphereObject3D::buildUVBuffer()
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
    HR(mesh->CloneMesh(D3DXMESH_MANAGED, elements, gd3dDevice, &m_Mesh));
    ReleaseCOM(mesh);
}
