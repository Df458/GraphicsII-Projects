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

struct OutputVS
{
    float4 pos : POSITION0;
    float4 color : COLOR0;
};

OutputVS GouraudVert(float3 position : POSITION0, float3 normal : NORMAL0, float2 uv : TEXCOORD0)
{
    float4x4 matFinal = mul(matWorld, matVP);
    OutputVS outv = (OutputVS)0;
    outv.pos = mul(float4(position, 1.0f), matFinal);

    float3 wvpos = mul(float4(position, 1.0f), matWorld).xyz;
    float3 wnorm = normalize(mul(float4(normal, 0.0f), matITWorld).xyz);
    float3 wli   = normalize(vLightPos - wvpos);
    float3 wvtvi = normalize(vViewPos - wvpos);

    float3 amb  = colAmbient.rgb;

    float s = max(dot(wnorm, wli), 0.0f);
    float3 diff = s * colDiffuse.rgb * colLight.rgb;

    float3 reflected =  reflect(-wli, wnorm);
    float t = pow(max(dot(reflected, wvtvi), 0.0f), valShininess);
    float3 spec = t * (colSpecular * colLight).rgb;

	float d = distance(vLightPos, wvpos);
	float A = vAttenuation.x + vAttenuation.y*d + vAttenuation.z*d*d;

    outv.color = float4(amb + ((diff + spec) / A), colDiffuse.a);
    return outv;
}

OutputVS PhongVert(float3 position : POSITION0, float3 normal : NORMAL0)
{
    float4x4 matFinal = mul(matWorld, matVP);
    OutputVS outv = (OutputVS)0;
    outv.pos = mul(float4(position, 1.0f), matFinal);
    outv.color = abs(float4(normal, 1.0f));
    return outv;
}

float4 GouraudPix(OutputVS input) : COLOR
{
    return input.color;
}

float4 PhongPix() : COLOR
{
    return float4(1.0f, 0.0f, 1.0f, 1.0f);
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

technique PhongWire
{
    pass P0
    {
        vertexShader = compile vs_2_0 PhongVert();
        pixelShader = compile ps_2_0 PhongPix();
        FillMode = Wireframe;
    }
}

technique PhongSolid
{
    pass P0
    {
        vertexShader = compile vs_2_0 PhongVert();
        pixelShader = compile ps_2_0 PhongPix();
        FillMode = Solid;
    }
}
