#pragma once
#include "BaseObject3D.h"
class ConeObject3D :
	public BaseObject3D
{
public:
	ConeObject3D(float radius, unsigned radialSegments, float height);
	virtual ~ConeObject3D();
protected:
	float m_Height;
	float m_Negative_Height = 0;//Must be zero for cone.
	float m_Radius;
	unsigned m_RadialSegments;

	//Implementation of pure virtual generation functions
	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);
};

