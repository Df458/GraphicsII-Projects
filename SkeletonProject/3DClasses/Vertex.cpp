//=============================================================================
//              Adopted from Franc Luna's CubeDemo sample project.
//
// Please use this file to add classes / inherit and create various vertex classes
//=============================================================================
#include "Vertex.h"
#include "../d3dUtil.h"
//=============================================================================
// Initialize static variables.
IDirect3DVertexDeclaration9* VertexPos::Decl = 0;
IDirect3DVertexDeclaration9* VertexPosNM::Decl = 0;
//=============================================================================
void InitAllVertexDeclarations()
{
	// VertexPos

	D3DVERTEXELEMENT9 VertexPosElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};	
	HR(gd3dDevice->CreateVertexDeclaration(VertexPosElements, &VertexPos::Decl));
	D3DVERTEXELEMENT9 VertexPosNMElements[] = 
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0, 36,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
        {0, 48,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
		D3DDECL_END()
	};	
	HR(gd3dDevice->CreateVertexDeclaration(VertexPosNMElements, &VertexPosNM::Decl));
}
//-----------------------------------------------------------------------------
void DestroyAllVertexDeclarations()
{
	ReleaseCOM(VertexPos::Decl);
}
//=============================================================================
