//=============================================================================
//                          SkeletonClass.h
//
// Adopted from CubeDemo.cpp by Frank Luna (C) 2005 All Rights Reserved.
//
// Edited and changed by Adi Bar-Lev, 2013
// EGP-300-101 - Computer Graphics II, Spring 2013.
//
// This project creates a skeleton workspace and base classes for the student to be 
// able to start working with sample classes and develop evolve it according to the 
// given exercises
//
// Controls: Use mouse to orbit and zoom; use the 'W' and 'S' keys to 
//           alter the height of the camera.
//=============================================================================
#ifndef _SKELETON_CLASS_H
#define _SKELETON_CLASS_H
//=============================================================================
#pragma once
//=============================================================================
#include "d3dApp.h"

#include <vector>
//=============================================================================
class BaseObject3D;
class Scene;
class CameraSceneNode;
//=============================================================================
class SkeletonClass : public D3DApp
{
public:
	SkeletonClass(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	~SkeletonClass();

	bool checkDeviceCaps();
	void onLostDevice();
	void onResetDevice();
	void updateScene(float dt);
	void drawScene();

	// Helper methods
	void buildViewMtx();
	void buildProjMtx();

private:
	int currentobj = 0;
    float mCameraRotationY;
    float mCameraRadius;
    float mCameraHeight;

	//D3DXMATRIX mView;
	//D3DXMATRIX mProj;
	
	//Input
	float X, Y, prevX, prevY;

    //std::vector<BaseObject3D*>      m_Objects;

	bool
		pW,
		pT,
		pO,
		pS,
		pG,
		pD;

    Scene* m_Scene;
    CameraSceneNode* m_Camera;
};
//=============================================================================
#endif // _SKELETON_CLASS_H_
