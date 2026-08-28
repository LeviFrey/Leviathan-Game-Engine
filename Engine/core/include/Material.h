#pragma once
#include "Texture.h"

struct Material {
    Texture diffuse_  = {0};
    Texture specular_ = {0};
    float shininess_    = 1.0f;
    float tesselationRate_ = 1.0f;
};
