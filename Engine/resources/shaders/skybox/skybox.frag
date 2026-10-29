#version 460 core


layout (binding = 0) uniform samplerCube cubemap;

in VS_OUT {
    vec3 TexCoords;
} fs_in;

out vec4 FragColor;

void main() {
    FragColor = texture(cubemap, fs_in.TexCoords);
}
