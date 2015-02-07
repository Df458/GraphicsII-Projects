#pragma once
#include "BaseObject3D.h"
class ConeObject3D :
	public BaseObject3D
{
public:
	ConeObject3D();
	~ConeObject3D();
protected:
	unsigned m_Height;
	unsigned m_Radius;
	unsigned m_RadialSegments;

	virtual void Create(IDirect3DDevice9* gd3dDevice, unsigned int resolution);
		

	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);
};

