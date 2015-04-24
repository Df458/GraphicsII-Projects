#ifndef PARTICLE_EMITTER_NODE_H
#define PARTICLE_EMITTER_NODE_H
#include "SceneNode.h"
#include <stack>

class MeshObject3D;
class BaseMaterial;

struct Particle {
	float life = 100;
	D3DXMATRIX position;
};

class ParticleEmitterNode : public SceneNode
{
public:
	ParticleEmitterNode(MeshObject3D* model);
	ParticleEmitterNode(MeshObject3D* model, float x, float y, float z, float xRot = 0, float yRot = 0, float zRot = 0);
	ParticleEmitterNode(rapidxml::xml_node<>* node);
	~ParticleEmitterNode();
	virtual BaseMaterial* getMaterial(void);


	virtual void Update(float deltatime);
	virtual void Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice);
	virtual void createParticle();
private:
	MeshObject3D* m_Model = NULL;
	BaseMaterial* m_Material = NULL;
	std::vector<Particle*> particles;
	std::stack<Particle*> dead;
	float timer = 0;
};

#endif
