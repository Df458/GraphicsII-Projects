//=============================================================================
//                              BaseMaterial
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#include "BaseMaterial.h"
//=============================================================================
BaseMaterial::BaseMaterial(void)
{
    m_Effect = NULL;
}

//-----------------------------------------------------------------------------
// Release shader, blah...
BaseMaterial::~BaseMaterial(void)
{
}

//-----------------------------------------------------------------------------
// Need to add here a code that will associate with your shader parameters and 
// register them.
void BaseMaterial::ConnectToEffect( ID3DXEffect* effect )
{
    m_Effect = effect;
    m_WorldMatHandle = effect->GetParameterByName(0, "matWorld");
    m_ViewProjectionMatHandle = effect->GetParameterByName(0, "matVP");

    m_LightPosWHandle = effect->GetParameterByName(0, "vLightPos");
    m_ViewerPosWHandle = effect->GetParameterByName(0, "vViewPos");

    m_AmbientColHandle = effect->GetParameterByName(0, "colAmbient");
    m_DiffuseColHandle = effect->GetParameterByName(0, "colDiffuse");
    m_SpecularColHandle = effect->GetParameterByName(0, "colSpecular");
    m_ShininessHandle = effect->GetParameterByName(0, "valShininess");
}

//=============================================================================

void BaseMaterial::Render(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat)
{
    HR(m_Effect->Begin(1, 0));
    HR(m_Effect->SetMatrix(m_WorldMatHandle, &worldMat));
    HR(m_Effect->SetMatrix(m_ViewProjectionMatHandle, &viewProjMat));
    HR(m_Effect->SetVector(m_AmbientColHandle, &m_AmbientColor));
    HR(m_Effect->SetVector(m_DiffuseColHandle, &m_DiffuseColor));
    HR(m_Effect->SetVector(m_SpecularColHandle, &m_SpecularColor));
    HR(m_Effect->SetValue(m_ShininessHandle, &m_Shininess));
    HR(m_Effect->CommitChanges());
    HR(m_Effect->BeginPass(0));
}

void BaseMaterial::PostRender()
{
    HR(m_Effect->EndPass()
    HR(m_Effect->End());
}
