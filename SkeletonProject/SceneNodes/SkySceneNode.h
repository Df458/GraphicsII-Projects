#ifndef SKY_SCENE_NODE_H
#define SKY_SCENE_NODE_H
#include "SceneNode.h"

class UVSphereObject3D;
class BaseMaterial;

class SkySceneNode : public SceneNode
{
public:
    SkySceneNode();
	SkySceneNode(float x, float y, float z, float xRot = 0, float yRot = 0, float zRot = 0);
    SkySceneNode(rapidxml::xml_node<>* node, ID3DXEffect* effect);
    virtual ~SkySceneNode() {}
private:
    UVSphereObject3D* m_Model;
    bool active = true;
};

#endif
