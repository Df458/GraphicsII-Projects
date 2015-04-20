#include "SceneManager.h"
#include "ResourceManager.h"
#include <rapidxml.hpp>

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Initalize()
{
	activeSceneNumber = 0;
	sceneCount = 0;
}

void SceneManager::Terminate()
{

}

void SceneManager::Update(float deltatime)
{
	for (auto& scene : mScenes)
	{
		scene->Update(deltatime);
	}
}


bool SceneManager::LoadMultiScene(std::string name)
{
	char* multiSceneBuffer = gResourceManager->loadSceneData(name);
	if (!multiSceneBuffer)
		return false;

	rapidxml::xml_document<> document;
	document.parse<0>(multiSceneBuffer);
	rapidxml::xml_node<>* node = document.first_node("multiscene", 10, false);

	for (rapidxml::xml_node<>* scene = node->first_node("scene", 5, false); scene; scene = scene->next_sibling("scene", 5, false))
	{
		if (rapidxml::xml_attribute<>* sceneName = scene->first_attribute("scene-name", 10, false))
			LoadScene(sceneName->value());
	}
	
	return true;
}

void SceneManager::UnloadMultiScene(std::string name)
{

}

void SceneManager::LoadScene(std::string name)
{
	Scene* newScene = new Scene(name);
	mScenes.push_back(newScene);
	sceneCount++;
}

void SceneManager::UnloadScene(std::string name)
{

}

void SceneManager::updateSize(unsigned int renderWidth, float renderHeight)
{
	m_RenderWidth = renderWidth;
	m_RenderHeight = renderHeight;

	for (auto& scene : mScenes)
	{
		scene->updateSize(renderWidth, renderHeight);
	}

}
Scene* SceneManager::GetActiveScene()
{
	return mScenes[activeSceneNumber];
}

unsigned int SceneManager::getSceneID(std::string name)
{
	return activeSceneNumber;
}

Scene* SceneManager::getScene(unsigned int sceneNumber)
{
	return mScenes[sceneNumber];
}

void SceneManager::ChangeScene()
{
	activeSceneNumber++;
	if (activeSceneNumber >= sceneCount)
	{
		activeSceneNumber = 0;
	}
}


void SceneManager::LoadResources()
{

}

void SceneManager::UnloadResources()
{

}