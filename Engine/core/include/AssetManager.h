#pragma once

// standard
#include <string>
#include <map>
#include <vector>
#include <filesystem>

// from Engine
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "AssetTypes.h"
#include "Material.h"

// Used for Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


/*
 * A global storage for all game assets
 * If an Asset is needed between objects, AssetManager will cache them and can be loaded separately
 */
class AssetManager {

private:

    struct DefaultShaders {
        ShaderID fallback_;
        ShaderID outline_;
        ShaderID skybox_;
        ShaderID noPostEffect_;
        ShaderID visualizeNormals_;
    };
    
    struct DefaultGeometry {
        MeshID cube_;
        MeshID quad_;
    };

    struct DefaultTextures {
        TextureID fallback_;
    };

    struct DefaultMaterials {
        MaterialID textureless_;
    };

public:
    static void init();

    
    // GET asset object from ID
    template <typename T>
    static const T& getAssetFromID(AssetID<T> id) {
        auto& asset_storage = getStorage<T>();
        return asset_storage.data_.at(id.value);
    }

    // LOAD and PROCESS assets from Filesystem and emplace into AssetManager
    static ModelID loadModel(const std::filesystem::path& path);
    static TextureID loadTexture(const std::filesystem::path& path);
    static TextureID loadCubemap(std::array<std::filesystem::path, 6> paths);
    static ShaderID loadShader(
            const std::filesystem::path& vertex_path, 
            const std::filesystem::path& fragment_path,
            const std::filesystem::path& geometry_path = {});

    
    // STORE asset for single-time use
    template <typename T>
    static AssetID<T> storeAsset(T asset) {
        auto& asset_storage = getStorage<T>();
        asset_storage.data_.push_back(asset);
        return {(uint32_t)asset_storage.data_.size()-1};
    }

    // REGISTER assets with user-specific strings
    template <typename T>
    static AssetID<T> registerAsset(std::string asset_name, T asset) {
        auto& asset_storage = getStorage<T>();
        AssetID<T> id = storeAsset<T>(asset);
        asset_storage.registry_.insert({asset_name, id});
        return id;
    }

    // ACQUIRE asset id from user-specific name
    template <typename T>
    static AssetID<T> getAssetFromName(std::string name) {
        auto& asset_storage = getStorage<T>();
        return asset_storage.registry_.at(name);
    };
    
    // Get Preloaded assets
    static const DefaultShaders& defaultShaders() { return defaultShaders_; }
    static const DefaultGeometry& defaultMeshes() { return defaultGeometry_; }
    static const DefaultTextures& defaultTextures() { return defaultTextures_; }
    static const DefaultMaterials& defaultMaterials() { return defaultMaterials_; }

private:

    struct LoadContext {
        std::filesystem::path directory_;
        std::unordered_map<int, MaterialID> material_cache_;
        Model model_;
    };

    struct ShaderKey {
        std::filesystem::path vertex;
        std::filesystem::path fragment;
        std::filesystem::path geometry;
        auto operator<(const ShaderKey& other) const  {
            return 
                std::tie(
                    vertex, 
                    fragment, 
                    geometry)
                 < std::tie(
                    other.vertex, 
                    other.fragment, 
                    other.geometry);
        }
    };

    struct CubemapKey {
        std::array<std::filesystem::path, 6> paths;
        bool operator<(const CubemapKey& other) const {
            return paths < other.paths;
        }
    };
 
    template <typename T>
    struct AssetStorage {
        std::vector<T> data_;
        std::unordered_map<std::string, AssetID<T>> registry_;
    };
    
    static AssetStorage<Mesh> mesh_storage_;
    static AssetStorage<Texture> texture_storage_;
    static AssetStorage<Material> material_storage_;
    static AssetStorage<Shader> shader_storage_;
    static AssetStorage<Model> model_storage_;

    template <typename T>
    static AssetStorage<T>& getStorage();
     
    // Data Caches
    static std::map<std::string, TextureID> texture_cache_;
    static std::map<CubemapKey, TextureID> cubemap_cache_;
    static std::map<std::string, ModelID> model_cache_;
    static std::map<ShaderKey, ShaderID> shader_cache_;

    // Model loading
    static ModelID processModelFromFile(const std::filesystem::path& path);
    static void processNode(aiNode* node, const aiScene* scene, LoadContext& context);
    static void processMesh(aiMesh* mesh, const aiScene* scene, LoadContext& context);
    static TextureID loadMaterialTextures(
            aiMaterial* mat, 
            aiTextureType type, 
            LoadContext& context);

    static DefaultShaders defaultShaders_;
    static DefaultGeometry defaultGeometry_;
    static DefaultTextures defaultTextures_;
    static DefaultMaterials defaultMaterials_;
};
