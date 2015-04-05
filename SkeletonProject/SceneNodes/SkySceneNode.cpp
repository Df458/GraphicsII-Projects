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
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    ID3DXEffect* effect;
	LPD3DXBUFFER error_buf;
    fprintf(stderr, "Compiling shader...\n");
	//D3DXCreateEffectFromFile(gd3dDevice, "skybox.fx", NULL, NULL, 0, NULL, &effect, &error_buf);
	effect = gResourceManager->GetEffect("skybox.fx");
	
	m_Mat = new SkyBoxMaterial("OutputCube.dds");
    fprintf(stderr, "done.\n");
    m_Model = new UVSphereObject3D(5, 8, 8, m_Mat, effect); // Needs an effect
	m_Model->Create(gd3dDevice);
}

SkySceneNode::SkySceneNode(float x, float y, float z, float xRot, float yRot, float zRot)
{
    printf("Unimplemented stub\n");
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    m_Model = new UVSphereObject3D(1, 6, 8, new SkyBoxMaterial("OutputCube.dds")); // Needs an effect
}

SkySceneNode::SkySceneNode(rapidxml::xml_node<>* node, ID3DXEffect* effect)
{
    printf("Unimplemented stub\n");
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    m_Model = new UVSphereObject3D(1, 6, 8, new SkyBoxMaterial("OutputCube.dds")); // Needs an effect
}

void SkySceneNode::Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice)
{
	D3DXVECTOR4 t = activeScene->getActiveFocus()->getTranslation();
	D3DXMatrixTranslation(&m_Translation, t.x, t.y, t.z);
	m_World = m_Scale * m_Rotation * m_Translation;
    D3DXMATRIX world = activeScene->getTopMatrix() * m_World;
    D3DXMATRIX view = activeScene->getView();
    D3DXMATRIX proj = activeScene->getProjection();
    LightSceneNode* light = activeScene->getActiveLight();
    m_Model->Render(gd3dDevice, world, view, proj, light, NULL);
}

IDirect3DCubeTexture9* SkySceneNode::getSkyTexture() {
	return m_Mat->getSkyTexture();
}
