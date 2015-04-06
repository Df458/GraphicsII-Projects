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
	float4 pos		: POSITION0;
	float3 light	: TEXCOORD0;
	float3 normal	: TEXCOORD1;
	float3 view		: TEXCOORD2;
	float2 uv		: TEXCOORD3;
};

OutputVS PhongVS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD0, float3 Tangent : TANGENT)
{
	//Set inital output to 0
	OutputVS outVS = (OutputVS)0;
	
	float3x3 toTangentSpace;
	toTangentSpace[0] = mul(Tangent, matWorld);
	toTangentSpace[1] = mul(cross(Tangent, normal), matWorld);
	toTangentSpace[2] = mul(normal, matWorld);
	
	//Make the matrix for in world view * Projection
	float4x4 matWorldViewProj = mul(matWorld, matVP);

	// Transform the position 
	outVS.pos = mul(pos, matWorldViewProj);

	// Find position in world space
	float3 worldPos = mul(pos, matWorld).xyz;

	//  Sets the light vector to the unit vectors of the lighting position - world space
	outVS.light = normalize(vLightPos - worldPos);
	outVS.light = mul(toTangentSpace, outVS.light);
	
	outVS.view = vViewPos - normalize(mul(pos, matWorld));// Sets the view vector
	outVS.view = mul(toTangentSpace, outVS.view);
	outVS.normal = mul(normal, matWorld);// Sets the normal vector

	outVS.uv = uv;
	return outVS;
}

float4 PhongPS(OutputVS input) : COLOR
{
	//float3 Normal = normalize(input.normal);
	float3 Normal = NormalStr*2*(tex2D(normalbump, input.uv) - 0.5);
	float3 LightDirection = input.light;
	float3 ViewDirection = normalize(input.view);

	float4 Diffuse = saturate(dot(Normal, LightDirection));
	float4 Shadow = saturate(4 * Diffuse);

	float3 Reflect = normalize(2 * Diffuse * Normal - LightDirection); // Calculate reflections
	float4 Specular = pow(saturate(dot(Reflect, -ViewDirection)), SpecularPower); // Calculate Specular (Relections.ViewDirection)^8

	// REFLECT STUFF IS RIGHT HERE!!
	//float3 emt = reflect(-ViewDirection, Normal);
	float4 rcolor = texCUBE(skysampler, Normal);

	
	float4 TextureColor = tex2D(sstate, input.uv);

	//Toggles "Logic"
	//Set ToggleTexture int to 1 to let variables be themselves (Self to 1st power)
	//Set ToggleTexture int to 0 to set variables to 1 (Self to 0th power)
	TextureColor = pow(abs(TextureColor), ToggleTexture);
	
	//Set ToggleDiffuse & ToggleDiffuse int to 1 to let variables be themselves
	//Set ToggleDiffuse & ToggleDiffuse int to 0 to set variables to 0
	//Diffuse = mul(Diffuse, ToggleDiffuse);
	//Specular = mul(Specular, ToggleSpecular);
	rcolor = mul(rcolor, ToggleReflection);
	
	return TextureColor * colAmbient + Shadow * (TextureColor * colDiffuse * Diffuse + rcolor * (ReflectionCoef * 10) + Specular * (SpecularCoef * 10));
	//return TextureColor * colAmbient + (rcolor + Specular);
	//return TextureColor * colAmbient * AmbientCoef + (TextureColor + colDiffuse * Diffuse) * DiffuseCoef + rcolor + Specular * SpecularCoef;
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
	}
}
