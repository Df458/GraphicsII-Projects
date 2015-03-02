//=============================================================================
//                              TexturedMaterial
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#pragma once
#include "../d3dApp.h"
#include "BaseMaterial.h"
#include <rapidxml.hpp>
//=============================================================================

class LightSceneNode;

class TexturedMaterial : public BaseMaterial
{
protected:
	int					ToggleTexture;
    IDirect3DTexture9*  m_Texture;
    D3DXHANDLE          m_TextureHandle;
	D3DXHANDLE			ToggleTextureHandle;

public:
    TexturedMaterial(const char* texture_name, D3DXVECTOR3 amb = D3DXVECTOR3(0.1f, 0.1f, 0.1f), D3DXVECTOR3 diff = D3DXVECTOR3(0.9f, 0.9f, 0.9f), D3DXVECTOR3 spec = D3DXVECTOR3(1.0f, 1.0f, 1.0f), float shine = 0.1f);
    TexturedMaterial(rapidxml::xml_node<>* node);
    virtual ~TexturedMaterial(void);

    void ConnectToEffect( ID3DXEffect* effect );
    void Render( D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR4 viewer_pos, unsigned pass, LightSceneNode* light ); 
};
//=============================================================================

