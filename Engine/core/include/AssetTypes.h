#pragma once
#include <cstdint>

struct ModelTag {};
struct MeshTag {};
struct TextureTag {};
struct MaterialTag {};
struct ShaderTag {};

template <typename Tag>
struct AssetID {
    uint32_t value;
};

using ModelID    = AssetID<ModelTag>;
using MeshID     = AssetID<MeshTag>;
using TextureID  = AssetID<TextureTag>;
using MaterialID = AssetID<MaterialTag>;
using ShaderID   = AssetID<ShaderTag>;

