#pragma once
#include "MeshObject3D.h"
class UVSphereObject3D :
	public MeshObject3D
{
public:
	UVSphereObject3D(float radius, unsigned rings, unsigned radialSegments, BaseMaterial* mat, ID3DXEffect* effect = NULL);
	~UVSphereObject3D();
protected:
    float m_Radius;
    unsigned m_Rings;
    unsigned m_RadialSegments;

    void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
    void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);
};

