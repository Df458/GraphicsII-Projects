#pragma once
#include "BaseObject3D.h"
class UVSphereObject3D :
	public BaseObject3D
{
public:
	UVSphereObject3D();
	~UVSphereObject3D();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection);
};

