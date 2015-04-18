#pragma once
#include "MeshObject3D.h"

class TeapotObject3D :
	public MeshObject3D
{
public:
	TeapotObject3D();
	virtual ~TeapotObject3D(){};
protected:
	//Implementation of pure virtual generation functions
	virtual void buildMeshBuffers(IDirect3DDevice9* gd3dDevice);
	virtual void buildUVBuffer(IDirect3DDevice9* gd3dDevice);
};

