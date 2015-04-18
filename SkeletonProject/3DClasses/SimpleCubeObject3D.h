#pragma once
#include "MeshObject3D.h"

class SimpleCubeObject3D :
	public MeshObject3D
{
public:
	SimpleCubeObject3D(BaseMaterial* mat, float w = 1, float h = 1, float d = 1);
	virtual ~SimpleCubeObject3D(){};
protected:
    float m_Width = 1;
    float m_Height = 1;
    float m_Depth = 1;
	//Implementation of pure virtual generation functions
	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice) {}
	virtual void buildUVBuffer(IDirect3DDevice9* gd3dDevice);
};


