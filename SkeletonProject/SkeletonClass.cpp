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

    m_Camera = new CameraSceneNode();
    m_Scene = new Scene();
    m_Scene->setActiveCamera(m_Camera);
    //PlainObject3D* plane = new PlainObject3D(15, 15, 15, 15);
    //plane->Create(gd3dDevice);
    //m_Scene->addNode(new ModelSceneNode(plane));
    TubeObject3D* tube = new TubeObject3D(3, 1, 16, 4);
	PlainObject3D* floor = new PlainObject3D(200.0f, 200.0f, 100, 100);
	CuboidObject3D* stand1 = new CuboidObject3D(50.0f, 50.0f, 70.0f, 20, 20, 20);

	CuboidObject3D* cuboid1 = new CuboidObject3D(1.0f, 1.0f, 1.0f, 1, 1, 1);
    tube->Create(gd3dDevice);
	floor->Create(gd3dDevice);
	stand1->Create((gd3dDevice));

	//floor->Create(gd3dDevice);
    m_Scene->addNode(new ModelSceneNode(tube, 20, 0, -10));
	m_Scene->addNode(new ModelSceneNode(floor,0,-10,0));
	m_Scene->addNode(new ModelSceneNode(stand1, -75, 25, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid1, 20,20,20));

    // replace or add to the following object creation
    //m_Objects.push_back( new PlainObject3D(15, 15, 15, 15));
    //m_Objects.push_back( new UVSphereObject3D(4.0f, 16, 32));
	//m_Objects.push_back(new CuboidObject3D(1,1,1,2,2,2));
	
    //m_Objects[0]->Create( gd3dDevice );
    //m_Objects[1]->Create( gd3dDevice );
    
    // Hugues Scene Stuff

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
}

void SkeletonClass::onResetDevice()
{
	GfxStats::GetInstance()->onResetDevice();

	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	buildProjMtx();
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
	if( gDInput->keyDown(DIK_W) )	 
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
#ifdef __GNUC__
		m_Camera->Rotate((gDInput->mouseDX() / md3dPP.BackBufferWidth)  * dt * 400, 0, 0.0f, true);
		m_Camera->Rotate(0, (gDInput->mouseDY() / md3dPP.BackBufferHeight)  * dt * 100, 0.0f, true);
#else
		m_Camera->Rotate(gDInput->mouseDX()  * dt * 12, 0, 0.0f, true);
		m_Camera->Rotate(0, gDInput->mouseDY()  * dt * 12, 0.0f, true);
#endif

		m_Camera->SetRotationLimits(0, (float)M_PI, -(float)M_PI / 2, (float)M_PI / 2, 0, 0);
	}

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
