#pragma once
#include "BaseObject3D.h"
class UVSphereObject3D :
	public BaseObject3D
{
public:
	UVSphereObject3D(float radius, unsigned rings, unsigned radialSegments);
	~UVSphereObject3D();
protected:
    float m_Radius;
    unsigned m_Rings;
    unsigned m_RadialSegments;

    void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
    void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);
};

