//=============================================================================
//              Adopted from Franc Luna's CubeDemo sample project.
//
// Please use this file to add classes / inherit and create various vertex classes
//=============================================================================
#ifndef VERTEX_H
#define VERTEX_H
//=============================================================================
#include <d3dx9.h>
//=============================================================================
// Call in constructor and destructor, respectively, of derived application class.
void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();
//=============================================================================
struct VertexPos
{
	VertexPos():pos(0.0f, 0.0f, 0.0f),normal(0.0f, 0.0f, 0.0f),uv(0.0f, 0.0f),tangent(0.0f, 0.0f, 0.0f),binormal(0.0f, 0.0f, 0.0f){}
	VertexPos(float x, float y, float z, float nx = 0, float ny = 0, float nz = 0, float ux = 0, float uy = 0, float tx = 0, float ty = 0, float tz = 0, float bx = 0, float by = 0, float bz = 0):pos(x,y,z),normal(nx,ny,nz),uv(ux,uy),tangent(tx, ty, tz),binormal(bx, by, bz){}
	VertexPos(const D3DXVECTOR3& v):pos(v){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 binormal;
	D3DXVECTOR2 uv;
	static IDirect3DVertexDeclaration9* Decl;
};
//=============================================================================
#endif // VERTEX_H
