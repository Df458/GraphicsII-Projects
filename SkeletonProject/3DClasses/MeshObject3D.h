#include "../3DClasses/BaseObject3D.h"

class MeshObject3D : public BaseObject3D {
public:
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection);
protected:
    LPD3DXMESH m_Mesh;
    LPD3DXBUFFER m_Buffer;
};
