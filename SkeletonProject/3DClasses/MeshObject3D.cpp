#include "MeshObject3D.h"
#include "Vertex.h"
#include "../Materials/BaseMaterial.h"
#include "../GfxStats.h"

MeshObject3D::MeshObject3D(BaseMaterial* mat, ID3DXEffect* effect) : BaseObject3D(mat, effect)
{
}

BaseMaterial* MeshObject3D::getMaterial(void)
{
	return m_Material;
}

void MeshObject3D::Create(IDirect3DDevice9* gd3dDevice)
{
    buildVertexBuffer( gd3dDevice);
    buildIndexBuffer( gd3dDevice);
    buildUVBuffer( gd3dDevice);

    LPD3DXMESH temp = 0;
    D3DVERTEXELEMENT9 e[MAX_FVF_DECL_SIZE];
    UINT eCount = 0;
    HR(VertexPosNM::Decl->GetDeclaration(e, &eCount));
    HR(m_Mesh->CloneMesh(D3DXMESH_MANAGED, e, gd3dDevice, &temp));
    HR(D3DXComputeTangentFrameEx(temp, D3DDECLUSAGE_NORMAL, 0, D3DDECLUSAGE_TEXCOORD, 0, D3DDECLUSAGE_TANGENT, 0, D3DDECLUSAGE_BINORMAL, 0, 0, 0, 0.1f, 0.25f, 0.01f, &m_Mesh, 0));
    ReleaseCOM(temp);
}

void MeshObject3D::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection, LightSceneNode* light)
{
    // Update the statistics singlton class
    GfxStats::GetInstance()->addVertices(m_VertexCount);
    GfxStats::GetInstance()->addTriangles(m_TriCount);
    
    if(!m_Mesh)
        printf("Error: no mesh\n");
    if(!m_Material)
        printf("Error: no material\n");
    D3DXVECTOR3 vpos, vscale;
    D3DXQUATERNION quat;
    D3DXMATRIX iview;
    D3DXMatrixInverse(&iview, 0, &view);
    HR(D3DXMatrixDecompose(&vscale, &quat, &vpos, &view));
    // Set matrices and model relevant render date
    D3DXMATRIX vp = view * projection;
    unsigned passes = m_Material->PreRender();
    for(unsigned i = 0; i < passes; ++i)
    {
        m_Material->Render(world, vp, D3DXVECTOR4(vpos), i, light);

        HR(m_Mesh->DrawSubset(0));
        m_Material->PostPass();
    }
    m_Material->PostRender();
}
