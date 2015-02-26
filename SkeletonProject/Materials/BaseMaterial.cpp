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
using namespace rapidxml;

BaseMaterial::BaseMaterial(D3DXVECTOR3 amb, D3DXVECTOR3 diff, D3DXVECTOR3 spec, float shine)
: m_AmbientColor(amb), m_DiffuseColor(diff), m_SpecularColor(spec), m_Shininess(shine)
{
    m_Effect = NULL;
}

BaseMaterial::BaseMaterial(rapidxml::xml_node<>* node)
{
    m_Effect = NULL;

    if(xml_attribute<>* shine = node->first_attribute("shine", 5, false))
        m_Shininess = atof(shine->value());

    if(xml_node<>* color = node->first_node("ambient", 7, false))
    {
        if(xml_attribute<>* ar = color->first_attribute("r", 1, false))
            m_AmbientColor.x = atof(ar->value());
        if(xml_attribute<>* ag = color->first_attribute("g", 1, false))
            m_AmbientColor.y = atof(ag->value());
        if(xml_attribute<>* ab = color->first_attribute("b", 1, false))
            m_AmbientColor.z = atof(ab->value());
        if(xml_attribute<>* aa = color->first_attribute("a", 1, false))
            m_AmbientColor.w = atof(aa->value());
    }

    if(xml_node<>* color = node->first_node("diffuse", 7, false))
    {
        if(xml_attribute<>* ar = color->first_attribute("r", 1, false))
            m_DiffuseColor.x = atof(ar->value());
        if(xml_attribute<>* ag = color->first_attribute("g", 1, false))
            m_DiffuseColor.y = atof(ag->value());
        if(xml_attribute<>* ab = color->first_attribute("b", 1, false))
            m_DiffuseColor.z = atof(ab->value());
        if(xml_attribute<>* aa = color->first_attribute("a", 1, false))
            m_DiffuseColor.w = atof(aa->value());
    }

    if(xml_node<>* color = node->first_node("specular", 8, false))
    {
        if(xml_attribute<>* ar = color->first_attribute("r", 1, false))
            m_SpecularColor.x = atof(ar->value());
        if(xml_attribute<>* ag = color->first_attribute("g", 1, false))
            m_SpecularColor.y = atof(ag->value());
        if(xml_attribute<>* ab = color->first_attribute("b", 1, false))
            m_SpecularColor.z = atof(ab->value());
        if(xml_attribute<>* aa = color->first_attribute("a", 1, false))
            m_SpecularColor.w = atof(aa->value());
    }
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
    m_ITWorldMatHandle = effect->GetParameterByName(0, "matITWorld");
    m_ViewProjectionMatHandle = effect->GetParameterByName(0, "matVP");

    m_LightPosWHandle = effect->GetParameterByName(0, "vLightPos");
    m_ViewerPosWHandle = effect->GetParameterByName(0, "vViewPos");

    m_AmbientColHandle = effect->GetParameterByName(0, "colAmbient");
    m_DiffuseColHandle = effect->GetParameterByName(0, "colDiffuse");
    m_SpecularColHandle = effect->GetParameterByName(0, "colSpecular");
    m_ShininessHandle = effect->GetParameterByName(0, "valShininess");

    m_Technique = m_Effect->GetTechniqueByName("GouraudWire");
}

//=============================================================================

unsigned BaseMaterial::PreRender(void) {
    HR(m_Effect->SetTechnique(m_Technique));
    unsigned passes;
    HR(m_Effect->Begin(&passes, 0));
    return passes;
}

void BaseMaterial::Render(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, unsigned pass)
{
    D3DXMatrixInverse(&m_ITWorldMat, 0, &worldMat);
    D3DXMatrixTranspose(&m_ITWorldMat, &m_ITWorldMat);
    D3DXVECTOR4 pos = D3DXVECTOR4(0.0, 1.0, -1.2, 1.0);
    HR(m_Effect->SetMatrix(m_WorldMatHandle, &worldMat));
    HR(m_Effect->SetMatrix(m_ITWorldMatHandle, &m_ITWorldMat));
    HR(m_Effect->SetMatrix(m_ViewProjectionMatHandle, &viewProjMat));
    HR(m_Effect->SetVector(m_AmbientColHandle, &m_AmbientColor));
    HR(m_Effect->SetVector(m_DiffuseColHandle, &m_DiffuseColor));
    HR(m_Effect->SetVector(m_SpecularColHandle, &m_SpecularColor));
    HR(m_Effect->SetVector(m_LightPosWHandle, &pos));
    HR(m_Effect->SetFloat(m_ShininessHandle, m_Shininess));
    HR(m_Effect->CommitChanges());
    HR(m_Effect->BeginPass(pass));
}

void BaseMaterial::PostPass(void)
{
    HR(m_Effect->EndPass());
}

void BaseMaterial::PostRender(void)
{
    HR(m_Effect->End());
}
