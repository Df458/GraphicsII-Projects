#include "SkyBoxMaterial.h"

SkyBoxMaterial::SkyBoxMaterial(const char* texture_name)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, texture_name, &m_Texture));
}

void SkyBoxMaterial::ConnectToEffect( ID3DXEffect* effect )
{
    m_Effect = effect;
    if(!m_Effect)
        printf("Error: trying to connect a null effect\n");
    m_WVPMatHandle = effect->GetParameterByName(0, "matWVP");
	m_TextureHandle = effect->GetParameterByName(0, "SkyTexture");
}

void SkyBoxMaterial::Render( D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR4 viewer_pos, unsigned pass, LightSceneNode* light )
{
    D3DXMatrixMultiply(&m_WVPMat, &viewProjMat, &worldMat);
	HR(m_Effect->SetMatrix(m_WVPMatHandle, &m_WVPMat));
	if (m_Texture != nullptr)
		HR(m_Effect->SetTexture(m_TextureHandle, m_Texture));
	HR(m_Effect->CommitChanges());
	HR(m_Effect->BeginPass(pass));
}
