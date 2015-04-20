#include "LightSceneNode.h"
#include "CameraSceneNode.h"
#include "SkySceneNode.h"
#include "../Materials/BaseMaterial.h"
#include "../3DClasses/UVSphereObject3D.h"
#include "../Scene.h"
#include "../ResourceManager.h"

using namespace rapidxml;

LightSceneNode::LightSceneNode()
{
}

LightSceneNode::LightSceneNode(xml_node<>* node) : SceneNode(node)
{
	m_Material = new BaseMaterial();
    m_Type = LightType::POINT_LIGHT;
    if(xml_attribute<>* type = node->first_attribute("type", 4, false))
    {
        m_Attenuation = D3DXVECTOR4(1, 0, 0, 1);
        if(!strcmp(type->value(), "point"))
        {
            m_Type = LightType::POINT_LIGHT;
        }
        else if(!strcmp(type->value(), "spot"))
        {
            m_Type = LightType::SPOT_LIGHT;
        }
        else if(!strcmp(type->value(), "directional"))
        {
            m_Type = LightType::DIRECTIONAL_LIGHT;
            m_Attenuation = D3DXVECTOR4(0, 0, 0, 1);
        }
    }

    if(xml_attribute<>* type = node->first_attribute("energy", 6, false))
		m_Energy = (float)atof(type->value());
    
    if(xml_node<>* att = node->first_node("attenuation", 11, false))
    {
        if(xml_attribute<>* aa = att->first_attribute("a", 1, false))
            m_Attenuation.x = (float)atof(aa->value());
        if(xml_attribute<>* ab = att->first_attribute("b", 1, false))
			m_Attenuation.y = (float)atof(ab->value());
        if(xml_attribute<>* ac = att->first_attribute("c", 1, false))
			m_Attenuation.z = (float)atof(ac->value());
    }

    if(xml_node<>* color = node->first_node("color", 5, false))
    {
        if(xml_attribute<>* ar = color->first_attribute("r", 1, false))
			m_Color.x = (float)atof(ar->value());
        if(xml_attribute<>* ag = color->first_attribute("g", 1, false))
			m_Color.y = (float)atof(ag->value());
        if(xml_attribute<>* ab = color->first_attribute("b", 1, false))
			m_Color.z = (float)atof(ab->value());
        if(xml_attribute<>* aa = color->first_attribute("a", 1, false))
			m_Color.w = (float)atof(aa->value());
    }
	ID3DXEffect* effect = nullptr;
	if (xml_node<>* shader = node->first_node("shader", 6, false))
	{
		if (xml_attribute<>* shadername = shader->first_attribute("filename", 8, false))
		{
			gResourceManager->LoadEffectResource(shadername->value());
			effect = (ID3DXEffect*)gResourceManager->GetEffect(shadername->value())->GetData();
		}
	}
	if (effect == nullptr)
	{
		effect = (ID3DXEffect*)gResourceManager->getDefaultEffect()->GetData();
	}
	BaseMaterial* material = new BaseMaterial(D3DXVECTOR3(m_Color), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 0);
	if (!effect)
	{
		effect = (ID3DXEffect*)gResourceManager->getDefaultEffect()->GetData();
	}
	material->ConnectToEffect(effect);
	m_Material = material;
	m_Model = new UVSphereObject3D(0.1, 8, 8);
    m_Model->Create();
}

void LightSceneNode::Update(float deltatime)
{
}

void LightSceneNode::Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice)
{
	m_World = m_Translation * m_Rotation * m_Scale;
    D3DXMATRIX world = activeScene->getTopMatrix() * m_World;
    D3DXMATRIX view = activeScene->getView();
    D3DXMATRIX proj = activeScene->getProjection();
    LightSceneNode* light = this;
    D3DXMATRIX fc = activeScene->getActiveCamera()->getFocusView();
    m_Model->Render(world, fc, view, proj, m_Material, activeScene);
}
