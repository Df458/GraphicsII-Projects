#pragma once
#include "BaseObject3D.h"
class CylinderObject3D :
	public BaseObject3D
{
public:
	CylinderObject3D();
	~CylinderObject3D();

	virtual void Create(IDirect3DDevice9* gd3dDevice, unsigned int resolution = 16);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection);
};

