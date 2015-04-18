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
	virtual void buildMeshBuffers();
	virtual void buildUVBuffer();
};

