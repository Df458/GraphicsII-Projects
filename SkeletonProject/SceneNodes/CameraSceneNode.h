#ifndef CAMERA_SCENE_NODE
#define CAMERA_SCENE_NODE
#include <d3dx9.h>
#include "SceneNode.h"

class CameraSceneNode : public SceneNode
{
public:
    CameraSceneNode();

    virtual void Update(float deltatime){}
    virtual void Render(){}

    D3DXMATRIX getView(void) const;
    D3DXMATRIX getProjection(void) const;
    void setView(D3DXMATRIX);
    void setProjection(D3DXMATRIX);
protected:
    D3DXMATRIX m_Projection;
};

#endif
