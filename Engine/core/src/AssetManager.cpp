#include "AssetManager.h"
#include "Mesh.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

using AM = AssetManager;

// -- Storage Handlers -- 

template<> 
AM::AssetStorage<Mesh>& AM::getStorage() { return mesh_storage_; }

template<> 
AM::AssetStorage<Texture>& AM::getStorage() { return texture_storage_; }

template<>
AM::AssetStorage<Cubemap>& AM::getStorage() { return cubemap_storage_; }

template<> 
AM::AssetStorage<Material>& AM::getStorage() { return material_storage_; }

template<> 
AM::AssetStorage<Shader>& AM::getStorage() { return shader_storage_; }

template<> 
AM::AssetStorage<Model>& AM::getStorage() { return model_storage_; }

template<>
AM::AssetStorage<VertexArrayObject>& AM::getStorage() { return rdc_storage_; }


// -- Cache Handlers --
template <>
AM::AssetCache<Texture>& AM::getCache() { return texture_cache_; }

template<>
AM::AssetCache<Cubemap>& AM::getCache() { return cubemap_cache_; }

template<>
AM::AssetCache<Model>& AM::getCache() { return model_cache_; }

template<>
AM::AssetCache<Shader>& AM::getCache() { return shader_cache_; }

template<>
Texture AM::loadOnCacheMiss(const AssetKey<Texture>& key) {
    return Texture(key.path);
    //return TextureLoader::loadTextureFromFile(key.path, {});
}

template<>
Cubemap AM::loadOnCacheMiss(const AssetKey<Cubemap>& key) {
    //return TextureLoader::loadCubemapFromFile(key.paths);
    return Cubemap(key.paths);
}

template<>
Model AM::loadOnCacheMiss(const AssetKey<Model>& key) {
    return Model(key.path);
    //return ModelLoader::loadModelFromFile(key.path);
}

template<>
Shader AM::loadOnCacheMiss(const AssetKey<Shader>& key) {
    return Shader(key.vertex, key.fragment, key.geometry);
    //return ShaderLoader::loadShaderFromFile(key.vertex, key.fragment, key.geometry);
}
