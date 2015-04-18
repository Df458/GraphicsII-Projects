#pragma once
#include "MeshObject3D.h"
class CylinderObject3D :
	public MeshObject3D
{
public:
	CylinderObject3D(float radius, unsigned radialSegments, float height);
	virtual ~CylinderObject3D(){};
protected:
	float m_Height;
	float m_Radius;
	unsigned m_RadialSegments;

	//Implementation of pure virtual generation functions
    virtual void buildMeshBuffers(IDirect3DDevice9* gd3dDevice);
	virtual void buildUVBuffer(IDirect3DDevice9* gd3dDevice);
};

