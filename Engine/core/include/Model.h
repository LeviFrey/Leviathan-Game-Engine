#pragma once

// From engine
#include "Mesh.h"
#include "Material.h"

// Standard Lib
#include <vector>
#include <memory>
#include <filesystem>

// Used for Assimp model loading
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


struct RenderPart {
    Mesh* mesh_;
    Material* material_;
};

class Model {
    public:
        
        Model() = default;
        Model(const std::filesystem::path& path);
        Model(std::vector<RenderPart>&& parts) : parts_(std::move(parts)) {}

        const std::vector<RenderPart>& getParts() const { 
            return parts_; 
        }

        void addPart(Mesh* mesh, Material* material) { 
            parts_.push_back({mesh, material}); 
        } 

        Mesh* addUniqueMesh(Mesh mesh) {
            unique_meshes_.push_back(std::make_unique<Mesh>(std::move(mesh)));
            return unique_meshes_.back().get();
        }

        template <typename T, typename... Args>
        T* addUniqueMaterial(Args&&... args) {
            static_assert(std::is_base_of_v<Material, T>);

            std::unique_ptr<T> material = std::make_unique<T>(std::forward<Args>(args)...);
            T* ptr = material.get();
            unique_materials_.push_back(std::move(material));
            return ptr;
        }
        
        template <typename T, typename... Args>
        T* addUniqueTexture(Args&&... args) {
            static_assert(std::is_base_of_v<TextureBase, T>);

            std::unique_ptr<T> texture = std::make_unique<T>(std::forward<Args>(args)...);
            T* ptr = texture.get();
            unique_textures_.push_back(std::move(texture));
            return ptr;
        }

    private:
        std::vector<RenderPart> parts_;

        // Used for Model loading, uses unique assets.
        std::vector<std::unique_ptr<Mesh>> unique_meshes_;
        std::vector<std::unique_ptr<TextureBase>> unique_textures_;
        std::vector<std::unique_ptr<Material>> unique_materials_;

        std::filesystem::path path_;
        
        // Model Loading helper functions
        std::unique_ptr<Material> loadMaterial(aiMaterial* mat);
        Texture* loadTexture(aiMaterial* mat, aiTextureType type);
        std::filesystem::path loadTexturePath(aiMaterial* mat, aiTextureType type);
        void processNode(aiNode* node, const aiScene* scene);
        void processMesh(aiMesh* mesh, const aiScene* scene);
};


