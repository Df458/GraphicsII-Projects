#include "../d3dUtil.h"
#include "CameraSceneNode.h"
#include "ParticleEmitterNode.h"
#include "../3DClasses/XModel3D.h"
#include "../3DClasses/MeshObject3D.h"
#include "../Materials/BaseMaterial.h"
#include "../Scene.h"
#include "../ResourceManager.h"

using namespace rapidxml;

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
	for (auto i : particles)
		delete i;
	while (dead.size() > 0) {
		delete dead.top();
		dead.pop();
	}
    delete m_Model;
}

BaseMaterial* ParticleEmitterNode::getMaterial(void)
{ 
	return m_Material; 
}

void ParticleEmitterNode::Update(float deltatime)
{
	for (auto i = particles.begin(); i != particles.end();) {
		D3DXMATRIX w;
		D3DXMatrixTranslation(&w, (rand() % 14 - 7) * deltatime, rand() % 4 * deltatime, (rand() % 14 - 7) * deltatime);
		(*i)->position *= w;
		(*i)->life -= deltatime;
		if ((*i)->life <= 0) {
			dead.push(*i);
			i = particles.erase(i);
		} else
			++i;
	}
	timer -= deltatime;
	if (timer <= 0) {
		createParticle();
		timer = 0.05;
	}
}

void ParticleEmitterNode::createParticle() {
	if (dead.size() > 0) {
		particles.push_back(dead.top());
		dead.pop();
	}
	else {
		particles.push_back(new Particle);
	}

	Particle* p = particles.back();
	p->life = 10;
	D3DXMATRIX w;
	D3DXMatrixTranslation(&w, (rand() % 100 - 50) * 0.02f, (rand() % 100 - 50) * 0.02f, (rand() % 100 - 50) * 0.02f);
	p->position = m_World * w;
}

void ParticleEmitterNode::Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice)
{
	m_World = m_Scale * m_Rotation * m_Translation;
    
    D3DXMATRIX view = activeScene->getView();
    D3DXMATRIX proj = activeScene->getProjection();
    D3DXMATRIX fc = activeScene->getActiveCamera()->getFocusView();
	for (int i = 0; i < particles.size(); ++i) {
		D3DXMATRIX world = particles[i]->position;
		m_Model->Render(world, fc, view, proj, m_Material, activeScene);
	}
}