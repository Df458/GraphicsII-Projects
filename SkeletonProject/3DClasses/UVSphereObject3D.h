#pragma once
#include "MeshObject3D.h"
class UVSphereObject3D :
	public MeshObject3D
{
public:
	UVSphereObject3D(float radius, unsigned rings, unsigned radialSegments);
	~UVSphereObject3D();
protected:
    float m_Radius;
    unsigned m_Rings;
    unsigned m_RadialSegments;

    void buildMeshBuffers();
	virtual void buildUVBuffer();
};

