#pragma once
#include "BaseObject3D.h"
class IcoSphereObject3D :
	public BaseObject3D
{
public:
	IcoSphereObject3D();
	~IcoSphereObject3D();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection);
};

