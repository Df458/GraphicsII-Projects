#include "../d3dUtil.h"
#include "ModelSceneNode.h"
#include "../3DClasses/MeshObject3D.h"
#include "../3DClasses/UVSphereObject3D.h"
#include "../Materials/BaseMaterial.h"

using namespace rapidxml;

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

ModelSceneNode::ModelSceneNode(xml_node<>* node, ID3DXEffect* effect) : SceneNode(node)
{
    BaseMaterial* mat = new BaseMaterial();
//:TODO: 23.02.15 20:00:58, df458
// Load material data here
    xml_attribute<>* type = node->first_attribute("type", 4, false);
    if(!type)
    {
        fprintf(stderr, "Error: Model defined in level with no type(node %s).\n", node->name());
        return;
    }
    if(!strcmp(type->value(), "primitive"))
    {
        if(xml_attribute<>* shape = node->first_attribute("shape", 5, false))
            generatePrimitive(shape->value(), node, mat, effect);
    }
    else if(!strcmp(type->value(), "mesh"))
    {
//:TODO: 23.02.15 19:38:45, df458
// load a mesh here
    }
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

void ModelSceneNode::generatePrimitive(const char* name, xml_node<>* node, BaseMaterial* mat, ID3DXEffect* effect)
{
//:TODO: 23.02.15 19:43:02, df458
// This is a factory method to make primitives. Add whatever we need
    if(!strcmp(name, "sphere"))
    {
        float radius = 1.0f;
        if(xml_attribute<>* atr = node->first_attribute("radius", 6, false))
            radius = atof(atr->value());
        unsigned rings = 8;
        if(xml_attribute<>* atn = node->first_attribute("rings", 5, false))
            rings = atoi(atn->value());
        unsigned radials = 8;
        if(xml_attribute<>* atl = node->first_attribute("radials", 7, false))
            radials = atoi(atl->value());
        m_Model = new UVSphereObject3D(radius, rings, radials, mat, effect);
    }
    else
    {
        fprintf(stderr, "Primitive shape: %s is undefined in this function(%s:%d).\nPlease define it.\n", name, __FILE__, __LINE__);
    }

    if(m_Model)
    {
        m_Model->Create(gd3dDevice);
    }
}
