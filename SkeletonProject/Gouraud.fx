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

struct OutputVS
{
    float4 pos : POSITION0;
    float4 color : COLOR0;
    float4 spec : COLOR1;
    float2 uv : TEXCOORD0;
};

OutputVS GouraudVert(float3 position : POSITION0, float3 normal : NORMAL0, float2 uv : TEXCOORD0)
{
    float4x4 matFinal = mul(matWorld, matVP);
    OutputVS outv = (OutputVS)0;
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

float4 GouraudPix(OutputVS input) : COLOR
{
    float4 tcol = tex2D(sstate, input.uv);
	tcol = pow(tcol, ToggleTexture);
    float4 finalcol = tcol * input.color + input.spec;
    return finalcol;
}

technique GouraudWire
{
    pass P0
    {
        vertexShader = compile vs_2_0 GouraudVert();
        pixelShader = compile ps_2_0 GouraudPix();
        FillMode = Wireframe;
    }
}

technique GouraudSolid
{
    pass P0
    {
        vertexShader = compile vs_2_0 GouraudVert();
        pixelShader = compile ps_2_0 GouraudPix();
        FillMode = Solid;
    }
}
