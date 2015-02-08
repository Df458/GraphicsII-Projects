#pragma once
#include "BaseObject3D.h"

class CuboidObject3D :
	public BaseObject3D
{
public:
	CuboidObject3D(float length, float width, float height, unsigned X_Resolution = 1, unsigned Y_Resolution = 1, unsigned Z_Resolution = 1);
	CuboidObject3D(float size, unsigned X_Resolution = 1, unsigned Y_Resolution = 1, unsigned Z_Resolution = 1);
	virtual ~CuboidObject3D(){};
protected:
	float m_Length; //X length
	float m_Width; //Z depth
	float m_Height; //Y Height;

	unsigned mX_Resolution; //Vertical divisions of Cuboid along the Lengths' sides
	unsigned mY_Resolution; //Horizontal divisions of Cuboid
	unsigned mZ_Resolution; //Vertical divisions of Cuboid along the Widths' sides

	//Implementation of pure virtual generation functions
	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);
};

