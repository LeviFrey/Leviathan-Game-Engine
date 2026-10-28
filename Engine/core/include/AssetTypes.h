#pragma once

#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Shader.h"
#include "VertexArrayObject.h"

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

