uniform extern float4x4 gTransform;
struct OutputVS
{
    float4 pos : POSITION0;
};

OutputVS BasicVert(float3 position : POSITION0)
{
    OutputVS outVS = (OutputVS)0;
    outVS.pos = mul(float4(position, 1.0f), gTransform);
    return outVS;   
}

float4 BasicPix() : COLOR
{
    return float4(1.0f, 0.0f, 1.0f, 1.0f);
}

technique TestTechnique
{
    pass P0
    {
        vertexShader = compile vs_2_0 BasicVert();
        pixelShader = compile ps_2_0 BasicPix();
        FillMode = Wireframe;
    }
}
