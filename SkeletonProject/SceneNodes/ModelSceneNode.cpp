#include "../d3dUtil.h"
#include "ModelSceneNode.h"
#include "../3DClasses/BaseObject3D.h"

ModelSceneNode::ModelSceneNode(BaseObject3D* model)
{
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    m_Model = model;
}

void ModelSceneNode::Update(float deltatime)
{
    m_Model->Update(deltatime);
}

void ModelSceneNode::Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice)
{
    D3DXMATRIX world = activeScene->getTopMatrix() * m_World;
    D3DXMATRIX view = activeScene->getView();
    D3DXMATRIX proj = activeScene->getProjection();
    m_Model->Render(gd3dDevice, world, view, proj);
}
