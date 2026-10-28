#pragma once

// standard
#include <filesystem>
#include <unordered_map>

// from Engine
#include "Material.h"
#include "Model.h"

// Used for Assimp model loading
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class ModelLoader {
    private:
        struct LoadContext {
            std::filesystem::path directory;
            std::unordered_map<int, Material*> material_cache;
            Model model;
        };

    public:
        static Model loadModelFromFile(const std::filesystem::path& path);
        static void processNode(aiNode* node, const aiScene* scene, LoadContext& context);
        static void processMesh(aiMesh* mesh, const aiScene* scene, LoadContext& context);
        static Material* getOrCreateMaterial(aiMesh* mesh, const aiScene* scene, LoadContext& context);
        static Texture loadMaterialTextures(
            aiMaterial* mat,
            aiTextureType type,
            LoadContext& context);
};
        
