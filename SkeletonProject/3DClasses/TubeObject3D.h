#pragma once
#include "BaseObject3D.h"
class TubeObject3D :
	public BaseObject3D
{
public:
	TubeObject3D(float radius, float width, unsigned radialSegments, float height, BaseMaterial* mat, ID3DXEffect* effect = NULL);
	virtual ~TubeObject3D(){};
protected:
	float m_Radius;
	float m_Width;
	unsigned m_RadialSegments;
	float m_Height;

	//Implementation of pure virtual generation functions
	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);
};

