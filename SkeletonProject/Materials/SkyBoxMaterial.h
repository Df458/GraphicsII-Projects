#pragma once
#include "BaseMaterial.h"

class SkyBoxMaterial : public BaseMaterial
{
    D3DXMATRIX          m_WVPMat;
    D3DXHANDLE          m_WVPMatHandle;
public:
    SkyBoxMaterial(const char* texture_name);
    virtual void ConnectToEffect( ID3DXEffect* effect );
    virtual void Render( D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR4 viewer_pos, unsigned pass, LightSceneNode* light ); 
};
