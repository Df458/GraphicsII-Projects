/************************************************************************
							Scene.h
Author: Johnathan O'Malia, Hugues Ross © 2015
Description:	Mid Level Data structure representing scene data

TODO:
cleanup/deletion of variables
Defaults.
Add XML childing support

/************************************************************************/
#ifndef SCENE_H_
#define SCENE_H_

//C++ Headers
#include <set>
#include <stack>
#include <string>

//RapidXML
#include <rapidxml.hpp>

//DirectX 9
#include <d3dx9.h>

class SceneNode;
class CameraSceneNode;
class SkySceneNode;
class LightSceneNode;

class Scene
{
public:
	Scene();
	Scene(std::string filepath);
	~Scene();

	void				Initialize();
	void				Terminate();

	bool				loadScene(std::string filepath);

	void				Update(float deltatime);
	void				Render(IDirect3DDevice9* gd3dDevice);

	bool				addNode(SceneNode* node, SceneNode* parent = 0);
	bool				removeNode(SceneNode* node);
	bool				containsNode(SceneNode* node);

	//SceneNode*			getNode(UINT uid);

	void				setActiveCamera(CameraSceneNode* camera);
	void				setActiveSky(SkySceneNode* sky);

	LightSceneNode*		getActiveLight(void) { return m_ActiveLight; };
	CameraSceneNode*	getActiveCamera(void) { return m_ActiveCamera; };
	SkySceneNode*		getActiveSky(void) { return m_ActiveSky; };
	SceneNode*			getActiveFocus(void) { return m_ActiveFocus; };
	SceneNode*			getRootNode(void) { return m_RootNode; };
	
	void				pushMatrix(D3DXMATRIX matrix);
	void				popMatrix();
	void				updateSize(unsigned int renderWidth, float renderHeight);

	D3DXMATRIX			getTopMatrix();
	D3DXMATRIX			getView();
	D3DXMATRIX			getProjection();

	void DEBUGTOGGLETEXTURE();
	void DEBUGBLENDSPECULARREFLECTION(float increment);
	void DEBUGTOGGLENORMAL();
	void DEBUGSPECULARPOWER(int power);
	void DEBUGNORMALSTRENGTH(float increment);
	void DEBUGTOGGLEWIREFRAME();
	void DEBUGTOGGLEREFLECTION();

private:
	void loadNode(rapidxml::xml_node<>* node, SceneNode* parent);
	void loadCameraNode(rapidxml::xml_node<>* node, SceneNode* parent);
	void loadSkyBoxNode(rapidxml::xml_node<>* node, SceneNode* parent);
	void loadLightNode(rapidxml::xml_node<>* node, SceneNode* parent);
	void loadModelNode(rapidxml::xml_node<>* node, SceneNode* parent);
	void loadEmitterNode(rapidxml::xml_node<>* node, SceneNode* parent);

	SceneNode* m_RootNode;
	SceneNode* m_ActiveFocus = NULL;
	CameraSceneNode* m_ActiveCamera = NULL;
	LightSceneNode* m_ActiveLight = NULL;
	SkySceneNode* m_ActiveSky = NULL;

	D3DCOLOR m_Sky;
	
	std::set<SceneNode*> m_Nodes;
	std::stack<D3DXMATRIX> m_MatrixStack;

	float m_RenderWidth = 1;
	float m_RenderHeight = 1;
};



#endif // !SCENE_H_