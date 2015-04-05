#include "../d3dUtil.h"
#include "ModelSceneNode.h"
#include "LightSceneNode.h"
#include "../3DClasses/MeshObject3D.h"
#include "../3DClasses/CylinderObject3D.h"
#include "../3DClasses/TorusObject3D.h"
#include "../3DClasses/SimpleCubeObject3D.h"
#include "../3DClasses/ConeObject3D.h"
#include "../3DClasses/TeapotObject3D.h"
#include "../3DClasses/UVSphereObject3D.h"
#include "../Materials/BaseMaterial.h"
#include "SkySceneNode.h"
#include "../Scene.h"

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
    BaseMaterial* mat;
    if(xml_node<>* nmat = node->first_node("material", 8, false))
    {
		mat = new BaseMaterial(nmat);
    }
    else
        mat = new BaseMaterial();
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

ModelSceneNode::~ModelSceneNode()
{
    delete m_Model;
}

BaseMaterial* ModelSceneNode::getMaterial(void)
{ 
	return m_Model->getMaterial(); 
}
void ModelSceneNode::Update(float deltatime)
{
	m_Yaw += 1 * deltatime;
	D3DXMatrixRotationY(&m_Rotation, m_Yaw);
	m_World = m_Scale * m_Rotation * m_Translation;
    m_Model->Update(deltatime);
}

void ModelSceneNode::Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice)
{
	m_World = m_Scale * m_Rotation * m_Translation;
    D3DXMATRIX world = activeScene->getTopMatrix() * m_World;
    D3DXMATRIX view = activeScene->getView();
    D3DXMATRIX proj = activeScene->getProjection();
    LightSceneNode* light = activeScene->getActiveLight();
    m_Model->Render(gd3dDevice, world, view, proj, light, activeScene->getActiveSky()->getSkyTexture());
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
    else if(!strcmp(name, "cylinder"))
    {
        float height = 1.0f;
        if(xml_attribute<>* ath = node->first_attribute("height", 6, false))
            height = atof(ath->value());
        float radius = 1.0f;
        if(xml_attribute<>* atr = node->first_attribute("radius", 6, false))
            radius = atof(atr->value());
        unsigned radials = 8;
        if(xml_attribute<>* atl = node->first_attribute("radials", 7, false))
            radials = atoi(atl->value());
        m_Model = new CylinderObject3D(radius, radials, height, mat, effect);
    }
    else if(!strcmp(name, "cone"))
    {
        float height = 1.0f;
        if(xml_attribute<>* ath = node->first_attribute("height", 6, false))
            height = atof(ath->value());
        float radius = 1.0f;
        if(xml_attribute<>* atr = node->first_attribute("radius", 6, false))
            radius = atof(atr->value());
        unsigned radials = 8;
        if(xml_attribute<>* atl = node->first_attribute("radials", 7, false))
            radials = atoi(atl->value());
        m_Model = new ConeObject3D(radius, radials, height, mat, effect);
    }
    else if(!strcmp(name, "cube"))
    {
        float width = 1.0f;
        if(xml_attribute<>* atw = node->first_attribute("width", 5, false))
            width = atof(atw->value());
        float height = 1.0f;
        if(xml_attribute<>* ath = node->first_attribute("height", 6, false))
            height = atof(ath->value());
        float depth = 1.0f;
        if(xml_attribute<>* atd = node->first_attribute("depth", 5, false))
            depth = atof(atd->value());
        m_Model = new SimpleCubeObject3D(mat, width, height, depth, effect);
    }
    else if(!strcmp(name, "teapot"))
    {
        m_Model = new TeapotObject3D(mat, effect);
    }
    else if(!strcmp(name, "torus"))
    {
        float radiusi = 1.0f;
        if(xml_attribute<>* atr = node->first_attribute("radiusi", 7, false))
            radiusi = atof(atr->value());
        float radiusr = 1.0f;
        if(xml_attribute<>* atr = node->first_attribute("radiusr", 7, false))
            radiusr = atof(atr->value());
        unsigned sides = 8;
        if(xml_attribute<>* ats = node->first_attribute("sides", 5, false))
            sides = atoi(ats->value());
        unsigned rings = 8;
        if(xml_attribute<>* atg = node->first_attribute("rings", 5, false))
            rings = atoi(atg->value());
        m_Model = new TorusObject3D(radiusi, radiusr, sides, rings, mat, effect);
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
