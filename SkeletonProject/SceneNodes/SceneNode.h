#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "../Scene.h"
#include <rapidxml.hpp>
#include <vector>

class BaseMaterial;
class SceneNode
{
public:
    SceneNode();
    SceneNode(SceneNode* node);
    SceneNode(rapidxml::xml_node<>* node);
    virtual ~SceneNode() {}
    virtual void Update(float deltatime) {};
    virtual void Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice) {};
    virtual void renderChildren(Scene* activeScene, IDirect3DDevice9* gd3dDevice);
	virtual BaseMaterial* getMaterial(void) { return nullptr; };

    void Translate(float x, float y, float z, bool relative = false, bool rotation_relative = false);
    void Rotate(float yaw, float pitch, float roll, bool relative = false);
    void Scale(float x, float y, float z, bool relative = false);
    D3DXMATRIX getMatrix(void);

	D3DXVECTOR4 getTranslation(void) { return D3DXVECTOR4(m_X, m_Y, m_Z, 1.0f); }
	D3DXVECTOR3 getPosition(void) { return D3DXVECTOR3(m_X, m_Y, m_Z); }
	D3DXVECTOR3 getRotation(void) { return D3DXVECTOR3(m_Yaw, m_Pitch, m_Roll); }
	D3DXMATRIX  getWorld(void) { return m_World; }

	void SetRotationLimits(float YawMin, float YawMax, float PitchMin, float PitchMax, float RollMin, float RollMax);
    
	UINT getUID(void){ return mID; }

    friend class Scene;
protected:
    virtual void addChild(SceneNode* child);
    virtual void removeChild(SceneNode* child);
	virtual void UpdateMatricies();

    SceneNode* m_Parent = NULL;
    std::vector<SceneNode*> m_Children;
	float 
		m_Yaw = 0,		 
		m_Pitch = 0, 
		m_Roll = 0,
		m_X = 0,
		m_Y = 0,
		m_Z = 0,
		m_sX = 1,
		m_sY = 1,
		m_sZ = 1;

    D3DXMATRIX m_World;
	D3DXMATRIX m_Translation;
	D3DXMATRIX m_Rotation;
	D3DXMATRIX m_Scale;

	unsigned int mID;

	//move to utils
	float clamp(float num, float min, float max);
};
#endif
