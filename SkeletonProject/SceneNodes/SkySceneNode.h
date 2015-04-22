#ifndef SKY_SCENE_NODE_H
#define SKY_SCENE_NODE_H
#include "SceneNode.h"

class UVSphereObject3D;
class BaseMaterial;
class SkyBoxMaterial;

class SkySceneNode : public SceneNode
{
public:
    SkySceneNode();
	SkySceneNode(float x, float y, float z, float xRot = 0, float yRot = 0, float zRot = 0);
    SkySceneNode(rapidxml::xml_node<>* node);
    virtual ~SkySceneNode() {}
    virtual void Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice);
	IDirect3DCubeTexture9* getSkyTexture();

private:
    UVSphereObject3D* m_Model = NULL;
	SkyBoxMaterial* m_Mat = NULL;
    bool active = true;
};

#endif
