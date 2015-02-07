#pragma once
#include "BaseObject3D.h"
class CubeObject3D :
	public BaseObject3D
{
public:
	CubeObject3D();
	~CubeObject3D();
protected:
	void buildVertexBuffer(IDirect3DDevice9* gd3dDevice, unsigned int resolution);
	void buildIndexBuffer(IDirect3DDevice9* gd3dDevice, unsigned int resolution);
};

