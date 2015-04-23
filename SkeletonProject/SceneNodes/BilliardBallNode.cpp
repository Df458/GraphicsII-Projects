#include "BilliardBallNode.h"


BilliardBallNode::BilliardBallNode(MeshObject3D* model, D3DXVECTOR3 StartingPosition)
{

}

BilliardBallNode::~BilliardBallNode()
{

}

void BilliardBallNode::Initialize()
{

}

void BilliardBallNode::Terminate()
{

}

void BilliardBallNode::Update(float delatime)
{
	m_Velocity += (m_Acceleration * delatime);
	m_Position += (m_Velocity * delatime);
}
