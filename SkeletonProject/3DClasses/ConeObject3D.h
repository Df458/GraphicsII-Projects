#pragma once
#include "BaseObject3D.h"
class ConeObject3D :
	public BaseObject3D
{
public:
	ConeObject3D();
	~ConeObject3D();
protected:
    void buildVertexBuffer(IDirect3DDevice9* gd3dDevice, unsigned int resolution);
    void buildIndexBuffer(IDirect3DDevice9* gd3dDevice, unsigned int resolution);
};

