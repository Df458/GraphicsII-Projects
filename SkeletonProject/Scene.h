#ifndef SCENE_H
#define SCENE_H
#include <d3dx9.h>
#include <set>
#include <stack>

class SceneNode;
class CameraSceneNode;
class LightSceneNode;

class Scene {
public:
    Scene(const char* filepath, ID3DXEffect* ceffect, ID3DXEffect* teffect);
    ~Scene();
    void Update(float deltatime);
    void Render(IDirect3DDevice9* gd3dDevice);

    bool loadLevel(const char* filepath, ID3DXEffect* ceffect, ID3DXEffect* teffect);

    bool addNode(SceneNode* node, SceneNode* parent = 0);
    bool removeNode(SceneNode* node);
    bool containsNode(SceneNode* node) const;
	SceneNode* getNode(UINT uid);

    bool setActiveCamera(CameraSceneNode* camera);
    LightSceneNode* getActiveLight(void);
	CameraSceneNode* getActiveCamera(void);
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
	void DEBUGCYCLESCENES();

protected:
    SceneNode* m_RootNode;
    SceneNode* m_ActiveFocus = NULL;
    D3DCOLOR m_Sky;
    CameraSceneNode* m_ActiveCamera;
    LightSceneNode* m_ActiveLight;
    std::set<SceneNode*> m_Nodes;
    std::stack<D3DXMATRIX> m_MatrixStack;
    float m_LastWidth = 1;
    float m_LastHeight = 1;
};
#endif
