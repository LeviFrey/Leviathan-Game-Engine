#version 460 core

in VS_OUT {
    vec2 TexCoord;
} fs_in;

out vec4 FragColor;

layout (binding = 0) uniform sampler2D diffuseTex;

struct Material {
    float shininess;
    float tesselationRate;
};

uniform Material material;

void main()
{
    vec2 texCoord = material.tesselationRate * fs_in.TexCoord;
    vec4 texColor = texture(diffuseTex, texCoord);
    if (texColor.a < 0.1) {
        discard;
    }
    FragColor = texColor;
}

