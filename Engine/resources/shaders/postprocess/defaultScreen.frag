#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoord;
} fs_in;

layout (binding = 0) uniform sampler2D bufferTex; 

void main() {   
    FragColor = texture(bufferTex, fs_in.TexCoord);
}
