#include "AssetManager.h"
#include "Mesh.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "ModelLoader.h"

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

using AM = AssetManager;

/*
AssetManager::AssetStorage<Mesh> AssetManager::mesh_storage_;
AssetManager::AssetStorage<Texture> AssetManager::texture_storage_;
AssetManager::AssetStorage<Material> AssetManager::material_storage_;
AssetManager::AssetStorage<Shader> AssetManager::shader_storage_;
AssetManager::AssetStorage<Model> AssetManager::model_storage_;
AssetManager::AssetStorage<RenderDataConfig> AssetManager::rdc_storage_;


std::map<AssetManager::AssetKey<Shader>, Shader> AssetManager::shader_cache_;
std::map<AssetManager::AssetKey<Model>, Model> AssetManager::model_cache_;
std::map<AssetManager::AssetKey<Texture>, Texture> AssetManager::texture_cache_;
std::map<AssetManager::AssetKey<Cubemap>, Cubemap> AssetManager::cubemap_cache_;
*/
template<> 
AM::AssetStorage<Mesh>& AM::getStorage() { return mesh_storage_; }
template<> 
AM::AssetStorage<Texture>& AM::getStorage() { return texture_storage_; }
template<> 
AM::AssetStorage<Material>& AM::getStorage() { return material_storage_; }
template<> 
AM::AssetStorage<Shader>& AM::getStorage() { return shader_storage_; }
template<> 
AM::AssetStorage<Model>& AM::getStorage() { return model_storage_; }
template<>
AM::AssetStorage<RenderDataConfig>& AM::getStorage() { return rdc_storage_; }

template <>
std::map<AssetKey<Texture>, Texture> AM::getCache() { return texture_cache_; }
template<>
std::map<AssetKey<Cubemap>, Cubemap> AM::getCache() { return cubemap_cache_; }
template<>
std::map<AssetKey<Model>, Model> AM::getCache() { return model_cache_; }
template<>
std::map<AssetKey<Shader>, Shader> AM::getCache() { return shader_cache_; }

template<>
Texture AM::loadOnCacheMiss(const AssetKey<Texture>& key) {
    return TextureLoader::loadTextureFromFile(key.path, {});
}

template<>
Cubemap AM::loadOnCacheMiss(const AssetKey<Cubemap>& key) {
    return TextureLoader::loadCubemapFromFile(key.paths);
}

template<>
Model AM::loadOnCacheMiss(const AssetKey<Model>& key) {
    return ModelLoader::loadModelFromFile(key.path);
}

template<>
Shader AM::loadOnCacheMiss(const AssetKey<Shader>& key) {
    return ShaderLoader::loadShaderFromFile(key.vertex, key.fragment, key.geometry);
}



/*
void AssetManager::init() {
    
    // -- Compile default Engine Shaders --
    defaultShaders_.fallback_ = loadShader(
            PathUtils::shaderDir / "default/default.vert",
            PathUtils::shaderDir / "default/default.frag");

    defaultShaders_.outline_ = loadShader(
            PathUtils::shaderDir / "singleColor/singleColor.vert", 
            PathUtils::shaderDir / "singleColor/singleColor.frag");

    defaultShaders_.skybox_ = loadShader(
            PathUtils::shaderDir / "skybox/skybox.vert", 
            PathUtils::shaderDir / "skybox/skybox.frag");

    defaultShaders_.noPostEffect_ = loadShader(
            PathUtils::shaderDir / "postprocess/screen.vert",
            PathUtils::shaderDir / "postprocess/defaultScreen.frag");
    
    defaultShaders_.visualizeNormals_ = loadShader(
            PathUtils::shaderDir / "normal/normal.vert",
            PathUtils::shaderDir / "normal/normal.frag",
            PathUtils::shaderDir / "normal/normal.geom");


    // -- Generate standard engine models --
    defaultGeometry_.cube_ = storeAsset<Mesh>(Shapes::createCube(1.0f));
    defaultGeometry_.quad_ = storeAsset<Mesh>(Shapes::createQuad());

    // -- Generate fallback texture -- 
    unsigned char black[] = {0,0,0,0};
    defaultTextures_.fallback_ = storeAsset<Texture>(TextureLoader::loadTextureFromData(
                black, 
                1, 
                1,
                {}
                ));

    // -- Generate fallback material (uses fallback textures) -- 
    defaultMaterials_.textureless_ = storeAsset<Material>({});

    // -- Default rendering data config (posistion, normal, texCoords) --
    RenderDataConfig rdc;
    rdc.init();
    default_rdc_ = storeAsset<RenderDataConfig>(rdc);
}
*/
