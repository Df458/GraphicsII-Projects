#include "../d3dUtil.h"
#include "SkySceneNode.h"
#include "../Scene.h"
#include "CameraSceneNode.h"
#include "../Materials/SkyBoxMaterial.h"
#include "../3DClasses/UVSphereObject3D.h"
#include "../Utils.h"
#include "../ResourceManager.h"


SkySceneNode::SkySceneNode()
{
   /* m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    ID3DXEffect* effect;
	effect = (ID3DXEffect*)gResourceManager->getDefaultSkyEffect()->GetData();
	
	m_Mat = new SkyBoxMaterial("defaultSky.dds");
    fprintf(stderr, "done.\n");
    m_Model = new UVSphereObject3D(5, 8, 8, m_Mat, effect); // Needs an effect
	m_Model->Create(gd3dDevice);*/
	int nope = 1;//SHOULD NEVER REACH HERE
}

SkySceneNode::SkySceneNode(float x, float y, float z, float xRot, float yRot, float zRot)
{
	/*
    printf("Unimplemented stub\n");
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    m_Model = new UVSphereObject3D(1, 6, 8, new SkyBoxMaterial("defaultSky.dds")); // Needs an effect*/
	int nope = 1;//SHOULD NEVER REACH HERE
}

SkySceneNode::SkySceneNode(rapidxml::xml_node<>* node)
{
	ID3DXEffect* effect = nullptr;
	IDirect3DCubeTexture9* texture = nullptr;

	if(rapidxml::xml_node<>* shader_node = node->first_node("shader", 6, false))
	{
		if (rapidxml::xml_attribute<>* shadername = shader_node->first_attribute("name", 4, false))
		{
			gResourceManager->LoadEffectResource(shadername->value());
			effect = (ID3DXEffect*)gResourceManager->GetEffect(shadername->value())->GetData();
		}
	}
	if (effect == nullptr)
	{
		effect = (ID3DXEffect*)gResourceManager->getDefaultSkyEffect()->GetData();
	}
	if (rapidxml::xml_node<>* texture_node = node->first_node("texture", 7, false))
	{
		if (rapidxml::xml_attribute<>* texturename = texture_node->first_attribute("name", 4, false))
		{
			gResourceManager->LoadTextureResource(texturename->value());
			effect = (ID3DXEffect*)gResourceManager->GetEffect(texturename->value())->GetData();
		}
	}

	if (texture == nullptr)
	{
		texture = (IDirect3DCubeTexture9*)gResourceManager->getDefaultSkyEffect()->GetData();
	}

    D3DXMatrixIdentity(&m_World);
	float radius = 100.0f;
	unsigned rings = 50;
	unsigned radials = 50;

	if (rapidxml::xml_attribute<>* atr = node->first_attribute("radius", 6, false))
		radius = (float)atof(atr->value());
	if (rapidxml::xml_attribute<>* atn = node->first_attribute("rings", 5, false))
		rings = atoi(atn->value());
	if (rapidxml::xml_attribute<>* atl = node->first_attribute("radials", 7, false))
		radials = atoi(atl->value());

	m_Mat = new SkyBoxMaterial(effect, texture);
	m_Model = new UVSphereObject3D(radius, rings, radials);
}

void SkySceneNode::Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice)
{
	m_World = m_Scale * m_Rotation * m_Translation;
    D3DXMATRIX view = activeScene->getActiveCamera()->getView();
    D3DXMATRIX proj = activeScene->getProjection();
    LightSceneNode* light = activeScene->getActiveLight();
    D3DXMATRIX fc = activeScene->getActiveCamera()->getFocusView();
    m_Model->Render(m_World, fc, view, proj, light, NULL, m_Mat);
}

IDirect3DCubeTexture9* SkySceneNode::getSkyTexture() {
	return m_Mat->getSkyTexture();
}
