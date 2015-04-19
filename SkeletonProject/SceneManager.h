/************************************************************************
							SceneManager.h
Author: Johnathan O'Malia © 2015
Description:	
				Provide high level scene management.
				Ensure clean up of removed levels.
				Interface with resource manager for appropriate asset management

TODO:
Defaults.
Switching multiscenes
Unloading multiscenes 
Clean up/ deletion of data

/************************************************************************/
#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

//C++ Headers
#include <string>
#include <map>
#include <vector>
#include <inttypes.h>

//Custom Data Headers
#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Initalize();
	void Terminate();

	bool LoadMultiScene(std::string name);
	void UnloadMultiScene(std::string name);
	
	Scene* GetActiveScene();

	unsigned int getSceneID(std::string name);
	Scene* getScene(unsigned int sceneNumber);

	void ChangeScene();
	void updateSize(unsigned int renderWidth, float renderHeight);
private:
	void LoadResources();
	void UnloadResources();

	void LoadScene(std::string name);
	void UnloadScene(std::string name);

	std::vector<std::string> ActiveResources;

	int sceneCount;
	int activeSceneNumber;

	std::vector<Scene*> mScenes;

	float m_RenderWidth = 1;
	float m_RenderHeight = 1;
};

#endif // !SCENE_MANAGER_H_