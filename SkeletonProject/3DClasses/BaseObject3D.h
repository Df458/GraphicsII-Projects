//=============================================================================
//                              BaseObject3D
//
// Written by Adi Bar-Lev, 2013 - Modified by Johnathan O'Malia & Hugues Ross
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that can handle 3D rendering via Vertex and Index buffer
//=============================================================================
#ifndef _BASE_OBJECT_3D_H
#define _BASE_OBJECT_3D_H
//=============================================================================
#pragma once
//=============================================================================
#include <d3dx9.h>

#include "../d3dUtil.h"
//=============================================================================
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
class BaseMaterial;
class LightSceneNode;
//=============================================================================
class BaseObject3D
{
protected:	
	IDirect3DVertexBuffer9*     m_VertexBuffer;
	IDirect3DIndexBuffer9*      m_IndexBuffer;
	BaseMaterial*               m_Material;
    unsigned                    m_VertexCount;
    unsigned                    m_IndexCount;
    unsigned                    m_TriCount;


protected:
    // Replace the code in the following methods
	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice) = 0;
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice) = 0;
	virtual void buildUVBuffer(IDirect3DDevice9* gd3dDevice) = 0;

public:
    BaseObject3D(BaseMaterial* mat);
    virtual ~BaseObject3D(void);

	virtual void attachMaterial(BaseMaterial* mat);

    // Replace or add to the following code as you progress with the material
	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& world, D3DXMATRIX& cview, D3DXMATRIX& view, D3DXMATRIX& projection, LightSceneNode* light, IDirect3DCubeTexture9* cube);
	void Update(float deltaTime);
};
//=============================================================================
#endif // _BASE_OBJECT_3D_H

