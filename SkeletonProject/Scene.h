#ifndef SCENE_H
#define SCENE_H
#include <d3dx9.h>
#include <set>
#include <stack>

class SceneNode;
class CameraSceneNode;

class Scene {
public:
    Scene(const char* filepath, ID3DXEffect* effect);
    ~Scene();
    void Update(float deltatime);
    void Render(IDirect3DDevice9* gd3dDevice);

    bool loadLevel(const char* filepath, ID3DXEffect* effect);

    bool addNode(SceneNode* node, SceneNode* parent = 0);
    bool removeNode(SceneNode* node);
    bool containsNode(SceneNode* node) const;

    bool setActiveCamera(CameraSceneNode* camera);

    void pushMatrix(D3DXMATRIX matrix);
    void popMatrix();
    D3DXMATRIX getTopMatrix();
    D3DXMATRIX getView();
    D3DXMATRIX getProjection();
protected:
    SceneNode* m_RootNode;
    CameraSceneNode* m_ActiveCamera;
    std::set<SceneNode*> m_Nodes;
    std::stack<D3DXMATRIX> m_MatrixStack;
};
#endif
