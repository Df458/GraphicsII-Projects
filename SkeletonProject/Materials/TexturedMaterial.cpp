//=============================================================================
//                              TexturedMaterial
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#include "TexturedMaterial.h"
#include "../SceneNodes/LightSceneNode.h"
//=============================================================================
using namespace rapidxml;

TexturedMaterial::TexturedMaterial(const char* name, D3DXVECTOR3 amb, D3DXVECTOR3 diff, D3DXVECTOR3 spec, float shine)
: m_AmbientColor(amb), m_DiffuseColor(diff), m_SpecularColor(spec), m_Shininess(shine)
{
    m_Effect = NULL;
    HR(D3DXCreateTextureFromFile(gd3dDevice, name, &m_Texture));
}

TexturedMaterial::TexturedMaterial(rapidxml::xml_node<>* node)
{
    if(xml_attribute<>* shine = node->first_attribute("shine", 5, false))
        m_Shininess = atof(shine->value());

    if(xml_attribute<>* tex = node->first_attribute("texture", 7, false))
    {
        HR(D3DXCreateTextureFromFile(gd3dDevice, tex->value(), &m_Texture));
    }
    else
        fprintf(stderr, "Warning: trying to create a textured material with no texture!\n");

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
TexturedMaterial::~TexturedMaterial(void)
{
}

//-----------------------------------------------------------------------------
// Need to add here a code that will associate with your shader parameters and 
// register them.
void TexturedMaterial::ConnectToEffect( ID3DXEffect* effect )
{
    m_Effect = effect;
    if(!m_Effect)
        printf("Error: trying to connect a null effect\n");
    m_WorldMatHandle = effect->GetParameterByName(0, "matWorld");
    m_ITWorldMatHandle = effect->GetParameterByName(0, "matITWorld");
    m_ViewProjectionMatHandle = effect->GetParameterByName(0, "matVP");

    m_LightPosWHandle = effect->GetParameterByName(0, "vLightPos");
    m_ViewerPosWHandle = effect->GetParameterByName(0, "vViewPos");

    m_LightColorHandle = effect->GetParameterByName(0, "colLight");
    m_AmbientColHandle = effect->GetParameterByName(0, "colAmbient");
    m_DiffuseColHandle = effect->GetParameterByName(0, "colDiffuse");
    m_SpecularColHandle = effect->GetParameterByName(0, "colSpecular");
    m_ShininessHandle = effect->GetParameterByName(0, "valShininess");
    m_AttenuationHandle = effect->GetParameterByName(0, "vAttenuation");
    m_TextureHandle = effect->GetParameterByName(0, "Texture");

    m_Technique = m_Effect->GetTechniqueByName("GouraudSolid");
}

//=============================================================================

void TexturedMaterial::Render(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR4 viewer_pos, unsigned pass, LightSceneNode* light)
{
    if(!light)
    {
        fprintf(stderr, "Warning: Trying to render with a NULL light.\n");
        return;
    }

    D3DXMatrixInverse(&m_ITWorldMat, 0, &worldMat);
    D3DXMatrixTranspose(&m_ITWorldMat, &m_ITWorldMat);
    D3DXVECTOR4 pos = light->getTranslation();
    D3DXVECTOR4 light_color = light->getColor();
    D3DXVECTOR4 lightatt = light->getAttenuation();
    HR(m_Effect->SetMatrix(m_WorldMatHandle, &worldMat));
    HR(m_Effect->SetMatrix(m_ITWorldMatHandle, &m_ITWorldMat));
    HR(m_Effect->SetMatrix(m_ViewProjectionMatHandle, &viewProjMat));
    HR(m_Effect->SetVector(m_ViewerPosWHandle, &viewer_pos));
    HR(m_Effect->SetVector(m_AmbientColHandle, &m_AmbientColor));
    HR(m_Effect->SetVector(m_DiffuseColHandle, &m_DiffuseColor));
    HR(m_Effect->SetVector(m_SpecularColHandle, &m_SpecularColor));
    HR(m_Effect->SetVector(m_LightPosWHandle, &pos));
    HR(m_Effect->SetVector(m_LightColorHandle, &light_color));
    HR(m_Effect->SetVector(m_AttenuationHandle, &lightatt));
    HR(m_Effect->SetFloat(m_ShininessHandle, m_Shininess));
    HR(m_Effect->SetTexture(m_TextureHandle, m_Texture));
    HR(m_Effect->CommitChanges());
    HR(m_Effect->BeginPass(pass));
}
