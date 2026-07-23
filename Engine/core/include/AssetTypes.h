#pragma once
#include <cstdint>

struct Model;
struct Mesh;
struct Texture;
struct Material;
struct Shader;

template <typename T>
struct AssetID {
    uint32_t value;
};

using ModelID    = AssetID<Model>;
using MeshID     = AssetID<Mesh>;
using TextureID  = AssetID<Texture>;
using MaterialID = AssetID<Material>;
using ShaderID   = AssetID<Shader>;

