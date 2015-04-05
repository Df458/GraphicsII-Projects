#include "../d3dUtil.h"
#include "SkySceneNode.h"
#include "../Materials/SkyBoxMaterial.h"
#include "../3DClasses/UVSphereObject3D.h"
#include "../Utils.h"

SkySceneNode::SkySceneNode()
{
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    ID3DXEffect* effect;
	LPD3DXBUFFER error_buf;
    fprintf(stderr, "Compiling shader...\n");
	HR(D3DXCreateEffectFromFile(gd3dDevice, "Lighting.fx", NULL, NULL, 0, NULL, &effect, &error_buf))
	if (error_buf)
		fprintf(stderr, "Errors:\n%s\n", (char*)error_buf->GetBufferPointer());
    fprintf(stderr, "done.\n");
    m_Model = new UVSphereObject3D(5, 6, 8, new SkyBoxMaterial((getPath() + "cubemap.png").c_str()), effect); // Needs an effect
}

SkySceneNode::SkySceneNode(float x, float y, float z, float xRot, float yRot, float zRot)
{
    printf("Unimplemented stub\n");
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    m_Model = new UVSphereObject3D(1, 6, 8, new SkyBoxMaterial((getPath() + "cubemap.png").c_str())); // Needs an effect
}

SkySceneNode::SkySceneNode(rapidxml::xml_node<>* node, ID3DXEffect* effect)
{
    printf("Unimplemented stub\n");
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    m_Model = new UVSphereObject3D(1, 6, 8, new SkyBoxMaterial((getPath() + "cubemap.png").c_str())); // Needs an effect
}

void SkySceneNode::Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice)
{
	m_World = m_Scale * m_Rotation * m_Translation;
    D3DXMATRIX world = activeScene->getTopMatrix() * m_World;
    D3DXMATRIX view = activeScene->getView();
    D3DXMATRIX proj = activeScene->getProjection();
    LightSceneNode* light = activeScene->getActiveLight();
    m_Model->Render(gd3dDevice, world, view, proj, light);
}
