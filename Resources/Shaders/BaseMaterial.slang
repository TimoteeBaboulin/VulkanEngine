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
    float3 position;
    float3 color;
    float3 texCoord;
}

struct InstanceInput
{
    float4x4 modelMat;
    int index;
}

struct v2f
{
    float3 fragColor;
    float2 texCoord;
    int texIndex;

    float4 position : SV_POSITION;
}
[[vk_binding(0, 0)]] ConstantBuffer<UniformBufferObject> ubo;
[[vk_binding(0, 1)]] Sampler2D[32] textures;

[shader("vertex")]
v2f vertexMain([[vk_location(0)]] VertexInput inVert, [[vk_location(3)]] InstanceInput inInst)
{
    v2f output;

    // Calculate the final matrix.
    float4x4 finalMatrix = mul(mul(inInst.modelMat, ubo.viewMat), ubo.projMat);

    output.position = mul(float4(inVert.position, 1.0f), finalMatrix);
    output.fragColor = inVert.color;
    output.texCoord = inVert.texCoord.xy;
    output.texIndex = inInst.index;

    return output;
}

[shader("fragment")]
float4 fragmentMain(in v2f input) : SV_TARGET
{
    // Use the color from the vertex input.
    float4 color = textures[input.texIndex].Sample(input.texCoord);
    color.rgb += ubo.lightCol;
    color.rgb /= float3(2.0f, 2.0f, 2.0f);

    // Return the final color.
    return color;
}