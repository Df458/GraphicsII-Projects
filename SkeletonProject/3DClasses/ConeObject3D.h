#pragma once
#include "BaseObject3D.h"
class ConeObject3D :
	public BaseObject3D
{
public:
	ConeObject3D();
	~ConeObject3D();

	virtual void Create(IDirect3DDevice9* gd3dDevice, unsigned int resolution );
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection);
};

