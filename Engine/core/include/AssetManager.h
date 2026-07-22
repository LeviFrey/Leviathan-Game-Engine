#pragma once

// standard
#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include <functional>

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
    
    // Get assets from ID
    static const Mesh& getMesh(MeshID id);
    static const Texture& getTexture(TextureID id);
    static const Material& getMaterial(MaterialID id);
    static const Model& getModel(ModelID id);
    static const Shader& getShader(ShaderID id);
    static const Texture& getCubemap(TextureID id);
    
    // Load assets from Filesystem and emplace into AssetManager
    static ModelID loadModel(const std::filesystem::path& path);
    static TextureID loadTexture(const std::filesystem::path& path);
    static TextureID loadCubemap(std::array<std::filesystem::path, 6> paths);
    static ShaderID loadShader(
            const std::filesystem::path& vertex_path, 
            const std::filesystem::path& fragment_path,
            const std::filesystem::path& geometry_path = {});

    
    // Store generated assets into global data 
    static MeshID storeMesh(Mesh mesh);
    static TextureID storeTexture(Texture texture);
    static TextureID storeCubemap(Texture texture);
    static MaterialID storeMaterial(Material material);
    static ModelID storeModel(Model model);
    static ShaderID storeShader(Shader shader);
    
    // REGISTER assets with user-specific strings
    static 
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
    struct AssetStorage;

    template <>
    struct AssetStorage<Model> {
        std::vector <Model> data_;
        std::unordered_map<std::string, ModelID> registry_;
    };

    // Global Data Storage
    static std::vector<Mesh> meshes_;
    static std::vector<Texture> textures_;
    static std::vector<Texture> cubemaps_;
    static std::vector<Material> materials_;
    static std::vector<Model> models_;
    static std::vector<Shader> shaders_;

    // Data Caches
    static std::map<std::string, TextureID> texture_cache_;
    static std::map<CubemapKey, TextureID> cubemap_cache_;
    static std::map<std::string, ModelID> model_cache_;
    static std::map<ShaderKey, ShaderID> shader_cache_;

    // Model loading
    static ModelID loadModelFromFile(const std::filesystem::path& path);
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
