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
#include "../SceneNodes/LightSceneNode.h"
#include "../SceneNodes/SkySceneNode.h"
#include "../SceneNodes/CameraSceneNode.h"
#include "../ResourceManager.h"
#include "../Scene.h"
//=============================================================================
using namespace rapidxml;

BaseMaterial::BaseMaterial(D3DXVECTOR3 amb, D3DXVECTOR3 diff, D3DXVECTOR3 spec, float shine)
: m_AmbientColor(amb), m_DiffuseColor(diff), m_SpecularColor(spec), m_Shininess(shine)
{
	ToggleTexture = 0;
	ConnectToEffect((ID3DXEffect*)gResourceManager->getDefaultEffect()->GetData());
	m_Texture = nullptr;
	m_Normal = nullptr;
	ToggleNormal = 0;
	ToggleReflection = 0;
	ToggleWire = 0;

	AmbientCoefficient = 0.2f;
	DiffuseCoefficient = 0.65f;
	SpecularCoefficient = 0.15f;
	ReflectionCoefficient = 0.1f;
	NormalStrength = 1.0f;
	SpecularPower = 8;

}

BaseMaterial::BaseMaterial(const char* name, D3DXVECTOR3 amb, D3DXVECTOR3 diff, D3DXVECTOR3 spec, float shine)
: BaseMaterial(amb, diff, spec, shine)
{
	ToggleTexture = 1;
	ConnectToEffect((ID3DXEffect*)gResourceManager->getDefaultEffect()->GetData());
	ToggleNormal = 1;
	ToggleReflection = 1;
	ToggleWire = 0;
	m_Texture = (IDirect3DTexture9*)gResourceManager->GetTexture(name)->GetData();
	m_Normal = nullptr;

	AmbientCoefficient = 0.2f;
	DiffuseCoefficient = 0.65f;
	SpecularCoefficient = 0.15f;
	ReflectionCoefficient = 0.1f;
	NormalStrength = 1.0f;
	SpecularPower = 8;

}

BaseMaterial::BaseMaterial(IDirect3DTexture9* pTexture, ID3DXEffect* effect)
{
	ToggleTexture = 1;
	ToggleNormal = 1;
	ToggleReflection = 1;
	ToggleWire = 0;

	AmbientCoefficient = 0.2f;
	DiffuseCoefficient = 0.65f;
	SpecularCoefficient = 0.15f;
	ReflectionCoefficient = 0.1f;
	NormalStrength = 1.0f;
	SpecularPower = 8;

	m_Texture = pTexture;
	ConnectToEffect(effect);
}


