#include "MeshObject3D.h"
#include "Vertex.h"
#include "../Materials/BaseMaterial.h"
#include "../GfxStats.h"

MeshObject3D::MeshObject3D()
{
}

void MeshObject3D::Create(IDirect3DDevice9* gd3dDevice)
{
	buildMeshBuffers(gd3dDevice);
    buildUVBuffer( gd3dDevice);

    LPD3DXMESH temp = 0;
    D3DVERTEXELEMENT9 e[MAX_FVF_DECL_SIZE];
    UINT eCount = 0;
    HR(VertexPosNM::Decl->GetDeclaration(e, &eCount));
    HR(m_Mesh->CloneMesh(D3DXMESH_MANAGED, e, gd3dDevice, &temp));
	HR(D3DXComputeTangentFrameEx(temp, D3DDECLUSAGE_TEXCOORD, 0, D3DDECLUSAGE_TANGENT, 0, D3DDECLUSAGE_BINORMAL, 0, D3DDECLUSAGE_NORMAL, 0, 0, 0, 0.1f, 0.25f, 0.01f, &m_Mesh, 0));
    ReleaseCOM(temp);
}

void MeshObject3D::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& world, D3DXMATRIX& cview, D3DXMATRIX& view, D3DXMATRIX& projection, LightSceneNode* light, IDirect3DCubeTexture9* cube, BaseMaterial* material)
{
    // Update the statistics singleton class
    GfxStats::GetInstance()->addVertices(m_VertexCount);
    GfxStats::GetInstance()->addTriangles(m_TriCount);
    
    if(!m_Mesh)
        printf("Error: no mesh\n");
    if(!material)
        printf("Error: no material\n");
    D3DXVECTOR3 vscale;
    D3DXQUATERNION quat;
    D3DXMATRIX iview;
    D3DXMatrixInverse(&iview, 0, &view);
    HR(D3DXMatrixDecompose(&vscale, &quat, &vpos, &iview));
	OutputDebugString((std::to_string(vpos.x) + "\n").c_str());
    // Set matrices and model relevant render date
    D3DXMATRIX vp = view * projection;
    unsigned passes = material->PreRender();
    for(unsigned i = 0; i < passes; ++i)
    {
        material->Render(world, vp, D3DXVECTOR4(vpos), i, light, cube);

        HR(m_Mesh->DrawSubset(0));
        material->PostPass();
    }
    material->PostRender();
}
