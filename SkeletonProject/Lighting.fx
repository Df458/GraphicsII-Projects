uniform extern float4x4 matWorld;
uniform extern float4x4 matVP;

struct OutputVS
{
    float4 pos : POSITION0
};

OutputVS Vert(float3 position : POSITION0)
{
    float4x4 matFinal = mul(matWorld, matVP);
    OutputVS outv = (OutputVS)0;
    outv.pos = mul(float4(position, 1.0f), matFinal);
    return outv;
}

float4 GouraudPix() : COLOR
{
    return float4(1.0f, 0.0f, 1.0f, 1.0f);
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

technique GouraudSolid
{
    pass P0
    {
        vertexShader = compile vs_2_0 Vert();
        pixelShader = compile ps_2_0 PhongPix();
        FillMode = Solid;
    }
}