BaseMaterial::BaseMaterial(rapidxml::xml_node<>* node) : BaseMaterial()
{
	ToggleTexture = 1;
	ToggleNormal = 1;
	ToggleReflection = 1;
	ToggleWire = 0;
	
	AmbientCoefficient = 0.2f;
	DiffuseCoefficient = 0.65f;
	SpecularCoefficient = 0.15f;
	ReflectionCoefficient = 0.1f;
	NormalStrength = 1.0f;
	SpecularPower = 8;


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

	if (xml_attribute<>* tex = node->first_attribute("texture", 7, false))
	{
		gResourceManager->LoadTextureResource(tex->value());
		m_Texture = (IDirect3DTexture9*)gResourceManager->GetTexture(tex->value())->GetData();
	}
	else
	{
		fprintf(stderr, "Warning: trying to create a textured material with no texture!\n");

		ToggleTexture = 0;
	}

	if (xml_attribute<>* tex = node->first_attribute("normal", 6, false))
	{
		gResourceManager->LoadTextureResource(tex->value());
		m_Normal = (IDirect3DTexture9*)gResourceManager->GetTexture(tex->value())->GetData();
	}

	if (xml_node<>* shader = node->first_node("shader", 6, false))
	{
		if (xml_attribute<>* shadername = shader->first_attribute("filename", 8, false))
		{
			gResourceManager->LoadEffectResource(shadername->value());
			m_Effect = (ID3DXEffect*)gResourceManager->GetEffect(shadername->value())->GetData();
		}
	}
	else if (m_Effect == nullptr)
	{
		m_Effect = (ID3DXEffect*)gResourceManager->getDefaultEffect()->GetData();
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
	ConnectToEffect(m_Effect);
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
    if(!m_Effect)
        printf("Error: trying to connect a null effect\n");
    m_WorldMatHandle			= effect->GetParameterByName(0, "matWorld");
    m_ITWorldMatHandle			= effect->GetParameterByName(0, "matITWorld");
    m_ViewProjectionMatHandle	= effect->GetParameterByName(0, "matVP");

    m_LightPosWHandle			= effect->GetParameterByName(0, "vLightPos");
    m_ViewerPosWHandle			= effect->GetParameterByName(0, "vViewPos");

    m_LightColorHandle			= effect->GetParameterByName(0, "colLight");
    m_AmbientColHandle			= effect->GetParameterByName(0, "colAmbient");
    m_DiffuseColHandle			= effect->GetParameterByName(0, "colDiffuse");
    m_SpecularColHandle			= effect->GetParameterByName(0, "colSpecular");
    m_ShininessHandle			= effect->GetParameterByName(0, "valShininess");
    m_AttenuationHandle			= effect->GetParameterByName(0, "vAttenuation");

	m_TextureHandle				= effect->GetParameterByName(0, "Texture");
	m_NormalHandle				= effect->GetParameterByName(0, "NormalTexture");
	m_SkyTextureHandle			= effect->GetParameterByName(0, "SkyTexture");

	ToggleTextureHandle			= effect->GetParameterByName(0, "ToggleTexture");
	ToggleReflectionHandle		= effect->GetParameterByName(0, "ToggleReflection");
	ToggleNormalHandle			= effect->GetParameterByName(0, "ToggleNormal");
	SpecularPowerHandle			= effect->GetParameterByName(0, "SpecularPower");
	AmbientCoefficientHandle	= effect->GetParameterByName(0, "AmbientCoef");
	DiffuseCoefficientHandle	= effect->GetParameterByName(0, "DiffuseCoef");
	SpecularCoefficientHandle	= effect->GetParameterByName(0, "SpecularCoef");
	ReflectionCoefficientHandle = effect->GetParameterByName(0, "ReflectionCoef");
	NormalStrengthHandle		= effect->GetParameterByName(0, "NormalStr");


    m_Technique = m_Effect->GetTechniqueByName("PhongSolid");
}

//=============================================================================

unsigned BaseMaterial::PreRender(void)
{
    HR(m_Effect->SetTechnique(m_Technique));
    unsigned passes;
    HR(m_Effect->Begin(&passes, 0));
    return passes;
}

void BaseMaterial::Render(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR4 viewer_pos, unsigned pass, Scene* scene)
{
	if (m_Billboard)
		worldMat *= scene->getActiveCamera()->getBillboardMatrix();
	LightSceneNode* light = scene->getActiveLight();
	IDirect3DCubeTexture9* cube = NULL;
	if(SkySceneNode* sky = scene->getActiveSky())
		cube = sky->getSkyTexture();
	if (!light || !cube)
	{
		fprintf(stderr, "Warning: Trying to render with a NULL light/sky.\n");
		HR(m_Effect->BeginPass(pass));
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
	HR(m_Effect->SetInt(ToggleNormalHandle, ToggleNormal));
	HR(m_Effect->SetInt(ToggleReflectionHandle, ToggleReflection));
	HR(m_Effect->SetInt(ToggleTextureHandle, ToggleTexture));
	HR(m_Effect->SetInt(SpecularPowerHandle, SpecularPower));
	HR(m_Effect->SetFloat(AmbientCoefficientHandle, AmbientCoefficient));
	HR(m_Effect->SetFloat(DiffuseCoefficientHandle, DiffuseCoefficient));
	HR(m_Effect->SetFloat(SpecularCoefficientHandle, SpecularCoefficient));
	HR(m_Effect->SetFloat(ReflectionCoefficientHandle, ReflectionCoefficient));
	HR(m_Effect->SetFloat(NormalStrengthHandle, NormalStrength));
	if (m_Texture != nullptr)
		HR(m_Effect->SetTexture(m_TextureHandle, m_Texture));
	if (m_Normal)
		HR(m_Effect->SetTexture(m_NormalHandle, m_Normal));
	if (cube)
		HR(m_Effect->SetTexture(m_SkyTextureHandle, cube));
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


void BaseMaterial::DEBUGTOGGLETEXTURE()
{
	if (ToggleTexture == 1)
		ToggleTexture = 0;
	else
		ToggleTexture = 1;
}

void BaseMaterial::DEBUGTOGGLENORMAL()
{
	if (ToggleNormal == 1)
		ToggleNormal = 0;
	else
		ToggleNormal = 1;
}

void BaseMaterial::DEBUGTOGGLEREFLECTION()
{
	if (ToggleReflection == 1)
		ToggleReflection = 0;
	else
		ToggleReflection = 1;
}

void BaseMaterial::DEBUGTOGGLEWIREFRAME()
{
	if (ToggleWire == 1) {
		ToggleWire = 0;
	}
	else{
		ToggleWire = 1;
	}
	updateTech();
}

void BaseMaterial::DEBUGBLENDSPECULARREFLECTION(float increment)
{
	SpecularCoefficient += increment;
	ReflectionCoefficient -= increment;
}

void BaseMaterial::DEBUGSPECULARPOWER(int power)
{
	SpecularPower = 2 ^ power;
}

void BaseMaterial::DEBUGNORMALSTRENGTH(float increment)
{
	NormalStrength += increment;
}

void BaseMaterial::updateTech()
{
	if (ToggleWire == 0)
		m_Technique = m_Effect->GetTechniqueByName((tech + "Solid").c_str());
	else
		m_Technique = m_Effect->GetTechniqueByName((tech + "Wire").c_str());
}
