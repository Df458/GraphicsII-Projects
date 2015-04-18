#include "SkyBoxMaterial.h"
#include "../ResourceManager.h"
#include "../Scene.h"

SkyBoxMaterial::SkyBoxMaterial(ID3DXEffect* pEffect, IDirect3DCubeTexture9* pTexture) : BaseMaterial(nullptr,pEffect)
{
	m_CubeTexture = pTexture;
	ConnectToEffect(pEffect);
}

void SkyBoxMaterial::ConnectToEffect( ID3DXEffect* effect )
{
    m_Effect = effect;
    if(!m_Effect)
        printf("Error: trying to connect a null effect\n");
	m_VPMatHandle = m_Effect->GetParameterByName(0, "matVP");
	m_TextureHandle = m_Effect->GetParameterByName(0, "SkyTexture");
	m_viewHandle = m_Effect->GetParameterByName(0, "view");
	m_Technique = m_Effect->GetTechniqueByName("SkyTech");
	tech = "SkyTech";
}

void SkyBoxMaterial::Render(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR4 viewer_pos, unsigned pass, Scene* scene)
{
	D3DXMatrixMultiply(&m_VPMat, &worldMat, &viewProjMat);
	HR(m_Effect->SetMatrix(m_VPMatHandle, &m_VPMat));
	HR(m_Effect->SetVector(m_viewHandle, &viewer_pos));
	//HR(m_Effect->SetTexture(m_TextureHandle, m_CubeTexture));
	HR(m_Effect->CommitChanges());
	HR(m_Effect->BeginPass(pass));
}
