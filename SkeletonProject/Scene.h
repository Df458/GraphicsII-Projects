#ifndef SCENE_H
#define SCENE_H
#include <d3dx9.h>
#include <set>
#include <stack>

class SceneNode;
class CameraSceneNode;
class SkySceneNode;
class LightSceneNode;

class Scene {
public:
    Scene(const char* filepath, ID3DXEffect* effect);
    ~Scene();
    void Update(float deltatime);
    void Render(IDirect3DDevice9* gd3dDevice);

    bool loadLevel(const char* filepath, ID3DXEffect* effect);

	void clear(void);

    bool addNode(SceneNode* node, SceneNode* parent = 0);
    bool removeNode(SceneNode* node);
    bool containsNode(SceneNode* node) const;
	SceneNode* getNode(UINT uid);

    bool setActiveCamera(CameraSceneNode* camera);
    bool setActiveSky(SkySceneNode* sky);
    LightSceneNode* getActiveLight(void);
	CameraSceneNode* getActiveCamera(void);
	SkySceneNode* getActiveSky(void) { return m_ActiveSky; }
    SceneNode* getActiveFocus(void) { return m_ActiveFocus; }
	SceneNode* getRootNode(void);

    void pushMatrix(D3DXMATRIX matrix);
    void popMatrix();
    void updateSize(float w, float h);
    D3DXMATRIX getTopMatrix();
    D3DXMATRIX getView();
    D3DXMATRIX getProjection();

	void DEBUGTOGGLETEXTURE();
	void DEBUGTOGGLESPECULAR();
	void DEBUGTOGGLEDIFFUSE();
	void DEBUGTOGGLEWIREFRAME();
	void DEBUGTOGGLETYPE();
	void DEBUGCYCLESCENES();

protected:
    SceneNode* m_RootNode;
    SceneNode* m_ActiveFocus = NULL;
    D3DCOLOR m_Sky;
    CameraSceneNode* m_ActiveCamera = NULL;
    LightSceneNode* m_ActiveLight = NULL;
    SkySceneNode* m_ActiveSky = NULL;
    std::set<SceneNode*> m_Nodes;
    std::stack<D3DXMATRIX> m_MatrixStack;
    float m_LastWidth = 1;
    float m_LastHeight = 1;
};
#endif
