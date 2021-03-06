#include <d3dx9shape.h>
#include "TorusObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"


TorusObject3D::TorusObject3D(float radiusi, float radiusr, unsigned sides, unsigned rings)
	:m_Radiusi(radiusi), m_Radiusr(radiusr), m_Sides(sides), m_Rings(rings)
{
	Create();
}

void TorusObject3D::buildMeshBuffers()
{
    HR(D3DXCreateTorus(gd3dDevice, m_Radiusi, m_Radiusr, m_Sides, m_Rings, &m_Mesh, NULL));
}

void TorusObject3D::buildUVBuffer()
{
	// TODO: We might want to try this if we have time
}
