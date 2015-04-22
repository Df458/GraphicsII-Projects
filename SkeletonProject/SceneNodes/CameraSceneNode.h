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

    D3DXMATRIX getView(void) const;
    D3DXMATRIX getProjection(void) const;
    void setView(D3DXMATRIX);
    void setProjection(D3DXMATRIX);
    void rebuildProjection(float w, float h);
	void setFocus(SceneNode* target);
	void turnFocus(float x, float y);
    void zoomFocus(float distance);
	void releaseFocus();

	SceneNode* getFocusNode(){ return focusTarget; };
    bool getFocused(void) { return focused; }
	D3DXMATRIX getFocusView(void) const;
	D3DXMATRIX getBillboardMatrix(void) const;
protected:
	bool focused;
	SceneNode* focusTarget;
	float focusDistance = 3;
	float desiredFocusDistance = 3;

    D3DXMATRIX m_Projection;
    D3DXMATRIX m_FocusView;
    float m_Near;
    float m_Far;
    float m_Angle;
};

#endif
