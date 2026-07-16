#include "AssetManager.h"
#include "Mesh.h"
#include "PathUtils.h"
#include "Shapes.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>


std::vector<Mesh> AssetManager::meshes_;
std::vector<Texture> AssetManager::textures_;
std::vector<Texture> AssetManager::cubemaps_;
std::vector<Material> AssetManager::materials_;
std::vector<Model> AssetManager::models_;
std::vector<Shader> AssetManager::shaders_;

std::map<AssetManager::ShaderKey, ShaderID> AssetManager::shader_cache_;
std::map<std::string, ModelID> AssetManager::model_cache_;
std::map<std::string, TextureID> AssetManager::texture_cache_;
std::map<AssetManager::CubemapKey, TextureID> AssetManager::cubemap_cache_;

AssetManager::DefaultShaders AssetManager::defaultShaders_;
AssetManager::DefaultGeometry AssetManager::defaultGeometry_;
AssetManager::DefaultTextures AssetManager::defaultTextures_;
AssetManager::DefaultMaterials AssetManager::defaultMaterials_;


void AssetManager::init() {
    /*
     *  Load Engine Shader
     */
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


    /*
     *  Load Engine Geometry
     */
    defaultGeometry_.cube_ = storeMesh(Shapes::createCube(1.0f));
    defaultGeometry_.quad_ = storeMesh(Shapes::createQuad());

    /*
     *  Load Engine Textures
     */
    unsigned char black[] = {0,0,0,0};
    defaultTextures_.fallback_ = storeTexture(TextureLoader::loadTextureFromData(
                black, 
                1, 
                1,
                {}
                ));

    /*
     * Load Engine Materials
     */
    defaultMaterials_.textureless_ = storeMaterial({});
    
}

/*
 *  Get assets from ID
 */

const Mesh& AssetManager::getMesh(MeshID id) {
    return meshes_.at(id);    
}

const Texture& AssetManager::getTexture(TextureID id) {
    return textures_.at(id);
}

const Texture& AssetManager::getCubemap(TextureID id) {
    return cubemaps_.at(id);
}

const Material& AssetManager::getMaterial(MaterialID id) {
    return materials_.at(id);
}

const Model& AssetManager::getModel(ModelID id) {
    return models_.at(id);
}

const Shader& AssetManager::getShader(ShaderID id) {
    return shaders_.at(id);
}

/*
 *  Store Assets
 */
MeshID AssetManager::storeMesh(Mesh mesh) {
    meshes_.push_back(mesh);
    return (MeshID)meshes_.size()-1;
}

MaterialID AssetManager::storeMaterial(Material material) {
    materials_.push_back(material);
    return (MaterialID)materials_.size()-1;
}

ModelID AssetManager::storeModel(Model model) {
    models_.push_back(model);
    return (ModelID)models_.size()-1;
}

TextureID AssetManager::storeTexture(Texture texture) {
    textures_.push_back(texture);
    return (TextureID)textures_.size()-1;
}

TextureID AssetManager::storeCubemap(Texture texture) {
    cubemaps_.push_back(texture);
    return (TextureID)cubemaps_.size()-1;
}

ShaderID AssetManager::storeShader(Shader shader) {
    shaders_.push_back(shader);
    return (ShaderID)shaders_.size()-1;
}


/*
 *  Load asset for the first time or pull from cache if already loaded somewherer else
 */

ModelID AssetManager::loadModel(const std::filesystem::path& path) {
    auto it = model_cache_.find(path.string());
    if (it != model_cache_.end()) {
        return it->second;
    } else {
        return loadModelFromFile(path); 
    }
}

TextureID AssetManager::loadTexture(const std::filesystem::path& path) {
    auto it = texture_cache_.find(path.string());
    if (it != texture_cache_.end()) {
        return it->second;
    } else {
        TextureID id = storeTexture(TextureLoader::loadTextureFromFile(path, {}));
        texture_cache_.insert({path.string(), id});
        return id;
    }
}

TextureID AssetManager::loadCubemap(std::array<std::filesystem::path, 6> paths) {
    auto it = cubemap_cache_.find({paths});
    if (it != cubemap_cache_.end()) {
        return it->second;
    } else {
        TextureID id = storeCubemap(TextureLoader::loadCubemapFromFile(paths));
        cubemap_cache_.insert({{paths}, id});
        return id;
    }
}


ShaderID AssetManager::loadShader(
        const std::filesystem::path& vertex_path, 
        const std::filesystem::path& fragment_path,
        const std::filesystem::path& geometry_path) {
    auto it = shader_cache_.find({
            vertex_path.string(), 
            fragment_path.string(), 
            geometry_path.string()});
    if(it != shader_cache_.end()) {
        return it->second;
    } else {
        ShaderID id = storeShader(ShaderLoader::loadShaderFromFile(
                    vertex_path, 
                    fragment_path,
                    geometry_path));
        shader_cache_.insert({{
                vertex_path.string(), 
                fragment_path.string(),
                geometry_path.string()}, 
                id});
        return id;
    }
}

/*
 * Model Loading Pipeline Code
 */

ModelID AssetManager::loadModelFromFile(const std::filesystem::path& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate /*| aiProcess_FlipUVs*/);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    }

    LoadContext context;
    context.directory_ = path.parent_path();
    processNode(scene->mRootNode, scene, context);
    models_.push_back(std::move(context.model_));
    model_cache_.insert({path.string(), models_.size()-1});
    return (ModelID)models_.size()-1;
}


void AssetManager::processNode(aiNode* node, const aiScene* scene, LoadContext& context) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene, context); 
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, context);
    }
}

void AssetManager::processMesh(aiMesh* mesh, const aiScene* scene, LoadContext& context) { 
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position_ = vector;
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal_ = vector;
        }


        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords_ = vec;
        } else {
            vertex.texCoords_ = glm::vec2(0.0, 0.0);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    // Create the new mesh and assign it an ID
    meshes_.push_back({vertices, indices});
    MeshID mesh_id = meshes_.size()-1;

    auto it = context.material_cache_.find(mesh->mMaterialIndex);
    MaterialID mat_id;

    // Use cached material
    if (it != context.material_cache_.end()) {
        mat_id = it->second;
    }
    
    // Create a new material and assign a new ID
    else {
        Material newMaterial;
        
        // Get textures
        aiMaterial* assimp_material = scene->mMaterials[mesh->mMaterialIndex];
        newMaterial.diffuse_ = loadMaterialTextures(assimp_material, aiTextureType_DIFFUSE, context);
        newMaterial.specular_ = loadMaterialTextures(assimp_material, aiTextureType_SPECULAR, context);
        // Get shininess
        int shininess;
        if (assimp_material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
            if (shininess <= 0.0f)
                shininess = 1.0f;
            newMaterial.shininess_ = shininess;
        }
        
        // Fill the Material object, add it to the assets, and cache it
        materials_.push_back(std::move(newMaterial));
        mat_id = materials_.size()-1;
        context.material_cache_.insert({mesh->mMaterialIndex, mat_id});
    }
    
    context.model_.parts_.push_back({mesh_id, mat_id});
}


TextureID AssetManager::loadMaterialTextures(aiMaterial* mat, aiTextureType type, LoadContext& context) {
    if (mat->GetTextureCount(type) == 0) {
        return defaultTextures_.fallback_;
    }
    aiString str;
    mat->GetTexture(type, 0, &str);
    string subPath = str.C_Str();
    const std::filesystem::path newPath = context.directory_ / subPath;
    return loadTexture(newPath);
}
