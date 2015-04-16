/************************************************************************
							SceneManager.h
Author: Johnathan O'Malia © 2015
Description:	
				Provide high level scene management.
				Ensure clean up of removed levels.
				Interface with resource manager for appropriate asset management

TODO:
Everything

/************************************************************************/
#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

//C++ Headers
#include <map>
#include <string>
#include <vector>

//Custom Data Headers
#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void LoadScene(std::string name);
	void UnloadScene(std::string name);

	unsigned int getSceneID(std::string name);
	Scene getScene(std::string name);
	Scene getScene(unsigned int sceneID);

private:
	void LoadResource();
	std::vector<std::string> ActiveResources

};

#endif // !SCENE_MANAGER_H_