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

uniform extern bool ToggleTexture;
uniform extern bool ToggleWireframe;
uniform extern bool ToggleSpecular;
uniform extern bool ToggleDiffuse;

struct OutputVS //vertex shader output
{
	float4 pos		: POSITION0;
	float3 light	: TEXCOORD0;
	float3 normal	: TEXCOORD1;
	float3 view		: TEXCOORD2;
};

OutputVS PhongVS(float4 pos : POSITION0, float3 normal : NORMAL0)
{
	OutputVS outVS = (OutputVS)0; //Set inital output to 0
	float4x4 matFinal = mul(matWorld, matVP);
	outVS.pos = mul(pos, matFinal);//Transforms normals to world space
	outVS.light = vLightPos; // Sets the light vector
	outVS.view = vViewPos - normalize(mul(pos, matWorld));// Sets the view vector
	outVS.normal = mul(normal, matWorld);// Sets the normal vector

	return outVS;
}

float4 PhongPS(OutputVS input) : COLOR
{
	float3 Normal = normalize(input.normal);
	float3 LightDirection = normalize(input.light);
	float3 ViewDirection = normalize(input.view);
	float4 Diffuse = saturate(dot(Normal, LightDirection));
	float4 Shadow = saturate(4 * Diffuse);

	float3 Reflect = normalize(2 * Diffuse * Normal - LightDirection); // Calculate reflections
	float4 Specular = pow(saturate(dot(Reflect, -ViewDirection)), 8); // Calculate Specular (Relections.ViewDirection)^8

	//Specular = float4(0, 0, 0, 0);
	return colAmbient + Shadow * (colDiffuse * Diffuse + Specular);
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


