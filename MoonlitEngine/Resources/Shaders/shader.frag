#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(set = 1, binding = 0) uniform sampler2D[32] texSampler;
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in int textureIndex;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSampler[nonuniformEXT(textureIndex)], fragTexCoord);
}