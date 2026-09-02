struct VertexShaderInput
{
    float3 postion : POSITION;
    float3 colour : COLOUR;
   
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float3 colour : COLOUR;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = float4(input.postion, 1.0f);
    output.colour = input.colour;
    return output;
}