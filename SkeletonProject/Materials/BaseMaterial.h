//=============================================================================
//                              BaseMaterial
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#pragma once
#include "../d3dApp.h"
#include <rapidxml.hpp>
//=============================================================================

class LightSceneNode;

class BaseMaterial
{
protected:
    ID3DXEffect*        m_Effect;               // the shader associate effect file
    int id;

    //-------- Material Parameters -------
    D3DXMATRIX          m_WorldMat;
    D3DXMATRIX          m_ITWorldMat;
    D3DXMATRIX          m_ViewProjectionMat;

    D3DXVECTOR4         m_AmbientColor;
    D3DXVECTOR4         m_DiffuseColor;
    D3DXVECTOR4         m_SpecularColor;
    float               m_Shininess;            // specular power

	int					ToggleDiffuse;
	int					ToggleSpecular;
	int					ToggleWire;
	int					Gouraud = 1;
	std::string			tech = "Gouraud";

    //---------- Shader Handles ----------
    // Generic shader handles
    D3DXHANDLE          m_WorldMatHandle = 0;
	D3DXHANDLE          m_ITWorldMatHandle = 0;
	D3DXHANDLE          m_ViewProjectionMatHandle = 0;

	D3DXHANDLE          m_LightPosWHandle = 0;       // Position (spot/point) / Direction (directional)
	D3DXHANDLE          m_LightColorHandle = 0;
	D3DXHANDLE          m_ViewerPosWHandle = 0;

    // Material specific shader handles
	D3DXHANDLE          m_AmbientColHandle = 0;
	D3DXHANDLE          m_DiffuseColHandle = 0;
	D3DXHANDLE          m_SpecularColHandle = 0;
	D3DXHANDLE          m_ShininessHandle = 0;
	D3DXHANDLE          m_AttenuationHandle = 0;

	D3DXHANDLE			ToggleSpecularHandle = 0;
	D3DXHANDLE			ToggleDiffuseHandle = 0;

	//Texture Thngs
	int					ToggleTexture;
	IDirect3DTexture9*  m_Texture;
	D3DXHANDLE          m_TextureHandle;
	D3DXHANDLE			ToggleTextureHandle = 0;


	D3DXHANDLE          m_Technique = 0;

public:
    BaseMaterial(D3DXVECTOR3 amb = D3DXVECTOR3(0.1f, 0.1f, 0.1f), D3DXVECTOR3 diff = D3DXVECTOR3(0.9f, 0.9f, 0.9f), D3DXVECTOR3 spec = D3DXVECTOR3(1.0f, 1.0f, 1.0f), float shine = 0.1f);
	BaseMaterial(const char* texture_name, D3DXVECTOR3 amb = D3DXVECTOR3(0.1f, 0.1f, 0.1f), D3DXVECTOR3 diff = D3DXVECTOR3(0.9f, 0.9f, 0.9f), D3DXVECTOR3 spec = D3DXVECTOR3(1.0f, 1.0f, 1.0f), float shine = 0.1f);
	BaseMaterial(rapidxml::xml_node<>* node);
    virtual ~BaseMaterial(void);

    virtual void ConnectToEffect( ID3DXEffect* effect );
    virtual unsigned PreRender(void);
    virtual void Render( D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR4 viewer_pos, unsigned pass, LightSceneNode* light ); 
    virtual void PostPass(void);
    virtual void PostRender(void);

	void DEBUGTOGGLETEXTURE();
	void DEBUGTOGGLESPECULAR();
	void DEBUGTOGGLEDIFFUSE();
	void DEBUGTOGGLEWIREFRAME();
	void DEBUGTOGGLETYPE();

	void updateTech();
};
//=============================================================================

