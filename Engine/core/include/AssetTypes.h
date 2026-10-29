#pragma once

#include "Assets/Model.h"
#include "Assets/Mesh.h"
#include "Assets/Texture.h"
#include "Assets/Material.h"
#include "Assets/Shader.h"
#include "Assets/VertexArrayObject.h"

template <typename T>
struct AssetID {
    std::size_t value;
};

using ModelID    = AssetID<Model>;
using MeshID     = AssetID<Mesh>;
using TextureID  = AssetID<Texture>;
using CubemapID  = AssetID<Cubemap>;
using MaterialID = AssetID<Material>;
using ShaderID   = AssetID<Shader>;
using RenderDataConfigID = AssetID<VertexArrayObject>;

