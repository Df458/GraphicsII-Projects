#include "LightSceneNode.h"

using namespace rapidxml;

LightSceneNode::LightSceneNode()
{
}

LightSceneNode::LightSceneNode(xml_node<>* node) : SceneNode(node)
{
    m_Type = LightType::POINT_LIGHT;
    if(xml_attribute<>* type = node->first_attribute("type", 4, false))
    {
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
        }
    }

    if(xml_attribute<>* type = node->first_attribute("energy", 6, false))
        m_Energy = atof(type->value());

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
}

void LightSceneNode::Update(float deltatime)
{
}
