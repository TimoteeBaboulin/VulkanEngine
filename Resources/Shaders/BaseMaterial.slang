[__AttributeUsage(_AttributeTargets.Function)]
struct RenderPassAttribute
{
    string passName;
}


struct UniformBufferObject
{
    matrix<float, 4, 4> viewMat;
    matrix<float, 4, 4> projMat;

    float4 cameraPos;
    float4 lightPos;
    float4 lightCol;
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

// Calculate ambient lighting
float3 CalculateAmbientLighting(in float3 lightCol)
{
    float3 ambient = 0.25f * lightCol;
    return ambient;
}

// Calculate diffuse lighting
float3 CalculateDiffuseLighting(in float3 lightCol, in float3 normal, in float3 lightDir)
{
    // Lambertian diffuse
    float diffuseStrength = dot(normal, lightDir);
    float3 diffuse = max(diffuseStrength, 0.0f) * lightCol;
    return diffuse;
}

// Calculate Blinn-Phong specular lighting
float3 CalculateSpecularLighting(in float3 lightCol, in float3 normal, in float3 halfwayDir)
{
    // Blinn-Phong specular
    // Clamped dot product of normal and halfway vector raised to the power of shininess (32 here)
    float specStrength = pow(max(dot(normal, halfwayDir), 0.0f), 32);
    float3 specular = specStrength * lightCol;
    return specular;
}

void CalculateBlinnPhongLighting(in float3 lightCol, in float3 normal, in float3 lightDir, in float3 viewDir, inout float4 color)
{
    float3 lighting = CalculateAmbientLighting(lightCol);
    lighting += CalculateDiffuseLighting(lightCol, normal, lightDir);
    float3 halfwayDir = normalize(lightDir + viewDir);
    lighting += CalculateSpecularLighting(lightCol, normal, halfwayDir);
    color.rgb *= lighting;
}

float GeometrySchlickGGX(in float3 normal, in float3 view, in float roughness)
{
    // Schlick-GGX k for direct lighting
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;

    float NdotV = max(dot(normal, view), 0.0f);

    float denom = NdotV * (1.0f - k) + k;
    return NdotV / denom;
}

float NormalDistributionGGX(in float3 normal, in float3 halfwayDir, in float roughness)
{
    float a = roughness * roughness;
    float NdotH = max(dot(normal, halfwayDir), 0.0f);
    float NdotH2 = NdotH * NdotH;
    float a2min1 = (a * a) - 1.0f;

    float denom = (NdotH2 * a2min1 + 1.0f);
    denom = 3.14159265f * denom * denom;
    return a / denom;
}

float3 FresnelSchlick(in float cosTheta, in float3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

void CalculatePBRLighting(in float3 lightCol, in float3 normal, in float3 rawLightDir, in float3 viewDir, inout float4 color)
{
    // Placeholder for PBR lighting calculations
    float roughness = 0.5f;
    float metallic = 0.5f;

    float3 L0 = float3(0.0f, 0.0f, 0.0f);

    // Calculate necessary vectors and factors
    float3 lightDir = normalize(rawLightDir);
    float3 halfwayDir = normalize(lightDir + viewDir);
    float3 F0 = float3(0.04f, 0.04f, 0.04f); // Base reflectivity for non-metals
    F0 = lerp(F0, color.rgb, metallic);

    // Calculate radiance
    float cosTheta = max(dot(normal, lightDir), 0.0f);
    // float attenuation = 1.0f / (length(rawLightDir) * length(rawLightDir));
    float attenuation = 1.0f; // No attenuation since it's a directional light
    float3 radiance = lightCol * attenuation;

    // Cook-Torrance BRDF
    float NDF = NormalDistributionGGX(normal, halfwayDir, roughness);
    float G = GeometrySchlickGGX(normal, viewDir, roughness) * GeometrySchlickGGX(normal, lightDir, roughness);
    float3 F = FresnelSchlick(cosTheta, F0);

    // Calculate the BRDF specular term
    float3 numerator = NDF * G * F;
    float denominator = 4.0f * max(dot(normal, viewDir), 0.0f) * max(dot(normal, lightDir), 0.0f) + 0.001f;
    float3 specular = numerator / denominator;

    // Calculate kS and kD
    float3 kS = F;
    float3 kD = 1.0f - kS;
    kD *= 1.0f - metallic;

    L0 += (kD * color.rgb / 3.14159265f + specular) * radiance * cosTheta;

    // Calculate some ambient lighting
    float3 ambient = 0.03f * color.rgb;
    color.rgb = ambient + L0;
}

[RenderPass("color")][shader("fragment")]
float4 fragmentMain(in v2f input) : SV_TARGET
{
    // Use the color from the vertex input.
    float4 color = textures[input.texIndex].Sample(input.texCoord);

    // Reconstruct the normal from tangent space to world space.
    float3x3 TBN = float3x3(normalize(input.tangeant), normalize(input.bitangeant), normalize(input.normal));
    float3 normal = normalize(mul(TBN, float3(0.0f, 0.0f, 1.0f)));

    // Get rid of the unused 4th component.
    float3 camPos = ubo.cameraPos.rgb;
    float3 lightPos = ubo.lightPos.rgb;
    float3 lightCol = ubo.lightCol.rgb;

    // Calculate the vectors needed for lighting calculations.
    float3 viewDir = normalize(camPos - input.worldPos);
    // float3 rawlightDir = input.worldPos - lightPos;
    float3 rawlightDir = ubo.lightPos.rgb; // Directional light
    float3 lightDir = normalize(rawlightDir);

    // Calculate lighting effects.
    // CalculateBlinnPhongLighting(lightCol, normal, lightDir, viewDir, color);
    CalculatePBRLighting(lightCol, normal, rawlightDir, viewDir, color);

    // Return the final color.
    return color;
}