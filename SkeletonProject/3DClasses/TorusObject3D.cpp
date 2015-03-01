#include <d3dx9shape.h>
#include "TorusObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"


TorusObject3D::TorusObject3D(float radiusi, float radiusr, unsigned sides, unsigned rings, BaseMaterial* mat, ID3DXEffect* effect)
	:MeshObject3D(mat, effect), m_Radiusi(radiusi), m_Radiusr(radiusr), m_Sides(sides), m_Rings(rings)
{
}

void TorusObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
    HR(D3DXCreateTorus(gd3dDevice, m_Radiusi, m_Radiusr, m_Sides, m_Rings, &m_Mesh, NULL));
}

