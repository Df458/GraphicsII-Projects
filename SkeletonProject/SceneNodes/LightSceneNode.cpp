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
}

void LightSceneNode::Update(float deltatime)
{
}
