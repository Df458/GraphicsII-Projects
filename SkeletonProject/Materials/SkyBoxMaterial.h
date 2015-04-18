#pragma once
#include "BaseMaterial.h"

class Scene;

class SkyBoxMaterial : public BaseMaterial
{
    D3DXMATRIX          m_VPMat;
    D3DXHANDLE          m_VPMatHandle;
	D3DXHANDLE			m_viewHandle;
	IDirect3DCubeTexture9*  m_CubeTexture;
public:
	SkyBoxMaterial(ID3DXEffect* pEffect, IDirect3DCubeTexture9* pTexture);
    virtual void ConnectToEffect( ID3DXEffect* effect);
	virtual void Render(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR4 viewer_pos, unsigned pass, Scene* scene);
	IDirect3DCubeTexture9* getSkyTexture() { return m_CubeTexture; }
};
