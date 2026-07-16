#pragma once
#include <vector>
#include "AssetTypes.h"

/*
enum TextureType {
    DIFFUSE,
    SPECULAR,
    CUBEMAP
};

struct TextureSlot {
    TextureID ID_;
    TextureType type_;
};

struct Material {
    std::vector<TextureSlot> texture_slots_;
    float shininess_;
};
*/

struct Material {
    TextureID diffuse_  = 0;
    TextureID specular_ = 0;
    float shininess_    = 1.0f;
    float tesselationRate_ = 1.0f;
};
