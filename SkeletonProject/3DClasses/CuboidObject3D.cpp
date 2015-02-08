#include "CuboidObject3D.h"


CuboidObject3D::CuboidObject3D(float length, float width, float height, unsigned X_Resolution, unsigned Y_Resolution, unsigned Z_Resolution)
	:BaseObject3D(),m_Length(length), m_Width(width), m_Height(height), mX_Resolution(X_Resolution), mY_Resolution(Y_Resolution), mZ_Resolution(Z_Resolution)
{
	//Calculate Vertex Count

	//Calculate Index Count
}

CuboidObject3D::CuboidObject3D(float size, unsigned X_Resolution, unsigned Y_Resolution, unsigned Z_Resolution)
	:CuboidObject3D(size, size, size, X_Resolution, Y_Resolution, Z_Resolution)
{
}

void CuboidObject3D::Create(IDirect3DDevice9* gd3dDevice)
{

}

void CuboidObject3D::buildVertexBuffer(IDirect3DDevice9* gd3dDevice) {

}

void CuboidObject3D::buildIndexBuffer(IDirect3DDevice9* gd3dDevice) {
}