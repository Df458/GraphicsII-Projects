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
	CuboidObject3D* cuboid1 = new CuboidObject3D(1.0f, 1.0f, 1.0f);
	CuboidObject3D* cuboid2 = new CuboidObject3D(2.0f, 2.0f, 2.0f, 2, 2, 2);
	CuboidObject3D* cuboid3 = new CuboidObject3D(3.0f, 3.0f, 3.0f, 3, 3, 3);
	CuboidObject3D* cuboid4 = new CuboidObject3D(4.0f, 4.0f, 4.0f, 4, 4, 4);
	CuboidObject3D* cuboid5 = new CuboidObject3D(5.0f, 5.0f, 5.0f, 5, 5, 5);
	CuboidObject3D* cuboid6 = new CuboidObject3D(6.0f, 6.0f, 6.0f, 6, 6, 6);
	CuboidObject3D* cuboid7 = new CuboidObject3D(7.0f, 7.0f, 7.0f, 7, 7, 7);
	CuboidObject3D* cuboid8 = new CuboidObject3D(8.0f, 8.0f, 8.0f, 8, 8, 8);

	CuboidObject3D* cuboid9 = new CuboidObject3D(1.0f, 1.0f, 2.0f, 1, 2 ,3);
	CuboidObject3D* cuboid10 = new CuboidObject3D(1.0f, 2.0f, 1.0f, 6, 5, 4);
	CuboidObject3D* cuboid11 = new CuboidObject3D(2.0f, 1.0f, 3.0f, 7, 8, 9);
	CuboidObject3D* cuboid12 = new CuboidObject3D(5.0f, 7.0f, 5.0f, 12, 11, 10);
	CuboidObject3D* cuboid13 = new CuboidObject3D(5.0f, 24.0f, 10.0f, 13, 14, 15);
	CuboidObject3D* cuboid14 = new CuboidObject3D(14.0f, 11.0f, 16.0f, 18, 17, 16);
	CuboidObject3D* cuboid15 = new CuboidObject3D(10.0f, 7.0f, 7.0f, 19, 20, 21);
	CuboidObject3D* cuboid16 = new CuboidObject3D(8.0f, 21.0f, 11.0f, 23, 23, 22);

	ConeObject3D* d4 = new ConeObject3D(4, 3, 6);

	ConeObject3D* cone1 = new ConeObject3D(4, 4, 6);
	ConeObject3D* cone2 = new ConeObject3D(4, 5, 6);
	ConeObject3D* cone3 = new ConeObject3D(4, 6, 8);
	ConeObject3D* cone4 = new ConeObject3D(4, 7, 12);
	ConeObject3D* cone5 = new ConeObject3D(4, 8, 15);
	ConeObject3D* cone6 = new ConeObject3D(4, 40, 6);

	DoubleConeObject3D* dcone1 = new DoubleConeObject3D(4, 4, 6, 6);
	DoubleConeObject3D* dcone2 = new DoubleConeObject3D(4, 5, 6, 6);
	DoubleConeObject3D* dcone3 = new DoubleConeObject3D(4, 6, 8, 6);
	DoubleConeObject3D* dcone4 = new DoubleConeObject3D(4, 7, 12, 10);
	DoubleConeObject3D* dcone5 = new DoubleConeObject3D(4, 8, 15, 2);
	DoubleConeObject3D* dcone6 = new DoubleConeObject3D(4, 40, 6, 1);

	CylinderObject3D* c1 = new CylinderObject3D(8, 4, 6);
	CylinderObject3D* c2 = new CylinderObject3D(4, 5, 6);
	CylinderObject3D* c3 = new CylinderObject3D(8, 6, 8);
	CylinderObject3D* c4 = new CylinderObject3D(9, 7, 12);
	CylinderObject3D* c5 = new CylinderObject3D(10, 8, 15);
	CylinderObject3D* c6 = new CylinderObject3D(11, 40, 6);

	UVSphereObject3D* sp1 = new UVSphereObject3D(6, 6, 6);
	UVSphereObject3D* sp2 = new UVSphereObject3D(4, 4, 6);
	UVSphereObject3D* sp3 = new UVSphereObject3D(5, 10, 9);
	UVSphereObject3D* sp4 = new UVSphereObject3D(9, 12, 6);
	UVSphereObject3D* sp5 = new UVSphereObject3D(10, 6, 9);
	UVSphereObject3D* sp6 = new UVSphereObject3D(10, 50, 50);

	TubeObject3D* tube1 = new TubeObject3D(3, 2, 16, 4);
	TubeObject3D* tube2 = new TubeObject3D(5, 1, 4, 10);
	TubeObject3D* tube3 = new TubeObject3D(9, 2, 14, 3);
	TubeObject3D* tube4 = new TubeObject3D(4, 3, 30, 8);
	TubeObject3D* tube5 = new TubeObject3D(5, 4, 9, 6);
	TubeObject3D* tube6 = new TubeObject3D(7, 4, 6, 9);

    tube->Create(gd3dDevice);
	
	tube1->Create(gd3dDevice);
	tube2->Create(gd3dDevice);
	tube3->Create(gd3dDevice);
	tube4->Create(gd3dDevice);
	tube5->Create(gd3dDevice);
	tube6->Create(gd3dDevice);

	floor->Create(gd3dDevice);
	stand1->Create((gd3dDevice));
	cuboid1->Create((gd3dDevice));
	cuboid2->Create((gd3dDevice));
	cuboid3->Create((gd3dDevice));
	cuboid4->Create((gd3dDevice));
	cuboid5->Create((gd3dDevice));
	cuboid6->Create((gd3dDevice));
	cuboid7->Create((gd3dDevice));
	cuboid8->Create((gd3dDevice));

	cuboid9->Create((gd3dDevice));
	cuboid10->Create((gd3dDevice));
	cuboid11->Create((gd3dDevice));
	cuboid12->Create((gd3dDevice));
	cuboid13->Create((gd3dDevice));
	cuboid14->Create((gd3dDevice));
	cuboid15->Create((gd3dDevice));
	cuboid16->Create((gd3dDevice));

	d4->Create((gd3dDevice));

	cone1->Create((gd3dDevice));
	cone2->Create((gd3dDevice));
	cone3->Create((gd3dDevice));
	cone4->Create((gd3dDevice));
	cone5->Create((gd3dDevice));
	cone6->Create((gd3dDevice));

	dcone1->Create((gd3dDevice));
	dcone2->Create((gd3dDevice));
	dcone3->Create((gd3dDevice));
	dcone4->Create((gd3dDevice));
	dcone5->Create((gd3dDevice));
	dcone6->Create((gd3dDevice));

	c1->Create((gd3dDevice));
	c2->Create((gd3dDevice));
	c3->Create((gd3dDevice));
	c4->Create((gd3dDevice));
	c5->Create((gd3dDevice));
	c6->Create((gd3dDevice));

	sp1->Create((gd3dDevice));
	sp2->Create((gd3dDevice));
	sp3->Create((gd3dDevice));
	sp4->Create((gd3dDevice));
	sp5->Create((gd3dDevice));
	sp6->Create((gd3dDevice));

    m_Scene->addNode(new ModelSceneNode(tube, 20, 0, -10));
	m_Scene->addNode(new ModelSceneNode(floor,0,-100,0));
	m_Scene->addNode(new ModelSceneNode(stand1, -75, 25, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid1, 0, 5, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid2, 0, 7, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid3, 0, 10, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid4, 0, 14, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid5, 0, 19, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid6, 0, 25, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid7, 0, 32, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid8, 0, 40, 0));

	m_Scene->addNode(new ModelSceneNode(cuboid8,  0, -5, 10));
	m_Scene->addNode(new ModelSceneNode(cuboid10, 0, -10, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid11, 0, -20, 10));
	m_Scene->addNode(new ModelSceneNode(cuboid12, 15, -34, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid13, 0, -40, 15));
	m_Scene->addNode(new ModelSceneNode(cuboid14, 20, -55, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid15, 30, -60, 0));
	m_Scene->addNode(new ModelSceneNode(cuboid16, 0, -79, 40));

	m_Scene->addNode(new ModelSceneNode(d4, 20, 0, 20));
	m_Scene->addNode(new ModelSceneNode(cone1, 30, 0, 20));
	m_Scene->addNode(new ModelSceneNode(cone2, 40, 0, 20));
	m_Scene->addNode(new ModelSceneNode(cone3, 50, 0, 20));
	m_Scene->addNode(new ModelSceneNode(cone4, 60, 0, 20));
	m_Scene->addNode(new ModelSceneNode(cone5, 70, 0, 20));
	m_Scene->addNode(new ModelSceneNode(cone6, 80, 0, 20));

	m_Scene->addNode(new ModelSceneNode(dcone1, 20, 0, 30));
	m_Scene->addNode(new ModelSceneNode(dcone2, 20, 0, 40));
	m_Scene->addNode(new ModelSceneNode(dcone3, 20, 0, 50));
	m_Scene->addNode(new ModelSceneNode(dcone4, 20, 0, 60));
	m_Scene->addNode(new ModelSceneNode(dcone5, 20, 0, 70));
	m_Scene->addNode(new ModelSceneNode(dcone6, 20, 0, 80));

	m_Scene->addNode(new ModelSceneNode(c1, 30, 0, -20));
	m_Scene->addNode(new ModelSceneNode(c2, 45, 0, -20));
	m_Scene->addNode(new ModelSceneNode(c3, 60, 0, -20));
	m_Scene->addNode(new ModelSceneNode(c4, 80, 0, -20));
	m_Scene->addNode(new ModelSceneNode(c5, 100, 0, -20));
	m_Scene->addNode(new ModelSceneNode(c6, 130, 0, -20));

	m_Scene->addNode(new ModelSceneNode(sp1, -20, 0, 30));
	m_Scene->addNode(new ModelSceneNode(sp2, -20, 0, 50));
	m_Scene->addNode(new ModelSceneNode(sp3, -20, 0, 70));
	m_Scene->addNode(new ModelSceneNode(sp4, -20, 0, 90));
	m_Scene->addNode(new ModelSceneNode(sp5, -20, 0, 110));
	m_Scene->addNode(new ModelSceneNode(sp6, -20, 0, 130));

	m_Scene->addNode(new ModelSceneNode(tube1, -20, -25, 30));
	m_Scene->addNode(new ModelSceneNode(tube2, -20, -25, 50));
	m_Scene->addNode(new ModelSceneNode(tube3, -20, -25, 70));
	m_Scene->addNode(new ModelSceneNode(tube4, -20, -25, 90));
	m_Scene->addNode(new ModelSceneNode(tube5, -20, -25, 110));
	m_Scene->addNode(new ModelSceneNode(tube6, -20, -25, 130));

    // replace or add to the following object creation
    //m_Objects.push_back( new PlainObject3D(15, 15, 15, 15));
    //m_Objects.push_back( new UVSphereObject3D(4.0f, 16, 32));
	//m_Objects.push_back(new CuboidObject3D(1,1,1,2,2,2));
	
    //m_Objects[0]->Create( gd3dDevice );
    //m_Objects[1]->Create( gd3dDevice );
    
    // Hugues Scene Stuff
    ConeObject3D* hcone = new ConeObject3D(5, 16, 1);
    TubeObject3D* htube = new TubeObject3D(5, 1, 16, 8);
    CylinderObject3D* hcyl = new CylinderObject3D(3, 16, 10);
    UVSphereObject3D* hsph = new UVSphereObject3D(7, 8, 32);

    hcone->Create(gd3dDevice);
    htube->Create(gd3dDevice);
    hcyl->Create(gd3dDevice);
    hsph->Create(gd3dDevice);

    auto node = new ModelSceneNode(hcone, 200, 0, 0);
    m_Scene->addNode(node);
    m_Scene->addNode(new ModelSceneNode(htube, 200, 0, 0));
    m_Scene->addNode(new ModelSceneNode(hcyl, 200, 1, 0));
    m_Scene->addNode(new ModelSceneNode(hsph, 200, 16, 0));

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
