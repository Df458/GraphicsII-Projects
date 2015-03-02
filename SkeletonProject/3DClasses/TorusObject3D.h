#pragma once
#include "MeshObject3D.h"
class TorusObject3D :
	public MeshObject3D
{
public:
	TorusObject3D(float radiusi, float radiusr, unsigned sides, unsigned rings, BaseMaterial* mat, ID3DXEffect* effect = NULL);
	virtual ~TorusObject3D(){};
protected:
	float m_Radiusi;
	float m_Radiusr;
    unsigned m_Sides;
    unsigned m_Rings;

	//Implementation of pure virtual generation functions
    virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
    virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice) {}
    virtual void buildUVBuffer(IDirect3DDevice9* gd3dDevice);
};
