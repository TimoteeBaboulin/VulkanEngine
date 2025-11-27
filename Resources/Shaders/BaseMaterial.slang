[__AttributeUsage(_AttributeTargets.Function)]
struct RenderPassAttribute
{
    string passName;
}


struct UniformBufferObject
{
    matrix<float, 4, 4> viewMat;
    matrix<float, 4, 4> projMat;

    float3 lightPos;
    float padding;
    float3 lightCol;
    float padding2;
}

struct VertexInput
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangeant : TANGEANT;
    float3 bitangeant : BITANGEANT;
}

struct InstanceInput
{
    float4x4 modelMat : MODEL;
    int index : TEXINDEX;
}

struct v2f
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangeant : TANGEANT;
    float3 bitangeant : BITANGEANT;

    int texIndex : TEXINDEX;
}

[[vk_binding(0, 0)]] ConstantBuffer<UniformBufferObject> ubo;
[[vk_binding(0, 1)]] Sampler2D[32] textures;

[RenderPass("depth")]
[RenderPass("color")] [shader("vertex")]
v2f vertexMain(in VertexInput inVert, in InstanceInput inInst)
{
    v2f output;

    // Calculate the final matrix.
    float4x4 finalMatrix = mul(mul(inInst.modelMat, ubo.viewMat), ubo.projMat);

    output.position = mul(float4(inVert.position, 1.0f), finalMatrix);
    output.texCoord = inVert.texCoord.xy;
    output.normal = inVert.normal;
    output.tangeant = inVert.tangeant;
    output.bitangeant = inVert.bitangeant;
    output.texIndex = inInst.index;

    return output;
}   

[RenderPass("color")][shader("fragment")]
float4 fragmentMain(in v2f input) : SV_TARGET
{
    // Use the color from the vertex input.
    float4 color = textures[input.texIndex].Sample(input.texCoord);
    color.rgb += ubo.lightCol;
    color.rgb /= float3(2.0f, 2.0f, 2.0f);

    // Return the final color.
    return color;
}