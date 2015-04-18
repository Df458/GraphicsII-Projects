#pragma once
#include "MeshObject3D.h"

class ConeObject3D :
	public MeshObject3D
{
public:
	ConeObject3D(float radius, unsigned radialSegments, float height);
	virtual ~ConeObject3D(){};
protected:
	float m_Height;
	float m_Negative_Height = 0;//Must be zero for cone.
	float m_Radius;
	unsigned m_RadialSegments;

	//Implementation of pure virtual generation functions
	virtual void ConeObject3D::buildMeshBuffers(IDirect3DDevice9* gd3dDevice);
	virtual void buildUVBuffer(IDirect3DDevice9* gd3dDevice) {} // TODO: We should probably implement this someday. How did I even miss it?
};

