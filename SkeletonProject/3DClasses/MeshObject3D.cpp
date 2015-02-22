#include "MeshObject3D.h"
#include "../GfxStats.h"

void MeshObject3D::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection) {
    // Update the statistics singlton class
    GfxStats::GetInstance()->addVertices(m_VertexCount);
    GfxStats::GetInstance()->addTriangles(m_TriCount);

    HR(m_Mesh->DrawSubset(0));
}
