#include "MeshObject3D.h"
#include "../GfxStats.h"

MeshObject3D::MeshObject3D() : BaseObject3D()
{
    m_Material = new D3DMATERIAL9;
    m_Material->Ambient = {1, 0, 1, 1};
    m_Material->Diffuse = {0, 0, 0, 1};
    m_Material->Specular = {1, 1, 1, 1};
}

void MeshObject3D::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& world, D3DXMATRIX& view, D3DXMATRIX& projection) {
    // Update the statistics singlton class
    GfxStats::GetInstance()->addVertices(m_VertexCount);
    GfxStats::GetInstance()->addTriangles(m_TriCount);
    // Set matrices and model relevant render date
	HR(gd3dDevice->SetTransform(D3DTS_WORLD, &world));
	HR(gd3dDevice->SetTransform(D3DTS_VIEW, &view));
	HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &projection));	
    HR(gd3dDevice->SetMaterial(m_Material));

    HR(m_Mesh->DrawSubset(0));
}
