#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "../Scene.h"
#include <vector>

class SceneNode
{
public:
    SceneNode();
    virtual void Update(float deltatime) {};
    virtual void Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice) {};
    virtual void renderChildren(Scene* activeScene, IDirect3DDevice9* gd3dDevice);

    void Translate(float x, float y, float z, bool relative = false, bool rotation_relative = false);
    void Rotate(float yaw, float pitch, float roll, bool relative = false);
    void Scale(float x, float y, float z, bool relative = false);

	void SetRotationLimits(float YawMin, float YawMax, float PitchMin, float PitchMax, float RollMin, float RollMax);

    friend class Scene;
protected:
    virtual void addChild(SceneNode* child);
    virtual void removeChild(SceneNode* child);

    SceneNode* m_Parent = NULL;
    std::vector<SceneNode*> m_Children;
	float 
		m_Yaw = 0,		 
		m_Pitch = 0, 
		m_Roll = 0,
		m_X = 0,
		m_Y = 0,
		m_Z = 0;

    D3DXMATRIX m_World;
	D3DXMATRIX m_Rotation;
	//move to utils
	float clamp(float num, float min, float max);
};
#endif
