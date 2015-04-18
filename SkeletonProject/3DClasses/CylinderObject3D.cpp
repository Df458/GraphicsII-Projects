#include <d3dx9shape.h>
#include <cfloat>
#include "CylinderObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"


CylinderObject3D::CylinderObject3D(float radius, unsigned radialSegments, float height, BaseMaterial* mat)
	:MeshObject3D(mat), m_Height(height), m_Radius(radius), m_RadialSegments(radialSegments)
{
	// Determine vertex count;
	m_VertexCount = (m_RadialSegments * 2) + 2;
	//Determine intex count;
	m_TriCount = m_RadialSegments * 4;
	m_IndexCount = m_TriCount * 3;
}

void CylinderObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
    HR(D3DXCreateCylinder(gd3dDevice, m_Radius, m_Radius, m_Height, m_RadialSegments, 1, &m_Mesh, NULL));
}

void CylinderObject3D::buildUVBuffer(IDirect3DDevice9* gd3dDevice)
{
    VertexPos* verts;
    ID3DXMesh* mesh = 0;
    D3DVERTEXELEMENT9 elements[64];
    unsigned element_count = 0;
    VertexPos::Decl->GetDeclaration(elements, &element_count);
    m_Mesh->CloneMesh(D3DXMESH_SYSTEMMEM, elements, gd3dDevice, &mesh);
    ReleaseCOM(m_Mesh);

    HR(mesh->LockVertexBuffer(0, (void**)&verts));
	D3DXVECTOR3 maxPoint(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	D3DXVECTOR3 minPoint(FLT_MAX, FLT_MAX, FLT_MAX);
	for(UINT i = 0; i < mesh->GetNumVertices(); ++i)
	{
		D3DXVec3Maximize(&maxPoint, &maxPoint, &verts[i].pos);
		D3DXVec3Minimize(&minPoint, &minPoint, &verts[i].pos);
	}

	float a = 0.0f;
	float b = 0.0f;
	float h = 0.0f;
    a = minPoint.z;
    b = maxPoint.z;
    h = b-a;
    for(unsigned i = 0; i < mesh->GetNumVertices(); ++i)
    {
		// Get the coordinates along the axes orthogonal to the
		// axis the cylinder is aligned with.

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
        x = verts[i].pos.x;
        z = verts[i].pos.y;
        y = verts[i].pos.z;

		// Convert to cylindrical coordinates.
		
		float theta = atan2f(z, x);
		float y2    = y - b; // Transform [a, b]-->[-h, 0]

		// Transform theta from [0, 2*pi] to [0, 1] range and
		// transform y2 from [-h, 0] to [0, 1].

		float u = theta / (2.0f*D3DX_PI);
		float v = y2 / -h; 
		
		// Save texture coordinates.
		
		verts[i].uv.x = u;
		verts[i].uv.y = v;
    }

    HR(mesh->UnlockVertexBuffer());
    HR(mesh->CloneMesh(D3DXMESH_MANAGED, elements, gd3dDevice, &m_Mesh));
    ReleaseCOM(mesh);
}
