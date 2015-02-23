#include "../d3dUtil.h"
#include "ModelSceneNode.h"
#include "../3DClasses/MeshObject3D.h"

ModelSceneNode::ModelSceneNode(MeshObject3D* model)
{
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    m_Model = model;
}

ModelSceneNode::ModelSceneNode(MeshObject3D* model, float x, float y, float z, float yaw, float pitch, float roll)
{
	m_Parent = NULL;
	D3DXMatrixIdentity(&m_World);
	m_Model = model;

	m_X = x;
	m_Y = y;
	m_Z = z;

	m_Yaw = yaw;
	m_Pitch = pitch;
	m_Roll = roll;

	UpdateMatricies();
}

void ModelSceneNode::Update(float deltatime)
{
	m_World = m_Scale * m_Rotation * m_Translation;
    m_Model->Update(deltatime);
}

void ModelSceneNode::Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice)
{
	m_World = m_Scale * m_Rotation * m_Translation;
    D3DXMATRIX world = activeScene->getTopMatrix() * m_World;
    D3DXMATRIX view = activeScene->getView();
    D3DXMATRIX proj = activeScene->getProjection();
    m_Model->Render(gd3dDevice, world, view, proj);
}
