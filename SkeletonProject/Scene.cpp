#include "Scene.h"

#include <d3dx9.h>
#include <rapidxml.hpp>
#include "GfxStats.h"
#include "d3dUtil.h"
#include "SceneNodes/SceneNode.h"
#include "SceneNodes/CameraSceneNode.h"
#include "SceneNodes/SkySceneNode.h"
#include "SceneNodes/LightSceneNode.h"
#include "SceneNodes/ModelSceneNode.h"
#include "Materials/BaseMaterial.h"
#include "Utils.h"
#include "ResourceManager.h"

Scene::Scene()
{

};


Scene::Scene(std::string filepath)
{
	m_RootNode = new SceneNode();

	if (!loadScene(filepath))
		fprintf(stderr, "Failed to load scene: %s\n", filepath.c_str());
	else
		printf("Successfully loaded scene: %s\n", filepath.c_str());
}

Scene::~Scene()
{

}

void Scene::Initialize()
{

}

void Scene::Terminate()
{

}

bool Scene::loadScene(std::string filepath)
{
	char* sceneBuffer = gResourceManager->loadSceneData(filepath);
	if (!sceneBuffer)
		return false;

	rapidxml::xml_document<> document;
	document.parse<0>(sceneBuffer);
	rapidxml::xml_node<>* node = document.first_node("scene", 5, false);

	loadNode(node, m_RootNode);

	if (m_ActiveCamera)
		if (m_ActiveFocus)
			m_ActiveCamera->setFocus(m_ActiveFocus);

	return true;
}

void Scene::loadNode(rapidxml::xml_node<>* node, SceneNode* parent)
{
	loadCameraNode(node, parent);
	loadSkyBoxNode(node, parent);
	loadLightNode(node, parent);
	loadModelNode(node, parent);
}

void Scene::loadCameraNode(rapidxml::xml_node<>* node, SceneNode* parent)
{
	for (rapidxml::xml_node<>* camera = node->first_node("camera", 6, false); camera; camera = camera->next_sibling("camera", 6, false))
	{
		CameraSceneNode* cam_node = new CameraSceneNode(camera);
		parent->addChild(cam_node);
		m_Nodes.insert(cam_node);


		if (rapidxml::xml_attribute<>* active = camera->first_attribute("active", 6, false))
			if (!strcmp(active->value(), "true"))
				setActiveCamera(cam_node);

		loadNode(camera, cam_node);
	}
}

void Scene::loadSkyBoxNode(rapidxml::xml_node<>* node, SceneNode* parent)
{
	for (rapidxml::xml_node<>* skybox = node->first_node("skybox", 6, false); skybox; skybox = skybox->next_sibling("skybox", 6, false))
	{
		SkySceneNode* sky_node = new SkySceneNode(skybox);
		parent->addChild(sky_node);
		m_Nodes.insert(sky_node);

		if (rapidxml::xml_node<>* color = node->first_node("sky", 3, false))
		{
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			if (rapidxml::xml_attribute<>* ar = color->first_attribute("r", 1, false))
				r = atof(ar->value());
			if (rapidxml::xml_attribute<>* ag = color->first_attribute("g", 1, false))
				g = atof(ag->value());
			if (rapidxml::xml_attribute<>* ab = color->first_attribute("b", 1, false))
				b = atof(ab->value());
			m_Sky = D3DCOLOR_COLORVALUE(r, g, b, 1.0f);
		}

		bool active = (skybox == node->first_node("skybox", 6, false));
		if (rapidxml::xml_attribute<>* active_a = node->first_attribute("active", 3, false)) {
			active = strcmp(active_a->value(), "false");
		}

		loadNode(skybox, sky_node);
		m_ActiveSky = sky_node;
	}
}

void Scene::loadLightNode(rapidxml::xml_node<>* node, SceneNode* parent)
{
	for (rapidxml::xml_node<>* light = node->first_node("light", 5, false); light; light = light->next_sibling("light", 5, false)) {
		LightSceneNode* light_node = new LightSceneNode(light);
		parent->addChild(light_node);
		m_Nodes.insert(light_node);


		bool active = (light == node->first_node("light", 6, false));
		if (!m_ActiveLight || active)
		{
			m_ActiveLight = light_node;
		}
		

		loadNode(light, light_node);
	}
}

void Scene::loadModelNode(rapidxml::xml_node<>* node, SceneNode* parent)
{
	for (rapidxml::xml_node<>* model = node->first_node("model", 5, false); model; model = model->next_sibling("model", 5, false)) {
		ModelSceneNode* model_node = new ModelSceneNode(model);
		parent->addChild(model_node);
		m_Nodes.insert(model_node);

		if (rapidxml::xml_attribute<>* active = model->first_attribute("focused", 7, false))
			if (!strcmp(active->value(), "true"))
				m_ActiveFocus = model_node;

		loadNode(model, model_node);
	}
}

void Scene::Update(float deltatime)
{
	for (SceneNode* i : m_Nodes)
	{
		i->Update(deltatime);
	}
}

void Scene::Render(IDirect3DDevice9* gd3dDevice)
{
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_Sky, 1.0f, 0));

	HR(gd3dDevice->BeginScene());

	m_RootNode->renderChildren(this, gd3dDevice);

	//// display the render statistics
	GfxStats::GetInstance()->display();


	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

bool Scene::addNode(SceneNode* node, SceneNode* parent /*= 0*/)
{
	if (parent && !containsNode(parent))
	{
		return false;
	}
	else if (!parent)
	{
		parent = m_RootNode;
	}
	parent->addChild(node);
	m_Nodes.insert(node);
	return true;
}

bool Scene::removeNode(SceneNode* node)
{
	if (!containsNode(node))
	{
		return false;
	}
	if (node->m_Parent)
		node->m_Parent->removeChild(node);
	m_Nodes.erase(node);
	return true;
}

bool Scene::containsNode(SceneNode* node)
{
	return m_Nodes.find(node) != m_Nodes.end();
}

void Scene::setActiveCamera(CameraSceneNode* camera)
{
	m_ActiveCamera = camera;
}

void Scene::setActiveSky(SkySceneNode* sky)
{
	m_ActiveSky = sky;
}

void Scene::pushMatrix(D3DXMATRIX matrix)
{
	m_MatrixStack.push(getTopMatrix() * matrix);
}

void Scene::popMatrix()
{
	if (!m_MatrixStack.empty())
	{
		m_MatrixStack.pop();
	}
}

void Scene::updateSize(unsigned int renderWidth, float renderHeight)
{
	m_RenderWidth = renderWidth;
	m_RenderHeight = renderHeight;

	if (m_ActiveCamera)
		m_ActiveCamera->rebuildProjection(m_RenderWidth, m_RenderHeight);

}

D3DXMATRIX Scene::getTopMatrix()
{
	if (m_MatrixStack.empty())
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

void Scene::DEBUGTOGGLETEXTURE()
{

}

void Scene::DEBUGBLENDSPECULARREFLECTION(float increment)
{

}

void Scene::DEBUGTOGGLENORMAL()
{

}

void Scene::DEBUGSPECULARPOWER(int power)
{

}

void Scene::DEBUGNORMALSTRENGTH(float increment)
{

}

void Scene::DEBUGTOGGLEWIREFRAME()
{

}

void Scene::DEBUGTOGGLEREFLECTION()
{

}

