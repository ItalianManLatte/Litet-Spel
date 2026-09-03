struct VertexShaderInput
{
    float3 postion : POSITION;
    float3 colour : COLOUR;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float3 colour : COLOUR;
    float2 uv : TEXCOORD;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = float4(input.postion, 1.0f);
    output.colour = input.colour;
    output.uv = input.uv;
    return output;
}