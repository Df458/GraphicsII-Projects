#pragma once
#include "BaseObject3D.h"
class TubeObject3D :
	public BaseObject3D
{
public:
	TubeObject3D(float radius, unsigned radialSegments, float height);
	virtual ~TubeObject3D(){};
protected:
	float m_Height;
	float m_Radius;
	unsigned m_RadialSegments;

	//Implementation of pure virtual generation functions
	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);
};

