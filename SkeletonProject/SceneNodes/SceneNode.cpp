#include "SceneNode.h"
#include "../Scene.h"
#include <cstdio>

SceneNode::SceneNode()
{
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
}

void SceneNode::addChild(SceneNode* child)
{
    printf("Checking parent...\n");
    if(child->m_Parent != NULL)
    {
        printf("Parent found.\n");
        child->m_Parent->removeChild(child);
    }

    printf("Adding child...\n");
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
    activeScene->pushMatrix(m_World);
    for(auto i : m_Children)
    {
        i->Render(activeScene, gd3dDevice);
        i->renderChildren(activeScene, gd3dDevice);
    }
    activeScene->popMatrix();
}

void SceneNode::Translate(float x, float y, float z, bool relative, bool rotation_relative)
{
	if (!relative) {
		D3DXMatrixTranslation(&m_World, x, y, z);
	}
	else
    {
        D3DXMATRIX trans;
		
        D3DXMatrixTranslation(&trans, x, y, z);
		
		if (rotation_relative)
		{
			trans *= m_Rotation;
			D3DXMatrixRotationYawPitchRoll(&m_World, 0, 0, 0);
		}
		
		m_World *= trans;
    }
}

void SceneNode::SetRotationLimits(float YawMin, float YawMax, float PitchMin, float PitchMax, float RollMin, float RollMax)
{
	m_Pitch = clamp(m_Pitch, PitchMin, PitchMax);
	m_Roll = clamp(m_Roll, RollMin, RollMax);

	if (m_Yaw < YawMin)
	{
		m_Yaw += YawMax;
	}

	if (m_Yaw > YawMax)
	{
		m_Yaw -= YawMax;
	}

	D3DXMatrixRotationYawPitchRoll(&m_World, m_Yaw, m_Pitch, m_Roll);
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
		D3DXMatrixRotationYawPitchRoll(&m_World, yaw, pitch, roll);
		m_Yaw = yaw;
		m_Pitch = pitch;
		m_Roll = roll;
	}
    else
    {
		m_Yaw += yaw;
		m_Pitch += pitch;
		m_Roll += roll;

		D3DXMatrixRotationYawPitchRoll(&m_World, m_Yaw, m_Pitch, m_Roll);
    }
}

void SceneNode::Scale(float x, float y, float z, bool relative)
{
    if(!relative)
        D3DXMatrixScaling(&m_World, x, y, z);
    else
    {
        D3DXMATRIX trans;

        D3DXMatrixScaling(&trans, x, y, z);
        m_World *= trans;
    }
}
