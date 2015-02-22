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
}


UVSphereObject3D::~UVSphereObject3D()
{
}

void UVSphereObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice)
{
    HR(D3DXCreateSphere(gd3dDevice, m_Radius, m_Rings, m_RadialSegments, &m_Mesh, NULL));
}

void UVSphereObject3D::buildIndexBuffer(IDirect3DDevice9* gd3dDevice)
{
}
