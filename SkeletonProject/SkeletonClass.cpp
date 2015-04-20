//=============================================================================
//                          SkeletonClass.cpp
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
#include "DirectInput.h"
#include <crtdbg.h>
#include "GfxStats.h"
#include <list>

#include "CRC32.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneNodes/CameraSceneNode.h"
#include "SkeletonClass.h"
#include "3DClasses/Vertex.h"
#include "SceneManager.h"

const char* objscenes[6] = {
	"cyl.xml",
	"cube.xml",
	"tea.xml",
	"cone.xml",
	"sph.xml",
	"torus.xml",
};

//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	SkeletonClass app(hInstance, "Exercise Skeleton Project", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	DirectInput di(DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	gDInput = &di;

	return gd3dApp->run();
}

SkeletonClass::SkeletonClass(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
: D3DApp(hInstance, winCaption, devType, requestedVP)
{
	if(!checkDeviceCaps())
	{
		MessageBox(0, "checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}
	ResourceManager* tmpManager = new ResourceManager();
	mSceneManager = new SceneManager();

	tmpManager->Initalize();
	mSceneManager->Initalize();

	gResourceManager = tmpManager;

	mCameraRadius    = 10.0f;
	mCameraRotationY = 1.2 * D3DX_PI;
	mCameraHeight    = 5.0f;

	InitAllVertexDeclarations();

	//hard coded loading
	gResourceManager->LoadTextureResource("normal.png");
	gResourceManager->LoadTextureResource("test.png");
	gResourceManager->LoadTextureResource("earth.png");
	gResourceManager->LoadEffectResource("skybox.fx");


	mSceneManager->LoadMultiScene("Assignment5.xml");
	mSceneManager->GetActiveScene()->updateSize(md3dPP.BackBufferWidth, md3dPP.BackBufferHeight);

	/*m_Camera = mSceneManager->GetActiveScene()->getActiveCamera();

	if (m_Scene->getActiveFocus()) {
		m_Camera->setFocus(m_Scene->getActiveFocus());
	}*/

	pW = false;
	pT = false;
	pO = false;
	pS = false;
	pA = false;
	pPlus = false;
	pMinus = false;
	p1 = false;
	p2 = false;
	p3 = false;
	p4 = false;
	p5 = false;
	p6 = false;
	p7 = false;
	pR = false;
	pN = false;

	onResetDevice();
}

SkeletonClass::~SkeletonClass()
{
    GfxStats::DeleteInstance();

	mSceneManager->Terminate();
	delete mSceneManager;

	DestroyAllVertexDeclarations();

	gResourceManager->Terminate();
}

bool SkeletonClass::checkDeviceCaps()
{
	return true;
}

void SkeletonClass::onLostDevice()
{
	GfxStats::GetInstance()->onLostDevice();
	gResourceManager->OnLostDevice();
}

void SkeletonClass::onResetDevice()
{
	GfxStats::GetInstance()->onResetDevice();

	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	//buildProjMtx();
	mSceneManager->updateSize(md3dPP.BackBufferWidth, md3dPP.BackBufferHeight);
	gResourceManager->OnResetDevice();
}

void SkeletonClass::updateScene(float dt)
{

	// Reset the statistics for the scene - each object will add to it.
	GfxStats::GetInstance()->setVertexCount(0);
	GfxStats::GetInstance()->setTriCount(0);
	GfxStats::GetInstance()->update(dt);

	// Get snapshot of input devices.
	gDInput->poll();

	if (gDInput->mouseButtonDown(0))
	{
		if (mSceneManager->GetActiveScene()->getActiveCamera()->getFocused())
			mSceneManager->GetActiveScene()->getActiveCamera()->turnFocus((gDInput->mouseDX()) * DEGTORAD, (gDInput->mouseDY()) * DEGTORAD);
		else
		{
			mSceneManager->GetActiveScene()->getActiveCamera()->Rotate((gDInput->mouseDX()) * DEGTORAD, (gDInput->mouseDY()) * DEGTORAD, 0, true);
			mSceneManager->GetActiveScene()->getActiveCamera()->SetRotationLimits(0, (float)M_PI, -(float)M_PI / 2, (float)M_PI / 2, 0, 0);
		}
	}

	if (gDInput->mouseDZ() != 0)
		if (gDInput->keyDown(DIK_LSHIFT))
			mSceneManager->GetActiveScene()->getActiveCamera()->zoomFocus(gDInput->mouseDZ()/2);
		else
			mSceneManager->GetActiveScene()->getActiveCamera()->zoomFocus(gDInput->mouseDZ());

	if (gDInput->keyDown(DIK_M))
		mSceneManager->GetActiveScene()->getActiveCamera()->releaseFocus();
	if (gDInput->keyDown(DIK_N))
		mSceneManager->GetActiveScene()->getActiveCamera()->setFocus(mSceneManager->GetActiveScene()->getActiveFocus());


	//Assignment 4 Controls
	if (gDInput->keyDown(DIK_ADD))
	{
		if (!pPlus)
			mSceneManager->GetActiveScene()->DEBUGBLENDSPECULARREFLECTION(-0.01f);
		pPlus = true;
	}
	else
		pPlus = false;

	if (gDInput->keyDown(DIK_MINUS))
	{
		if (!pMinus)
			mSceneManager->GetActiveScene()->DEBUGBLENDSPECULARREFLECTION(0.01f);
		pMinus = true;
	}
	else
		pMinus = false;

	if (gDInput->keyDown(DIK_A))
	{
		if (!pA)
			mSceneManager->GetActiveScene()->DEBUGNORMALSTRENGTH(-0.01f);
		pA = true;
	}
	else
		pA = false;

	if (gDInput->keyDown(DIK_S))
	{
		if (!pS)
			mSceneManager->GetActiveScene()->DEBUGNORMALSTRENGTH(0.01f);
		pS = true;
	}
	else
		pS = false;

	if (gDInput->keyDown(DIK_W))
	{
		if (!pW)
			mSceneManager->GetActiveScene()->DEBUGTOGGLEWIREFRAME();
		pW = true;
	}
	else
		pW = false;

	if (gDInput->keyDown(DIK_T))
	{
		if (!pT)
			mSceneManager->GetActiveScene()->DEBUGTOGGLETEXTURE();
		pT = true;
	}
	else
		pT = false;

	if (gDInput->keyDown(DIK_R))
	{
		if (!pR)
			mSceneManager->GetActiveScene()->DEBUGTOGGLEREFLECTION();
		pR = true;
	}
	else
		pR = false;

	if (gDInput->keyDown(DIK_N))
	{
		if (!pN)
			mSceneManager->GetActiveScene()->DEBUGTOGGLENORMAL();
		pN = true;
	}
	else
		pN = false;

	if (gDInput->keyDown(DIK_O))
	{
		if (!pO)
			mSceneManager->ChangeScene();
		
		pO = true;
	}
	else
		pO = false;

	if (gDInput->keyDown(DIK_1))
	{
		if (!p1)
			mSceneManager->GetActiveScene()->DEBUGSPECULARPOWER(1);
		p1 = true;
	}
	else
		p1 = false;

	if (gDInput->keyDown(DIK_2))
	{
		if (!p2)
			mSceneManager->GetActiveScene()->DEBUGSPECULARPOWER(2);
		p2 = true;
	}
	else
		p2 = false;

	if (gDInput->keyDown(DIK_3))
	{
		if (!p3)
			mSceneManager->GetActiveScene()->DEBUGSPECULARPOWER(3);
		p3 = true;
	}
	else
		p3 = false;

	if (gDInput->keyDown(DIK_4))
	{
		if (!p4)
			mSceneManager->GetActiveScene()->DEBUGSPECULARPOWER(4);
		p4 = true;
	}
	else
		p4 = false;

	if (gDInput->keyDown(DIK_5))
	{
		if (!p5)
			mSceneManager->GetActiveScene()->DEBUGSPECULARPOWER(5);
		p5 = true;
	}
	else
		p5 = false;
	
	if (gDInput->keyDown(DIK_6))
	{
		if (!p6)
			mSceneManager->GetActiveScene()->DEBUGSPECULARPOWER(6);
		p6 = true;
	}
	else
		p6 = false;

	if (gDInput->keyDown(DIK_7))
	{
		if (!p7)
			mSceneManager->GetActiveScene()->DEBUGSPECULARPOWER(7);
		p7 = true;
	}
	else
		p7 = false;

	mSceneManager->GetActiveScene()->Update(dt);
	//mSceneManager->Update(dt);
}

void SkeletonClass::drawScene()
{
	mSceneManager->GetActiveScene()->Render(gd3dDevice);
}

void SkeletonClass::buildViewMtx()
{
    
}

void SkeletonClass::buildProjMtx()
{
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
    D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
	mSceneManager->GetActiveScene()->getActiveCamera()->setProjection(proj);
}
