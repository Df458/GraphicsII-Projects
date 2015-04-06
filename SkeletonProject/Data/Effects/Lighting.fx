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

uniform extern int ToggleTexture;
uniform extern int ToggleSpecular;
uniform extern int ToggleDiffuse;

sampler sstate = sampler_state {
    Texture = <Texture>;
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

OutputVSG GouraudVert(float3 position : POSITION0, float3 normal : NORMAL0, float2 uv : TEXCOORD0)
{
    float4x4 matFinal = mul(matWorld, matVP);
    OutputVSG outv = (OutputVSG)0;
    outv.pos = mul(float4(position, 1.0f), matFinal);

	// Transform normal to world space.
    float3 wnorm = normalize(mul(normal, matWorld));

	// Transform vertex position to world space.
    float3 worldPos = mul(float4(position, 1.0f), matWorld);

	// Unit vector from vertex to light source.
    float3 wli   = normalize(vLightPos - worldPos);

	// Ambient Light Computation.
    float3 amb  = colAmbient.rgb;

	// Diffuse Light Computation.
    float s = max(dot(wnorm, wli), 0.0f);
    float3 diff = s * colDiffuse.rgb * colLight.rgb;

	// Specular Light Computation.
    float3 wvtvi = normalize(vViewPos - normalize(mul(position, matWorld)));
    float3 reflected = normalize(2 * diff * wnorm - wli);
    float t = pow(saturate(dot(reflected, -wvtvi)), valShininess);
    float3 spec = t * (colSpecular * colLight).rgb;

	float d = distance(vLightPos, worldPos);
	float A = vAttenuation.x + vAttenuation.y*d + vAttenuation.z*d*d;

    outv.color = float4(((amb + diff) / A), colDiffuse.a);
    outv.spec = float4(spec / A, 1);
    outv.uv = uv;
    return outv;
}

float4 GouraudPix(OutputVSG input) : COLOR
{
    float4 tcol = tex2D(sstate, input.uv);
	tcol = pow(abs(tcol), ToggleTexture);
    float4 finalcol = tcol * input.color + input.spec;
    return finalcol;
}


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

	// REFLECT STUFF IS RIGHT HERE!!
	float3 emt = reflect(-ViewDirection, Normal);
	float4 rcolor = texCUBE(skysampler, emt);

	//Specular = float4(0, 0, 0, 0);
	//return colAmbient + Shadow * (colDiffuse * Diffuse + Specular);

	float4 TextureColor = tex2D(sstate, input.uv);

	//Toggles "Logic"
	//Set ToggleTexture int to 1 to let variables be themselves (Self to 1st power)
	//Set ToggleTexture int to 0 to set variables to 1 (Self to 0th power)
	TextureColor = pow(abs(TextureColor), ToggleTexture);

	//Set ToggleDiffuse & ToggleDiffuse int to 1 to let variables be themselves
	//Set ToggleDiffuse & ToggleDiffuse int to 0 to set variables to 0
	Diffuse = mul(Diffuse, ToggleDiffuse);
	Specular = mul(Specular, ToggleSpecular);

	//return TextureColor * colAmbient + Shadow * (TextureColor * colDiffuse * Diffuse * rcolor + Specular);
	return TextureColor * colAmbient + (rcolor + Specular);
}


technique GouraudWire
{
    pass P0
    {
        vertexShader = compile vs_3_0 GouraudVert();
        pixelShader = compile ps_3_0 GouraudPix();
        FillMode = Wireframe;
    }
}

technique GouraudSolid
{
    pass P0
    {
        vertexShader = compile vs_3_0 GouraudVert();
        pixelShader = compile ps_3_0 GouraudPix();
        FillMode = Solid;
    }
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
