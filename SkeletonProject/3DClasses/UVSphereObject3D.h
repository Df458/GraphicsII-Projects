#pragma once
#include "BaseObject3D.h"
class UVSphereObject3D :
	public BaseObject3D
{
public:
	UVSphereObject3D();
	~UVSphereObject3D();
protected:
    void buildVertexBuffer(IDirect3DDevice9* gd3dDevice, unsigned int resolution);
    void buildIndexBuffer(IDirect3DDevice9* gd3dDevice, unsigned int resolution);
};

