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

#include "Scene.h"
#include "SceneNodes/CameraSceneNode.h"
#include "SceneNodes/ModelSceneNode.h"
#include "SkeletonClass.h"
#include "3DClasses/BaseObject3D.h"
#include "3DClasses/ConeObject3D.h"
#include "3DClasses/CuboidObject3D.h"
#include "3DClasses/CylinderObject3D.h"
#include "3DClasses/DoubleConeObject3D.h"
#include "3DClasses/PlainObject3D.h"
#include "3DClasses/TubeObject3D.h"
#include "3DClasses/UVSphereObject3D.h"
#include "3DClasses/Vertex.h"
#include "Materials/BaseMaterial.h"
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

	mCameraRadius    = 10.0f;
	mCameraRotationY = 1.2 * D3DX_PI;
	mCameraHeight    = 5.0f;

    m_Scene = new Scene("TestLevel.xml", m_DefaultEffect);
    m_Scene->updateSize(md3dPP.BackBufferWidth, md3dPP.BackBufferHeight);
	m_Camera = m_Scene->getActiveCamera();
    m_Camera->setFocus(m_Scene->getActiveFocus());

	onResetDevice();

	InitAllVertexDeclarations();
}

SkeletonClass::~SkeletonClass()
{
    GfxStats::DeleteInstance();

    //for ( unsigned int obj=0 ; obj<m_Objects.size() ; obj++ )
        //delete m_Objects[obj];
    //m_Objects.clear();
    delete m_Scene;

	DestroyAllVertexDeclarations();
}

bool SkeletonClass::checkDeviceCaps()
{
	return true;
}

void SkeletonClass::onLostDevice()
{
	GfxStats::GetInstance()->onLostDevice();
	m_DefaultEffect->OnLostDevice();
}

void SkeletonClass::onResetDevice()
{
	GfxStats::GetInstance()->onResetDevice();

	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	//buildProjMtx();
    m_Scene->updateSize(md3dPP.BackBufferWidth, md3dPP.BackBufferHeight);
	m_DefaultEffect->OnResetDevice();
}

void SkeletonClass::updateScene(float dt)
{

	// Reset the statistics for the scene - each object will add to it.
	GfxStats::GetInstance()->setVertexCount(0);
	GfxStats::GetInstance()->setTriCount(0);
	GfxStats::GetInstance()->update(dt);

	// Get snapshot of input devices.
	gDInput->poll();



	/******OLD CAMERA*****/
	// Check input.
	/*if( gDInput->keyDown(DIK_W) )	 
		mCameraHeight   += 25.0f * dt;
	if( gDInput->keyDown(DIK_S) )	 
		mCameraHeight   -= 25.0f * dt;

	// Divide by 50 to make mouse less sensitive. 
	mCameraRotationY += gDInput->mouseDX() / 100.0f;
	mCameraRadius    += gDInput->mouseDY() / 25.0f;

	// If we rotate over 360 degrees, just roll back to 0
	if( fabsf(mCameraRotationY) >= 2.0f * D3DX_PI ) 
		mCameraRotationY = 0.0f;

	// Don't let radius get too small.
	if( mCameraRadius < 5.0f )
		mCameraRadius = 5.0f;

	// The camera position/orientation relative to world space can 
	// change every frame based on input, so we need to rebuild the
	// view matrix every frame with the latest changes.
	buildViewMtx();
	/******OLD CAMERA*****/
	
	if (gDInput->mouseButtonDown(0))
    {
		m_Camera->turnFocus((gDInput->mouseDX()) * DEGTORAD, (gDInput->mouseDY()) * DEGTORAD);
    }

    if(gDInput->mouseDZ() != 0)
        m_Camera->zoomFocus(gDInput->mouseDZ() * 0.01f);
    
	/*
	if (gDInput->mouseButtonDown(0))
	{
#ifdef __GNUC__
		m_Camera->Rotate((gDInput->mouseDX() / md3dPP.BackBufferWidth)  * dt * 400, 0, 0.0f, true);
		m_Camera->Rotate(0, (gDInput->mouseDY() / md3dPP.BackBufferHeight)  * dt * 100, 0.0f, true);
#else
		m_Camera->Rotate(gDInput->mouseDX()  * dt * 12, 0, 0.0f, true);
		m_Camera->Rotate(0, gDInput->mouseDY()  * dt * 12, 0.0f, true);
#endif

		m_Camera->SetRotationLimits(0, (float)M_PI, -(float)M_PI / 2, (float)M_PI / 2, 0, 0);
	}
	*/
    if (gDInput->keyDown(DIK_W))
        m_Camera->Translate(0, 0, 25.0f * dt, true, true);

    if (gDInput->keyDown(DIK_S))
        m_Camera->Translate(0, 0, -25.0f * dt, true, true);

    if (gDInput->keyDown(DIK_A))
        m_Camera->Translate(-25.0f * dt, 0, 0, true, true);

    if (gDInput->keyDown(DIK_D))
        m_Camera->Translate(25.0f * dt, 0, 0, true, true);
    
    if (gDInput->keyDown(DIK_Q))
        m_Camera->Translate(0, -25.0f * dt, 0, true);

    if (gDInput->keyDown(DIK_E))
        m_Camera->Translate(0, 25.0f * dt, 0, true);

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
    printf("done.\n");
}
