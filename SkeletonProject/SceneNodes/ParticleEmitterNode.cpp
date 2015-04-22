#include "../d3dUtil.h"
#include "CameraSceneNode.h"
#include "ParticleEmitterNode.h"
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

    xml_attribute<>* type = node->first_attribute("type", 4, false);
    if(!type)
    {
        fprintf(stderr, "Error: Model defined in level with no type(node %s).\n", node->name());
        return;
    }

	if (effect == nullptr)
	{
		effect = (ID3DXEffect*)gResourceManager->getDefaultEffect()->GetData();
	}

    if(!strcmp(type->value(), "primitive"))
    {
    }
    else if(!strcmp(type->value(), "X"))
    {
		if (xml_attribute<>* name = node->first_attribute("filename", 8, false))
		{
			float modelScale = 1.0f;
			if (xml_attribute<>* scale = node->first_attribute("scale", 5, false))
			{
				modelScale = atof(scale->value());
			}
			gResourceManager->LoadMeshResource(name->value());
		}
    }
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
	m_Yaw += 1 * deltatime;
	D3DXMatrixRotationY(&m_Rotation, m_Yaw);
	m_World = m_Scale * m_Rotation * m_Translation;
}

void ParticleEmitterNode::Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice)
{
	m_World = m_Scale * m_Rotation * m_Translation;
    D3DXMATRIX world = m_World * activeScene->getTopMatrix();
    D3DXMATRIX view = activeScene->getView();
    D3DXMATRIX proj = activeScene->getProjection();
    D3DXMATRIX fc = activeScene->getActiveCamera()->getFocusView();
    m_Model->Render(world, fc, view, proj,  m_Material, activeScene);
}