#include "CameraSceneNode.h"

CameraSceneNode::CameraSceneNode()
{
    m_Parent = NULL;
    D3DXMatrixIdentity(&m_World);
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
