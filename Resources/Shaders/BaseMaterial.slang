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
    float4x4 inverseModelMat : INVERSEMODEL;
    int index : TEXINDEX;
}

struct v2f
{
    float4 position : SV_POSITION;
    float3 worldPos : WORLDPOS;
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
v2f vertexMain(VertexInput inVert, InstanceInput inInst)
{
    v2f output;

    // Calculate the final matrix.
    float4x4 mvp = mul(mul(inInst.modelMat, ubo.viewMat), ubo.projMat);
    float3x3 normalMatrix = transpose(float3x3(inInst.inverseModelMat));

    output.position = mul(float4(inVert.position, 1.0f), mvp);
    output.worldPos = mul(float4(inVert.position, 1.0f), inInst.modelMat).xyz;
    output.texCoord = inVert.texCoord.xy;
    output.normal = normalize(mul(normalMatrix, inVert.normal));
    output.tangeant = normalize(inVert.tangeant);
    output.bitangeant = normalize(inVert.bitangeant);
    output.texIndex = inInst.index;

    return output;
}

float3 CalculateAmbientLighting(in float3 lightCol)
{
    float3 ambient = 0.25f * lightCol;
    return ambient;
}

float3 CalculateDiffuseLighting(in float3 lightCol, in float3 normal, in float3 lightDir)
{
    float diffuseStrength = dot(normal, lightDir);
    float3 diffuse = max(diffuseStrength, 0.0f) * lightCol;
    return diffuse;
}

[RenderPass("color")][shader("fragment")]
float4 fragmentMain(in v2f input) : SV_TARGET
{
    // Use the color from the vertex input.
    float4 color = textures[input.texIndex].Sample(input.texCoord);
    float3x3 TBN = float3x3(normalize(input.tangeant), normalize(input.bitangeant), normalize(input.normal));
    float3 lightDir = normalize(input.worldPos - ubo.lightPos);
    float3 normal = normalize(mul(TBN, float3(0.0f, 0.0f, 1.0f)));

    float3 lighting = CalculateAmbientLighting(ubo.lightCol);
    lighting += CalculateDiffuseLighting(ubo.lightCol, normal, lightDir);
    color.rgb *= lighting;

    // Return the final color.
    return color;
}