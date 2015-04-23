/************************************************************************
							BilliardBallNode.h
Author: Johnathan O'Malia © 2015
Description: Physics controlled billiard ball data object

TODO:
ALL

/************************************************************************/
#ifndef BILLIARD_BALL_NODE_H_
#define BILLIARD_BALL_NODE_H_

#include "ModelSceneNode.h"

class BilliardBallNode : public ModelSceneNode
{
public:
	BilliardBallNode(MeshObject3D* model, D3DXVECTOR3 StartingPosition);
	~BilliardBallNode();

	void Initialize();
	void Terminate();

	virtual void Update(float delatime);

private:
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Velocity;
	D3DXVECTOR3 m_Acceleration;

	float m_deccelerationConstant;
	float mRadius;

	bool checkCollision(BilliardBallNode* other);

};

#endif // !BILLIARD_BALL_NODE_H_