#include "LightSceneNode.h"
#include "../Materials/BaseMaterial.h"
#include "../3DClasses/UVSphereObject3D.h"

using namespace rapidxml;

LightSceneNode::LightSceneNode()
{
}

LightSceneNode::LightSceneNode(xml_node<>* node, ID3DXEffect* effect) : SceneNode(node)
{
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
        m_Energy = atof(type->value());
    
    if(xml_node<>* att = node->first_node("attenuation", 11, false))
    {
        if(xml_attribute<>* aa = att->first_attribute("a", 1, false))
            m_Attenuation.x = atof(aa->value());
        if(xml_attribute<>* ab = att->first_attribute("b", 1, false))
            m_Attenuation.y = atof(ab->value());
        if(xml_attribute<>* ac = att->first_attribute("c", 1, false))
            m_Attenuation.z = atof(ac->value());
    }

    if(xml_node<>* color = node->first_node("color", 5, false))
    {
        if(xml_attribute<>* ar = color->first_attribute("r", 1, false))
            m_Color.x = atof(ar->value());
        if(xml_attribute<>* ag = color->first_attribute("g", 1, false))
            m_Color.y = atof(ag->value());
        if(xml_attribute<>* ab = color->first_attribute("b", 1, false))
            m_Color.z = atof(ab->value());
        if(xml_attribute<>* aa = color->first_attribute("a", 1, false))
            m_Color.w = atof(aa->value());
    }

    m_Model = new UVSphereObject3D(0.1, 8, 8, new BaseMaterial(D3DXVECTOR3(m_Color), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 0), effect);
    m_Model->Create(gd3dDevice);
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
    m_Model->Render(gd3dDevice, world, view, proj, light);
}
