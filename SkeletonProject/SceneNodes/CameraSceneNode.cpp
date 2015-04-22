#include "CameraSceneNode.h"
#include "../d3dUtil.h"

using namespace rapidxml;

CameraSceneNode::CameraSceneNode()
{
    m_Parent = NULL;
	focused = false;
    D3DXMatrixIdentity(&m_World);
    D3DXMatrixTranslation(&m_World, 0, 1.0f, -15.0f);
}

CameraSceneNode::CameraSceneNode(xml_node<>* node) : SceneNode(node)
{
    m_Parent = NULL;
	focused = false;
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

    m_Near = 0.1f;
    m_Far = 1000.0f;
    m_Angle = 90;

    if(xml_node<>* nproj = node->first_node("projection", 10, false))
    {
        if(xml_attribute<>* anear = nproj->first_attribute("near", 4, false))
            m_Near = atof(anear->value());
        if(xml_attribute<>* afar = nproj->first_attribute("far", 3, false))
            m_Far = atof(afar->value());
        if(xml_attribute<>* angle = nproj->first_attribute("angle", 5, false))
            m_Angle = atof(angle->value());
    }
}

void CameraSceneNode::Update(float deltatime)
{
	if (focusDistance != desiredFocusDistance)
	{
		focusDistance += (desiredFocusDistance - focusDistance) * 0.01f;

		D3DXMatrixTranslation(&m_FocusView, 0, 0, -focusDistance);
	}
}

void CameraSceneNode::setProjection(D3DXMATRIX projection)
{
    m_Projection = projection;
}

void CameraSceneNode::setView(D3DXMATRIX view)
{
    if(!focused)
    {
        D3DXMatrixInverse(&m_World, NULL, &view);
    }
}

D3DXMATRIX CameraSceneNode::getView(void) const
{
    D3DXMATRIX out;
    if(!focused)
        return *D3DXMatrixInverse(&out, NULL, &m_World);
    out = m_FocusView * focusTarget->getMatrix();
    return *D3DXMatrixInverse(&out, NULL, &out);
}

D3DXMATRIX CameraSceneNode::getFocusView(void) const
{
	return m_FocusView;
}

D3DXMATRIX CameraSceneNode::getProjection(void) const
{
    return m_Projection;
}

D3DXMATRIX CameraSceneNode::getBillboardMatrix(void) const
{
	D3DXMATRIX v = getView();
	D3DXVECTOR3 forward = D3DXVECTOR3(-v._13, -v._23, -v._33);
	D3DXVECTOR3 up(v._12, v._22, v._32);
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &forward, &up);
	D3DXMATRIX mat = { right.x, right.y, right.z, 0.0f, up.x, up.y, up.z, 0.0f, forward.x, forward.y, forward.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	return mat;
}

void CameraSceneNode::rebuildProjection(float w, float h)
{
    D3DXMatrixPerspectiveFovLH(&m_Projection, m_Angle * DEGTORAD, w/h, m_Near, m_Far);
}

void CameraSceneNode::setFocus(SceneNode* target)
{
    focused = true;
	if (target)
	{
		focusTarget = new SceneNode(target);
	}
	else
	{
		focusTarget = new SceneNode();
	}
    D3DXVECTOR3 diff = getPosition() - focusTarget->getPosition();
    focusDistance = sqrt(pow(diff.x, 2) + pow(diff.y, 2) + pow(diff.z, 2));
    D3DXVec3Normalize(&diff, &diff);
    focusTarget->Rotate(atan2(-diff.x, -diff.z), atan2(diff.y, sqrt(pow(diff.x, 2) + pow(diff.z, 2))), 0);
    D3DXMatrixTranslation(&m_FocusView, 0, 0, -focusDistance);
}

void CameraSceneNode::releaseFocus()
{
    if(!focused)
        return;
    D3DXMATRIX mat = m_FocusView * focusTarget->getMatrix();
    D3DXVECTOR3 scale;
    D3DXVECTOR3 trans;
    D3DXQUATERNION qrot;
    D3DXMatrixDecompose(&scale, &qrot, &trans, &mat);
    Translate(trans.x, trans.y, trans.z);
    D3DXVECTOR3 rot = focusTarget->getRotation();
    Rotate(rot.x, rot.y, rot.z);
	focused = false;
    delete focusTarget;
	focusTarget = nullptr;
}

void CameraSceneNode::turnFocus(float x, float y)
{
    if(!focused)
        return;

    focusTarget->Rotate(x, y, 0, true);
}

void CameraSceneNode::zoomFocus(float distance)
{
	distance /= 120.0f;
    if(!focused)
        return;
    desiredFocusDistance -= distance;
	if (desiredFocusDistance <= m_Near)
		desiredFocusDistance = m_Near + 0.1f;
}
