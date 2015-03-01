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

    //-------- Material Parameters -------
    D3DXMATRIX          m_WorldMat;
    D3DXMATRIX          m_ITWorldMat;
    D3DXMATRIX          m_ViewProjectionMat;

    D3DXVECTOR4         m_AmbientColor;
    D3DXVECTOR4         m_DiffuseColor;
    D3DXVECTOR4         m_SpecularColor;
    float               m_Shininess;            // specular power


    //---------- Shader Handles ----------
    // Generic shader handles
    D3DXHANDLE          m_WorldMatHandle;
    D3DXHANDLE          m_ITWorldMatHandle;
    D3DXHANDLE          m_ViewProjectionMatHandle;

    D3DXHANDLE          m_LightPosWHandle;       // Position (spot/point) / Direction (directional)
    D3DXHANDLE          m_LightColorHandle;
    D3DXHANDLE          m_ViewerPosWHandle;

    // Material specific shader handles
    D3DXHANDLE          m_AmbientColHandle;
    D3DXHANDLE          m_DiffuseColHandle;
    D3DXHANDLE          m_SpecularColHandle;
    D3DXHANDLE          m_ShininessHandle;
    D3DXHANDLE          m_AttenuationHandle;

    D3DXHANDLE          m_Technique;

public:
    BaseMaterial(D3DXVECTOR3 amb = D3DXVECTOR3(0.1f, 0.1f, 0.1f), D3DXVECTOR3 diff = D3DXVECTOR3(0.9f, 0.9f, 0.9f), D3DXVECTOR3 spec = D3DXVECTOR3(1.0f, 1.0f, 1.0f), float shine = 0.1f);
    BaseMaterial(rapidxml::xml_node<>* node);
    virtual ~BaseMaterial(void);

    void ConnectToEffect( ID3DXEffect* effect );
    unsigned PreRender(void);
    void Render( D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR4 viewer_pos, unsigned pass, LightSceneNode* light ); 
    void PostPass(void);
    void PostRender(void);
};
//=============================================================================

