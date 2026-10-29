#pragma once 
#include <glm/glm.hpp>
const int MAX_LIGHTS = 128;

typedef struct alignas(16) {
    // Base class LightSource
    glm::vec4 position_type_;  // (x,y,z,type)
    glm::vec4 ambient_;   // (r,g,b)
    glm::vec4 diffuse_;   // (r,g,b)
    glm::vec4 specular_;  // (r,g,b)
    glm::vec4 direction_; // (x,y,z)
    glm::vec4 attenuation_; // (const, lin, quad)
    glm::vec4 spotlight_;   // (innerCutoff, outerCutoff) 
} LightData;

typedef struct alignas(16) {
    LightData light_data[MAX_LIGHTS];
    glm::vec4 lightCount;
} LightBlock;

enum LightType {
    AREA,
    POINT,
    DIRECTIONAL,
    SPOT
}; 
