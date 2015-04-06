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

#include "ResourceManager.h"
#include "Scene.h"
#include "SceneNodes/CameraSceneNode.h"
#include "SceneNodes/ModelSceneNode.h"
#include "SkeletonClass.h"
#include "3DClasses/BaseObject3D.h"
#include "3DClasses/ConeObject3D.h"
#include "3DClasses/CuboidObject3D.h"
#include "3DClasses/CylinderObject3D.h"
#include "3DClasses/PlainObject3D.h"
#include "3DClasses/TubeObject3D.h"
#include "3DClasses/UVSphereObject3D.h"
#include "3DClasses/Vertex.h"
#include "Materials/BaseMaterial.h"

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
	tmpManager->Initalize();

	gResourceManager = tmpManager;

	mCameraRadius    = 10.0f;
	mCameraRotationY = 1.2 * D3DX_PI;
	mCameraHeight    = 5.0f;

	InitAllVertexDeclarations();

	m_Scene = new Scene("TestLevel.xml", gResourceManager->getDefaultEffect());
    m_Scene->updateSize(md3dPP.BackBufferWidth, md3dPP.BackBufferHeight);
	m_Camera = m_Scene->getActiveCamera();
	currentobj = 0;
	m_Scene->loadLevel(objscenes[currentobj], gResourceManager->getDefaultEffect());
	if (m_Scene->getActiveFocus()) {
		m_Camera->setFocus(m_Scene->getActiveFocus());
	}

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

    //for ( unsigned int obj=0 ; obj<m_Objects.size() ; obj++ )
        //delete m_Objects[obj];
    //m_Objects.clear();
    delete m_Scene;

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
    m_Scene->updateSize(md3dPP.BackBufferWidth, md3dPP.BackBufferHeight);
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
		if (m_Camera->getFocused())
			m_Camera->turnFocus((gDInput->mouseDX()) * DEGTORAD, (gDInput->mouseDY()) * DEGTORAD);
		else
		{
			m_Camera->Rotate((gDInput->mouseDX()) * DEGTORAD, (gDInput->mouseDY()) * DEGTORAD, 0, true);
			m_Camera->SetRotationLimits(0, (float)M_PI, -(float)M_PI / 2, (float)M_PI / 2, 0, 0);
		}
	}

	if (gDInput->mouseDZ() != 0)
		m_Camera->zoomFocus(gDInput->mouseDZ() * 0.0051f);

	if (gDInput->keyDown(DIK_M))
		m_Camera->releaseFocus();
	if (gDInput->keyDown(DIK_N))
		m_Camera->setFocus(m_Scene->getActiveFocus());


	//Assignment 4 Controls
	if (gDInput->keyDown(DIK_ADD))
	{
		if (!pPlus)
			m_Scene->DEBUGBLENDSPECULARREFLECTION(-0.01f);
		pPlus = true;
	}
	else
		pPlus = false;

	if (gDInput->keyDown(DIK_MINUS))
	{
		if (!pMinus)
			m_Scene->DEBUGBLENDSPECULARREFLECTION(0.01f);
		pMinus = true;
	}
	else
		pMinus = false;

	if (gDInput->keyDown(DIK_A))
	{
		if (!pA)
			m_Scene->DEBUGNORMALSTRENGTH(-0.01f);
		pA = true;
	}
	else
		pA = false;

	if (gDInput->keyDown(DIK_S))
	{
		if (!pS)
			m_Scene->DEBUGNORMALSTRENGTH(0.01f);
		pS = true;
	}
	else
		pS = false;

	if (gDInput->keyDown(DIK_W))
	{
		if (!pW)
			m_Scene->DEBUGTOGGLEWIREFRAME();
		pW = true;
	}
	else
		pW = false;

	if (gDInput->keyDown(DIK_T))
	{
		if (!pT)
			m_Scene->DEBUGTOGGLETEXTURE();
		pT = true;
	}
	else
		pT = false;

	if (gDInput->keyDown(DIK_R))
	{
		if (!pR)
			m_Scene->DEBUGTOGGLEREFLECTION();
		pR = true;
	}
	else
		pR = false;

	if (gDInput->keyDown(DIK_N))
	{
		if (!pN)
			m_Scene->DEBUGTOGGLENORMAL();
		pN = true;
	}
	else
		pN = false;

	if (gDInput->keyDown(DIK_O))
	{
		if (!pO) {
			++currentobj;
			if (currentobj >= 6)
				currentobj = 0;
			m_Scene->clear();
			m_Scene->loadLevel(objscenes[currentobj], gResourceManager->getDefaultEffect());
			if (m_Scene->getActiveFocus())
				m_Camera->setFocus(m_Scene->getActiveFocus());
		}
		pO = true;
	}
	else
		pO = false;

	if (gDInput->keyDown(DIK_1))
	{
		if (!p1)
			m_Scene->DEBUGSPECULARPOWER(1);
		p1 = true;
	}
	else
		p1 = false;

	if (gDInput->keyDown(DIK_2))
	{
		if (!p2)
			m_Scene->DEBUGSPECULARPOWER(2);
		p2 = true;
	}
	else
		p2 = false;

	if (gDInput->keyDown(DIK_3))
	{
		if (!p3)
			m_Scene->DEBUGSPECULARPOWER(3);
		p3 = true;
	}
	else
		p3 = false;

	if (gDInput->keyDown(DIK_4))
	{
		if (!p4)
			m_Scene->DEBUGSPECULARPOWER(4);
		p4 = true;
	}
	else
		p4 = false;

	if (gDInput->keyDown(DIK_5))
	{
		if (!p5)
			m_Scene->DEBUGSPECULARPOWER(5);
		p5 = true;
	}
	else
		p5 = false;
	
	if (gDInput->keyDown(DIK_6))
	{
		if (!p6)
			m_Scene->DEBUGSPECULARPOWER(6);
		p6 = true;
	}
	else
		p6 = false;

	if (gDInput->keyDown(DIK_7))
	{
		if (!p7)
			m_Scene->DEBUGSPECULARPOWER(7);
		p7 = true;
	}
	else
		p7 = false;

    m_Scene->Update(dt);
}

void SkeletonClass::drawScene()
{
    m_Scene->Render(gd3dDevice);

     //Clear the backbuffer and depth buffer.
    //HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));

    //HR(gd3dDevice->BeginScene());

     //Set render statws for the entire scene here:
    //HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
    //HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME));

     //Render all the objects
    //for ( unsigned int obj=0 ; obj<m_Objects.size() ; obj++ )
    //{
        //m_Objects[obj]->Render( gd3dDevice, mView, mProj );
    //}

     //display the render statistics
    //GfxStats::GetInstance()->display();

    //HR(gd3dDevice->EndScene());

     //Present the backbuffer.
    //HR(gd3dDevice->Present(0, 0, 0, 0));
}

void SkeletonClass::buildViewMtx()
{
    //D3DXMATRIX view;
    //float x = mCameraRadius * cosf(mCameraRotationY);
    //float z = mCameraRadius * sinf(mCameraRotationY);
    //D3DXVECTOR3 pos(x, mCameraHeight, z);
    //D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
    //D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    //D3DXMatrixLookAtLH(&view, &pos, &target, &up);
    //m_Camera->setView(view);
}

void SkeletonClass::buildProjMtx()
{
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
    D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
    m_Camera->setProjection(proj);
}
