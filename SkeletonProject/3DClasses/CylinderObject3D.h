#pragma once
#include "BaseObject3D.h"
class CylinderObject3D :
	public BaseObject3D
{
public:
	CylinderObject3D();
	~CylinderObject3D();
protected:
	unsigned m_Height;
	unsigned m_Radius;
	unsigned m_RadialSegments;

	virtual void Create(IDirect3DDevice9* gd3dDevice, unsigned int resolution);
    void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
    void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);
};

