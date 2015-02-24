#include "MeshObject3D.h"
#include "../Materials/BaseMaterial.h"
#include "../GfxStats.h"

MeshObject3D::MeshObject3D(BaseMaterial* mat, ID3DXEffect* effect) : BaseObject3D(mat, effect)
{
}

void MeshObject3D::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection) {
    // Update the statistics singlton class
    GfxStats::GetInstance()->addVertices(m_VertexCount);
    GfxStats::GetInstance()->addTriangles(m_TriCount);
    
    if(!m_Mesh)
        printf("Error: no mesh\n");
    if(!m_Material)
        printf("Error: no material\n");
    // Set matrices and model relevant render date
    D3DXMATRIX vp = view * projection;
    unsigned passes = m_Material->PreRender();
    for(unsigned i = 0; i < passes; ++i)
    {
        m_Material->Render(world, vp, i);

        HR(m_Mesh->DrawSubset(0));
        m_Material->PostPass();
    }
    m_Material->PostRender();
}
