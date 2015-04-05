#include "../d3dUtil.h"
#include "SkySceneNode.h"
#include "../Materials/SkyBoxMaterial.h"
#include "../3DClasses/UVSphereObject3D.h"
#include "../Utils.h"

SkySceneNode::SkySceneNode()
{
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    m_Model = new UVSphereObject3D(1, 6, 8, new SkyBoxMaterial((getPath() + "cubemap.png").c_str())); // Needs an effect
}

SkySceneNode::SkySceneNode(float x, float y, float z, float xRot, float yRot, float zRot)
{
    printf("Unimplemented stub\n");
}

SkySceneNode::SkySceneNode(rapidxml::xml_node<>* node, ID3DXEffect* effect)
{
    printf("Unimplemented stub\n");
}
