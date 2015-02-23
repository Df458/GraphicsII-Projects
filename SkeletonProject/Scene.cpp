#include <d3dx9.h>
#include "GfxStats.h"
#include "d3dUtil.h"
#include "Scene.h"
#include "SceneNodes/SceneNode.h"
#include "SceneNodes/CameraSceneNode.h"

Scene::Scene()
{
    m_RootNode = new SceneNode();
    //LPD3DXBUFFER error_buf;
    //HR(D3DXCreateEffectFromFile(gd3dDevice, "testShader.fx", NULL, NULL, 0, NULL, &m_Effect, &error_buf))
    //if(error_buf)
        //fprintf(stderr, "Errors:\n%s\n", (char*)error_buf->GetBufferPointer());
}

Scene::~Scene()
{
}

void Scene::Update(float deltatime)
{
    for(SceneNode* i : m_Nodes)
    {
        i->Update(deltatime);
    }
}

void Scene::Render(IDirect3DDevice9* gd3dDevice)
{
	// Clear the backbuffer and depth buffer.
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));

	HR(gd3dDevice->BeginScene());

    //D3DXHANDLE tech = m_Effect->GetTechniqueByName("TestTechnique");
    //HR(m_Effect->SetTechnique(tech));
    //UINT passes;
    //HR(m_Effect->Begin(&passes, 0));
    //for(unsigned i = 0; i < passes; ++i) {

        // Render all the objects
        m_RootNode->renderChildren(this, gd3dDevice);

    //}
    //HR(m_Effect->End());
    // display the render statistics
    GfxStats::GetInstance()->display();


	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

bool Scene::addNode(SceneNode* node, SceneNode* parent)
{
    if(parent && !containsNode(parent))
    {
        return false;
    }
    else if(!parent)
    {
        parent = m_RootNode;
    }
    parent->addChild(node);
    m_Nodes.insert(node);
    return true;
}

bool Scene::removeNode(SceneNode* node)
{
    if(!containsNode(node))
    {
        return false;
    }
    node->m_Parent->removeChild(node);
    m_Nodes.erase(node);
    return true;
}

bool Scene::containsNode(SceneNode* node) const
{
    return m_Nodes.find(node) != m_Nodes.end();
}

bool Scene::setActiveCamera(CameraSceneNode* camera)
{
    if(!containsNode(camera) && !addNode(camera))
        return false;
    m_ActiveCamera = camera;
    return true;
}

void Scene::pushMatrix(D3DXMATRIX matrix)
{
    m_MatrixStack.push(getTopMatrix() * matrix);
}

void Scene::popMatrix()
{
    if(!m_MatrixStack.empty())
    {
        m_MatrixStack.pop();
    }
}

D3DXMATRIX Scene::getTopMatrix()
{
    if(m_MatrixStack.empty())
    {
        D3DXMATRIX ret;
        D3DXMatrixIdentity(&ret);
        return ret;
    }
    
    return m_MatrixStack.top();
}

D3DXMATRIX Scene::getView()
{
    return m_ActiveCamera->getView();
}

D3DXMATRIX Scene::getProjection()
{
    return m_ActiveCamera->getProjection();
}
