#pragma once
#include "MeshObject3D.h"

class ConeObject3D :
	public MeshObject3D
{
public:
	ConeObject3D(float radius, unsigned radialSegments, float height, BaseMaterial* mat, ID3DXEffect* effect = NULL);
	virtual ~ConeObject3D(){};
protected:
	float m_Height;
	float m_Negative_Height = 0;//Must be zero for cone.
	float m_Radius;
	unsigned m_RadialSegments;

	//Implementation of pure virtual generation functions
	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice) {}
};

