#include "CameraSceneNode.h"

using namespace rapidxml;

CameraSceneNode::CameraSceneNode()
{
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
    D3DXMatrixTranslation(&m_World, 0, 1.0f, -15.0f);
}

CameraSceneNode::CameraSceneNode(xml_node<>* node) : SceneNode(node)
{
    m_Parent = NULL;
    if(xml_node<>* nlookat = node->first_node("lookat", 6, false))
    {
        float tX = 0;
        float tY = 0;
        float tZ = 0;
        if(xml_attribute<>* atx = nlookat->first_attribute("x", 1, false))
            tX = atof(atx->value());
        if(xml_attribute<>* aty = nlookat->first_attribute("y", 1, false))
            tY = atof(aty->value());
        if(xml_attribute<>* atz = nlookat->first_attribute("z", 1, false))
            tZ = atof(atz->value());
        D3DXVECTOR3 at  = D3DXVECTOR3(tX, tY, tZ);
        D3DXVECTOR3 eye = D3DXVECTOR3(m_X, m_Y, m_Z);
        D3DXVECTOR3 up  = D3DXVECTOR3(0, 1, 0);
        D3DXMATRIX mlookat = *D3DXMatrixLookAtLH(&mlookat, &at, &eye, &up);
        setView(mlookat);
    }

}

void CameraSceneNode::setProjection(D3DXMATRIX projection)
{
    m_Projection = projection;
}

void CameraSceneNode::setView(D3DXMATRIX view)
{
    D3DXMatrixInverse(&m_World, NULL, &view);
}

D3DXMATRIX CameraSceneNode::getView(void) const
{
    D3DXMATRIX out;
    return *D3DXMatrixInverse(&out, NULL, &m_World);
}

D3DXMATRIX CameraSceneNode::getProjection(void) const
{
    return m_Projection;
}
