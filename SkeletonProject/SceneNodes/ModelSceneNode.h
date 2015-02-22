#ifndef MODEL_SCENE_NODE_H
#define MODEL_SCENE_NODE_H
#include "SceneNode.h"

class MeshObject3D;

class ModelSceneNode : public SceneNode
{
public:
    ModelSceneNode(MeshObject3D* model);
	ModelSceneNode(MeshObject3D* model, float x, float y, float z, float xRot = 0, float yRot = 0, float zRot = 0);
    
    virtual void Update(float deltatime);
    virtual void Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice, LPD3DXEFFECT effect = NULL);
private:
    MeshObject3D* m_Model;
};

#endif
