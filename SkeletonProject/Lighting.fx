uniform extern float4x4 matWorld;
uniform extern float4x4 matVP;

uniform extern float4   vLightPos;
uniform extern float4   vViewPos;

uniform extern float4   colAmbient;
uniform extern float4   colDiffuse;
uniform extern float4   colSpecular;
uniform extern float    valShininess;

struct OutputVS
{
    float4 pos : POSITION0;
    float4 color : COLOR0;
};

OutputVS Vert(float3 position : POSITION0, float3 normal : NORMAL0)
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
    //return colAmbient;
}

float4 PhongPix() : COLOR
{
    return float4(1.0f, 0.0f, 1.0f, 1.0f);
}

technique GouraudWire
{
    pass P0
    {
        vertexShader = compile vs_2_0 Vert();
        pixelShader = compile ps_2_0 GouraudPix();
        FillMode = Wireframe;
    }
}

technique GouraudSolid
{
    pass P0
    {
        vertexShader = compile vs_2_0 Vert();
        pixelShader = compile ps_2_0 GouraudPix();
        FillMode = Solid;
    }
}

technique PhongWire
{
    pass P0
    {
        vertexShader = compile vs_2_0 Vert();
        pixelShader = compile ps_2_0 PhongPix();
        FillMode = Wireframe;
    }
}

technique PhongSolid
{
    pass P0
    {
        vertexShader = compile vs_2_0 Vert();
        pixelShader = compile ps_2_0 PhongPix();
        FillMode = Solid;
    }
}
