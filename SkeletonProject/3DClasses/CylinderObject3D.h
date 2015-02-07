#pragma once
#include "BaseObject3D.h"
class CylinderObject3D :
	public BaseObject3D
{
public:
	CylinderObject3D();
	~CylinderObject3D();
protected:
    void buildVertexBuffer(IDirect3DDevice9* gd3dDevice, unsigned int resolution);
    void buildIndexBuffer(IDirect3DDevice9* gd3dDevice, unsigned int resolution);
};

