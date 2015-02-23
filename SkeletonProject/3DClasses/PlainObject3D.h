#pragma once
#include "BaseObject3D.h"

class PlainObject3D :
	public BaseObject3D
{
public:
	PlainObject3D(float length, float width, BaseMaterial* mat, ID3DXEffect* effect = NULL, unsigned X_Resolution = 1, unsigned Z_Resolution = 1);
	PlainObject3D(float size, BaseMaterial* mat, ID3DXEffect* effect = NULL, unsigned X_Resolution = 1, unsigned Z_Resolution = 1);
	virtual ~PlainObject3D(){};
protected:
	float m_Length; //X length
	float m_Width; //Y Height;

	unsigned mX_Resolution; //Vertical divisions of Cuboid along the Lengths' sides
	unsigned mZ_Resolution; //Vertical divisions of Cuboid along the Widths' sides

	//Implementation of pure virtual generation functions
	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);
};

