#include "SceneNode.h"
#include "../Scene.h"
#include <cstdio>

using namespace rapidxml;

SceneNode::SceneNode()
{
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
	D3DXMatrixIdentity(&m_Translation);
	D3DXMatrixIdentity(&m_Rotation);
	D3DXMatrixIdentity(&m_Scale);
}

SceneNode::SceneNode(SceneNode* node)
{
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
	D3DXMatrixIdentity(&m_Translation);
	D3DXMatrixIdentity(&m_Rotation);
	D3DXMatrixIdentity(&m_Scale);
    m_Translation = node->m_Translation;
}

SceneNode::SceneNode(xml_node<>* node)
{
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
	D3DXMatrixIdentity(&m_Translation);
	D3DXMatrixIdentity(&m_Rotation);
	D3DXMatrixIdentity(&m_Scale);
    if(xml_node<>* translate = node->first_node("translate", 9, false)) {
        if(xml_attribute<>* atx = translate->first_attribute("x", 1, false))
            m_X = atof(atx->value());
        if(xml_attribute<>* aty = translate->first_attribute("y", 1, false))
            m_Y = atof(aty->value());
        if(xml_attribute<>* atz = translate->first_attribute("z", 1, false))
            m_Z = atof(atz->value());
    }
    if(xml_node<>* rotate = node->first_node("translate", 6, false)) {
        if(xml_attribute<>* atx = rotate->first_attribute("x", 1, false))
            m_Yaw = atof(atx->value());
        if(xml_attribute<>* aty = rotate->first_attribute("y", 1, false))
            m_Pitch = atof(aty->value());
        if(xml_attribute<>* atz = rotate->first_attribute("z", 1, false))
            m_Roll = atof(atz->value());
    }
    if(xml_node<>* scale = node->first_node("scale", 5, false)) {
        if(xml_attribute<>* atx = scale->first_attribute("x", 1, false))
            m_X = atof(atx->value());
        if(xml_attribute<>* aty = scale->first_attribute("y", 1, false))
            m_Y = atof(aty->value());
        if(xml_attribute<>* atz = scale->first_attribute("z", 1, false))
            m_Z = atof(atz->value());
    }
    UpdateMatricies();
}

void SceneNode::addChild(SceneNode* child)
{
    if(child->m_Parent != NULL)
    {
        printf("Parent found.\n");
        child->m_Parent->removeChild(child);
    }

    m_Children.push_back(child);
}

void SceneNode::removeChild(SceneNode* child)
{
    for(unsigned i = 0; i < m_Children.size(); ++i)
    {
        child->m_Parent = 0;
        m_Children.erase(m_Children.begin() + i);
    }
}

void SceneNode::renderChildren(Scene* activeScene, IDirect3DDevice9* gd3dDevice)
{
	m_World = m_Scale * m_Rotation * m_Translation;
    activeScene->pushMatrix(m_World);
    for(auto i : m_Children)
    {
        i->Render(activeScene, gd3dDevice);
        i->renderChildren(activeScene, gd3dDevice);
    }
    activeScene->popMatrix();
}

D3DXMATRIX SceneNode::getMatrix(void)
{
	m_World = m_Scale * m_Rotation * m_Translation;
    return m_World;
}

void SceneNode::UpdateMatricies()
{
	D3DXMatrixTranslation(&m_Translation, m_X, m_Y, m_Z);
	D3DXMatrixRotationYawPitchRoll(&m_Rotation, m_Yaw, m_Pitch, m_Roll);
	D3DXMatrixScaling(&m_Scale, m_sX, m_sY, m_sZ);
}
void SceneNode::Translate(float x, float y, float z, bool relative, bool rotation_relative)
{
	if (!relative) {
		D3DXMatrixTranslation(&m_Translation, x, y, z);
		m_X = x;
		m_Y = y;
		m_Z = z;
	}
	else
    {
        D3DXMATRIX trans;
		
        D3DXMatrixTranslation(&trans, x, y, z);
		
		if (rotation_relative)
		{
			trans *= m_Rotation;
			D3DXQUATERNION r;
			D3DXVECTOR3 s, t;
			D3DXMatrixDecompose(&s,&r,&t, &trans);
			D3DXMatrixIdentity(&trans);
			D3DXMatrixTranslation(&trans, t.x, t.y, t.z);
		}
		
		m_Translation *= trans;
		m_X += x;
		m_Y += y;
		m_Z += z;
    }
}

void SceneNode::SetRotationLimits(float YawMin, float YawMax, float PitchMin, float PitchMax, float RollMin, float RollMax)
{
	m_Pitch = clamp(m_Pitch, PitchMin, PitchMax);
	m_Roll = clamp(m_Roll, RollMin, RollMax);

	//D3DXMatrixRotationYawPitchRoll(&m_World, m_Yaw, m_Pitch, m_Roll);
	D3DXMatrixRotationYawPitchRoll(&m_Rotation, m_Yaw, m_Pitch, m_Roll);
}

float SceneNode::clamp(float num, float min, float max)
{
	if (num < min)
		return min;
	if (num > max)
		return max;

	return num;
}

void SceneNode::Rotate(float yaw, float pitch, float roll, bool relative)
{
	if (!relative)
	{
		D3DXMatrixRotationYawPitchRoll(&m_Rotation, yaw, pitch, roll);
		m_Yaw = yaw;
		m_Pitch = pitch;
		m_Roll = roll;
	}
    else
    {
		m_Yaw += yaw;
		m_Pitch += pitch;
		m_Roll += roll;

		D3DXMatrixRotationYawPitchRoll(&m_Rotation, m_Yaw, m_Pitch, m_Roll);
    }
}

void SceneNode::Scale(float x, float y, float z, bool relative)
{
    if(!relative)
        D3DXMatrixScaling(&m_Scale, x, y, z);
    else
    {
        D3DXMATRIX trans;

        D3DXMatrixScaling(&trans, x, y, z);
        m_Scale *= trans;
    }
}
