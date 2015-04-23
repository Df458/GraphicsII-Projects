uniform extern float4x4 matWorld;
uniform extern float4x4 matITWorld;
uniform extern float4x4 matVP;

uniform extern float3   vLightPos;
uniform extern float3   vAttenuation;
uniform extern float3   vViewPos;

uniform extern float4   colLight;
uniform extern float4   colAmbient;
uniform extern float4   colDiffuse;
uniform extern float4   colSpecular;
uniform extern float    valShininess;

uniform extern texture  Texture;
uniform extern texture  NormalTexture;
uniform extern texture  SkyTexture;

uniform extern int		ToggleTexture;
uniform extern int		ToggleReflection;
uniform extern int		ToggleNormal;
uniform extern int		SpecularPower;
uniform extern float    AmbientCoef;
uniform extern float    DiffuseCoef;
uniform extern float    SpecularCoef;
uniform extern float    ReflectionCoef;
uniform extern float	NormalStr;


sampler sstate = sampler_state {
    Texture = <Texture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler normalbump = sampler_state {
    Texture = <NormalTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

samplerCUBE skysampler = sampler_state {
      Texture   = <SkyTexture>;
      MinFilter = LINEAR;
      MagFilter = LINEAR;
      MipFilter = LINEAR;
      AddressU  = WRAP;
      AddressV  = WRAP;
};

struct OutputVSG
{
    float4 pos : POSITION0;
    float4 color : COLOR0;
    float4 spec : COLOR1;
    float2 uv : TEXCOORD0;
};

struct OutputVS //vertex shader output
{
	float4 A        : COLOR0;
    float4 pos      : POSITION0;
	float3 light	: TEXCOORD0;
	float3 normal	: TEXCOORD1;
	float3 view		: TEXCOORD2;
	float2 uv		: TEXCOORD3;
};

OutputVS PhongVS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD0, float3 Tangent : TANGENT)
{
	//Set inital output to 0
	OutputVS outVS = (OutputVS)0;
	
	//Make the matrix for in world view * Projection
	float4x4 matWorldViewProj = mul(matWorld, matVP);

	// Transform the position 
	outVS.pos = mul(pos, matWorldViewProj);

	// Find position in world space
	float3 worldPos = mul(pos, matWorld).xyz;

	outVS.uv = uv;
	return outVS;
}

float4 PhongPS(OutputVS input) : COLOR
{
	float4 TextureColor = tex2D(sstate, input.uv);

	return TextureColor;
}

technique PhongWire
{
	pass P0
	{
		vertexShader = compile vs_3_0 PhongVS();
		pixelShader = compile ps_3_0 PhongPS();
		FillMode = Wireframe;
	}
}

technique PhongSolid
{
	pass P0
	{
		vertexShader = compile vs_3_0 PhongVS();
		pixelShader = compile ps_3_0 PhongPS();
		FillMode = Solid;
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		BlendOp = Add;
	}
}
