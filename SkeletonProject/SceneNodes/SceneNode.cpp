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

void SceneNode::Translate(float x, float y, float z, bool relative)
{
    if(!relative)
        D3DXMatrixTranslation(&m_World, x, y, z);
    else
    {
        D3DXMATRIX trans;

        D3DXMatrixTranslation(&trans, x, y, z);
        m_World *= trans;
    }
}

void SceneNode::Rotate(float yaw, float pitch, float roll, bool relative)
{
    if(!relative)
        D3DXMatrixRotationYawPitchRoll(&m_World, yaw, pitch, roll);
    else
    {
        D3DXMATRIX trans;

        D3DXMatrixRotationYawPitchRoll(&trans, yaw, pitch, roll);
        m_World *= trans;
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
