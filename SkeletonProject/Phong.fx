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

uniform extern int ToggleTexture;
uniform extern int ToggleSpecular;
uniform extern int ToggleDiffuse;


uniform extern texture  Texture;

sampler sstate = sampler_state {
	Texture = <Texture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct OutputVS //vertex shader output
{
	float4 pos		: POSITION0;
	float3 light	: TEXCOORD0;
	float3 normal	: TEXCOORD1;
	float3 view		: TEXCOORD2;
	float2 uv		: TEXCOORD3;
};

OutputVS PhongVS(float4 pos : POSITION0, float3 normal : NORMAL0, float2 uv : TEXCOORD0)
{
	//Set inital output to 0
	OutputVS outVS = (OutputVS)0;

	//Make the matrix for in world view * Projection
	float4x4 matWorldViewProj = mul(matWorld, matVP);

	// Transform the position 
	outVS.pos = mul(pos, matWorldViewProj);

	// Find position in world space
	float3 worldPos = mul(pos, matWorld).xyz;

	//  Sets the light vector to the unit vectors of the lighting position - world space
	outVS.light = normalize(vLightPos - worldPos);;

	outVS.view = vViewPos - normalize(mul(pos, matWorld));// Sets the view vector
	outVS.normal = mul(normal, matWorld);// Sets the normal vector

	outVS.uv = uv;
	return outVS;
}

float4 PhongPS(OutputVS input) : COLOR
{
	float3 Normal = normalize(input.normal);
	float3 LightDirection = input.light;
	float3 ViewDirection = normalize(input.view);

	float4 Diffuse = saturate(dot(Normal, LightDirection));
	float4 Shadow = saturate(4 * Diffuse);

	float3 Reflect = normalize(2 * Diffuse * Normal - LightDirection); // Calculate reflections
	float4 Specular = pow(saturate(dot(Reflect, -ViewDirection)), 8); // Calculate Specular (Relections.ViewDirection)^8

	//Specular = float4(0, 0, 0, 0);
	//return colAmbient + Shadow * (colDiffuse * Diffuse + Specular);

	float4 TextureColor = tex2D(sstate, input.uv);
	TextureColor = pow(TextureColor, ToggleTexture);// set to 0 for no texture, 1 for them
	return TextureColor * colAmbient + Shadow * (TextureColor * colDiffuse * Diffuse + Specular);
}

technique PhongWire
{
	pass P0
	{
		vertexShader = compile vs_2_0 PhongVS();
		pixelShader = compile ps_2_0 PhongPS();
		FillMode = Wireframe;
	}
}

technique PhongSolid
{
	pass P0
	{
		vertexShader = compile vs_2_0 PhongVS();
		pixelShader = compile ps_2_0 PhongPS();
		FillMode = Solid;
	}
}


