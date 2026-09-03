struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float3 colour : COLOUR;
    float2 uv : TEXCOORD;
};

Texture2D diffuseTex : register(t0);
SamplerState samp : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
    return diffuseTex.Sample(samp, input.uv);
}