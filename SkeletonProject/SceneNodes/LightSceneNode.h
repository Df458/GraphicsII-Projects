#pragma once
#include "../d3dApp.h"
#include "SceneNode.h"

enum class LightType {
    POINT_LIGHT,
    SPOT_LIGHT,
    DIRECTIONAL_LIGHT
};

class LightSceneNode : public SceneNode
{
public:
    LightSceneNode();
    LightSceneNode(rapidxml::xml_node<>* node);
    virtual void Update(float deltatime);
    virtual void Render(Scene* activeScene, IDirect3DDevice9* gd3dDevice) {}
    D3DXVECTOR4 getColor(void) { return m_Color; }
protected:
    D3DXVECTOR4 m_Color;
    float m_Energy;
    float m_FalloffDistance;
    bool m_AffectsDiffuse;
    bool m_AffectsSpecular;
    LightType m_Type;
    float m_SpotAngle;
};
