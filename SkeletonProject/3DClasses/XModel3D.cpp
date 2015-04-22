#include "XModel3D.h"

XModel3D::XModel3D(LPD3DXMESH pMesh)
{
	m_Mesh = pMesh;
	m_VertexCount = m_Mesh->GetNumVertices();
	m_TriCount = m_Mesh->GetNumFaces();
	m_IndexCount = m_TriCount * 3;
	Create();
}