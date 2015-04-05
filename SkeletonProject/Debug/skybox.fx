uniform extern float4x4 matWVP;
uniform extern texture  SkyTexture;

samplerCUBE sstate = sampler_state {
      Texture   = <SkyTexture>;
      MinFilter = LINEAR;
      MagFilter = LINEAR;
      MipFilter = LINEAR;
      AddressU  = WRAP;
      AddressV  = WRAP;
};


void SkyVS(float3 position : POSITION0,
           out float4 outpos : POSITION0,
           out float3 texcoord : TEXCOORD0)
{
      outpos = mul(float4(position, 1.0f), matWVP).xyww;

      texcoord = position;
}

float4 SkyPS(float3 texcoord : TEXCOORD0) : COLOR
{
    return texCUBE(sstate, texcoord);
	//return tex2D(sstate, texcoord.xy);
	//return float4(texcoord, 1.0f);
}

technique SkyTech
{
      pass P0
      {
            vertexShader = compile vs_2_0 SkyVS();
            pixelShader  = compile ps_2_0 SkyPS();

            CullMode = None;
            ZFunc = Always;         // Always write sky to depth buffer
            StencilEnable = true;
            StencilFunc   = Always;

            StencilPass   = Replace;
            StencilRef    = 0;     // clear to zero
      }
}
