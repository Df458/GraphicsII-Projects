#pragma once
#include "BaseObject3D.h"
class IcoSphereObject3D :
	public BaseObject3D
{
public:
	IcoSphereObject3D();
	~IcoSphereObject3D();
protected:
    void buildVertexBuffer(IDirect3DDevice9* gd3dDevice, unsigned int resolution);
    void buildIndexBuffer(IDirect3DDevice9* gd3dDevice, unsigned int resolution);
};

