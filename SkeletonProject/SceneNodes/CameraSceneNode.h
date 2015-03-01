#ifndef CAMERA_SCENE_NODE
#define CAMERA_SCENE_NODE
#include <d3dx9.h>
#include <rapidxml.hpp>
#include "SceneNode.h"

class CameraSceneNode : public SceneNode
{
public:
    CameraSceneNode();
    CameraSceneNode(rapidxml::xml_node<>* node);

	virtual void Update(float deltatime);
    virtual void Render(){}
    virtual void renderChildren(Scene* activeScene, IDirect3DDevice9* gd3dDevice){ }

    D3DXMATRIX getView(void) const;
    D3DXMATRIX getProjection(void) const;
    void setView(D3DXMATRIX);
    void setProjection(D3DXMATRIX);
    void rebuildProjection(float w, float h);
	void setFocus(SceneNode* target);
	void turnFocus(float x, float y);
	void releaseFocus();

	SceneNode* getFocusNode(){ return focusTarget; };
protected:
	bool focused;
	SceneNode* focusTarget;
	float focusDistance;

    D3DXMATRIX m_Projection;
    D3DXMATRIX m_FocusView;
    float m_Near;
    float m_Far;
    float m_Angle;
};

#endif
