#version 460 core
#define AREA 0
#define POINT 1
#define DIRECTIONAL 2
#define SPOT 3

in VS_OUT {
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPos;
} fs_in;

out vec4 FragColor;

struct Material {
    float shininess;
    float tesselationRate;
};

struct LightData {
    vec4 position_type; // (x,y,z,type)
    vec4 ambient; // (r,g,b)
    vec4 diffuse; // (r,g,b)
    vec4 specular; // (r,g,b)
    vec4 direction; // (x,y,z)
    vec4 attenuation; // (const, lin, quad)
    vec4 spotlight; // (innerCutOff, outerCutOff)
};

#define MAX_LIGHTS 128
layout(std140, binding = 0) uniform LightBlock {
    LightData lights[MAX_LIGHTS];
    vec4 lightCount;
};

layout(std140, binding = 1) uniform CameraData {
    mat4 view;
    mat4 projection;
    vec4 viewPos;
};

layout (binding = 0) uniform sampler2D diffuseTex;
layout (binding = 1) uniform sampler2D specularTex;

uniform Material material;
uniform mat4 model;
vec4 diffuseSample;
vec4 specularSample;

vec3 CalcDirLight(LightData light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(-vec3(light.direction));

    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine
    vec3 diffuse = vec3(light.diffuse) * diff * vec3(diffuseSample);
    vec3 specular = vec3(light.specular) * spec * vec3(specularSample);
    return (diffuse + specular);
}

vec3 CalcPointLight(LightData light, vec3 norm, vec3 viewDir) {
    float dist = length(vec3(light.position_type) - fs_in.FragPos);
    float attenuation = 1.0 / (light.attenuation[0] + dist * light.attenuation[1] + dist * dist * light.attenuation[2]);
    vec3 lightDir = normalize(vec3(light.position_type) - fs_in.FragPos);
    float lightAngle = max(dot(norm, lightDir), 0.0);

    // diffuse shading
    vec3 diffuse = vec3(light.diffuse) * lightAngle * vec3(diffuseSample);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float light_above_face = max(dot(lightDir, norm), 0.0);
    reflectDir = normalize(light_above_face * reflectDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(light.specular) * spec * vec3(specularSample);
    return attenuation * (specular + diffuse);
}


vec3 CalcAreaLight(LightData light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(light.position_type.xyz - fs_in.FragPos);
    float lightAngle = max(dot(norm, lightDir), 0.0);

    // diffuse shading
    vec3 diffuse = vec3(light.diffuse) * lightAngle * vec3(diffuseSample);

    // specular shading
    vec3 specTex = texture(specularTex, fs_in.TexCoord).rgb;
    float specMask = max(max(specTex.r, specTex.g), specTex.b);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(light.specular) * spec * specTex * specMask;
    /*
    vec3 reflectDir = reflect(-lightDir, norm);
    float light_above_face = max(dot(lightDir, norm), 0.0);
    reflectDir = normalize(light_above_face * reflectDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(light.specular) * spec * vec3(specularSample);
    */
    return specular + diffuse;
}

vec3 CalcSpotLight(LightData light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(vec3(light.position_type) - fs_in.FragPos);
    float theta = dot(normalize(lightDir), normalize(-vec3(light.direction)));
    float innerOuterDiff = light.spotlight[0] - light.spotlight[1];
    float intensity = clamp((theta - light.spotlight[1]) / innerOuterDiff, 0.0f, 1.0f);
    
    // diffuse shading
    float lightAngle = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(light.diffuse) * lightAngle * vec3(diffuseSample);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float light_above_face = max(dot(lightDir, norm), 0.0);
    reflectDir = normalize(light_above_face * reflectDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(light.specular) * spec * vec3(specularSample);

    return intensity * (diffuse + specular);
}


void main()
{
    vec2 scaledTexCoord = material.tesselationRate * fs_in.TexCoord;
    diffuseSample = texture(diffuseTex, scaledTexCoord);
    specularSample = texture(specularTex, scaledTexCoord);
    if (diffuseSample.a < 0.1) {
        discard;
    }
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(vec3(viewPos) - fs_in.FragPos);
    vec3 result = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < lightCount[0]; i++) {
        if (lights[i].position_type[3] == AREA) {
            result += CalcAreaLight(lights[i], norm, viewDir);
        } else if (lights[i].position_type[3] == POINT) {
            result += CalcPointLight(lights[i], norm, viewDir);
        } else if (lights[i].position_type[3] == DIRECTIONAL) {
            result += CalcDirLight(lights[i], norm, viewDir);
        } else if  (lights[i].position_type[3] == SPOT) {
            result += CalcSpotLight(lights[i], norm, viewDir);
        }
    }
    FragColor = vec4(result, 1.0);
}
 
