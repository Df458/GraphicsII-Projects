#include "../d3dUtil.h"
#include "CameraSceneNode.h"
#include "ParticleEmitterNode.h"
#include "../3DClasses/XModel3D.h"
#include "../3DClasses/MeshObject3D.h"
#include "../Materials/BaseMaterial.h"
#include "../Scene.h"
#include "../ResourceManager.h"

using namespace rapidxml;
// TODO: Write the contents of this class
ParticleEmitterNode::ParticleEmitterNode(MeshObject3D* model)
{
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    m_Model = model;
}

ParticleEmitterNode::ParticleEmitterNode(MeshObject3D* model, float x, float y, float z, float yaw, float pitch, float roll)
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

ParticleEmitterNode::ParticleEmitterNode(xml_node<>* node) : SceneNode(node)
{
    BaseMaterial* mat;
	ID3DXEffect* effect = nullptr;
	IDirect3DTexture9* texture = nullptr;

	if (rapidxml::xml_node<>* shader_node = node->first_node("shader", 6, false))
	{
		if (rapidxml::xml_attribute<>* shadername = shader_node->first_attribute("filename", 8, false))
		{
			gResourceManager->LoadEffectResource(shadername->value());
			effect = (ID3DXEffect*)gResourceManager->GetEffect(shadername->value())->GetData();
		}
	}

    if(xml_node<>* nmat = node->first_node("material", 8, false))
    {
		mat = new BaseMaterial(nmat);
    }
	else
	{
		effect = (ID3DXEffect*)gResourceManager->getDefaultEffect()->GetData();
		texture = (IDirect3DTexture9*)gResourceManager->getDefaultTexture()->GetData();
		mat = new BaseMaterial(texture, effect);
	}
	m_Material = mat;
	m_Material->makeBillboard();

	if (effect == nullptr)
	{
		effect = (ID3DXEffect*)gResourceManager->getDefaultEffect()->GetData();
	}

	gResourceManager->LoadMeshResource("plane.x");
	m_Model = new XModel3D((LPD3DXMESH)gResourceManager->GetMesh("plane.x")->GetData());
}

ParticleEmitterNode::~ParticleEmitterNode()
{
    delete m_Model;
}

BaseMaterial* ParticleEmitterNode::getMaterial(void)
{ 
	return m_Material; 
}

void ParticleEmitterNode::Update(float deltatime)
{
}

void ParticleEmitterNode::Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice)
{
	m_World = m_Scale * m_Rotation * m_Translation;
    D3DXMATRIX world = m_World * activeScene->getTopMatrix();
    D3DXMATRIX view = activeScene->getView();
    D3DXMATRIX proj = activeScene->getProjection();
    D3DXMATRIX fc = activeScene->getActiveCamera()->getFocusView();
	for (int i = 0; i < 100; ++i) {
		D3DXMATRIX t;
		D3DXMatrixTranslation(&t, (rand() % 20 - 10), (rand() % 20 - 10), (rand() % 20 - 10));
		D3DXMATRIX w = world * t;
		m_Model->Render(w, fc, view, proj, m_Material, activeScene);
	}
}