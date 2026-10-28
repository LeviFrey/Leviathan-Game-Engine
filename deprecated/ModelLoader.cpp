#include "ModelLoader.h"
#include "TextureLoader.h"
#include "Mesh.h"
#include <iostream>
#include <vector>

Model ModelLoader::loadModelFromFile(const std::filesystem::path& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate /*| aiProcess_FlipUVs*/);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    }

    LoadContext context;
    context.directory = path.parent_path();
    processNode(scene->mRootNode, scene, context);
    return std::move(context.model);
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene, LoadContext& context) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene, context); 
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, context);
    }
}

void ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene, LoadContext& context) { 
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
    

    Mesh* new_mesh = context.model.addUniqueMesh(Mesh(vertices, indices));
    Material* new_mat = getOrCreateMaterial(mesh, scene, context);

    context.model.addPart(new_mesh, new_mat);
}

Material* ModelLoader::getOrCreateMaterial(
    aiMesh* mesh, 
    const aiScene* 
    scene, 
    LoadContext& context) 
{
    auto it = context.material_cache.find(mesh->mMaterialIndex);
    Material* new_mat;

    // Use cached material
    if (it != context.material_cache.end()) {
        new_mat = it->second;
    }
    
    // Create a new material and assign a new ID
    else {
        
        // Get textures
        aiMaterial* assimp_material = scene->mMaterials[mesh->mMaterialIndex];
        Texture*  diffuse = 
            context.model.addUniqueTexture<Texture>(
                    loadMaterialTextures(assimp_material, aiTextureType_DIFFUSE, context));
        Texture* specular = 
            context.model.addUniqueTexture<Texture>(
                    loadMaterialTextures(assimp_material, aiTextureType_SPECULAR, context));

        // Get shininess
        int shininess;
        if (assimp_material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
            if (shininess <= 0.0f)
                shininess = 1.0f;
            shininess = shininess;
        }
        
        new_mat = context.model.addUniqueMaterial<PhongMaterial>(
                diffuse, specular, shininess, 1.0f);
        context.material_cache.insert({mesh->mMaterialIndex, new_mat});
    }

    return new_mat;
}

Texture ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, LoadContext& context) {
    /* TODO: Implement fallback behavior 
    if (mat->GetTextureCount(type) == 0) {
        return defaultTextures_.fallback_;
    }
    */
    aiString str;
    mat->GetTexture(type, 0, &str);
    string subPath = str.C_Str();
    const std::filesystem::path newPath = context.directory / subPath;
    return TextureLoader::loadTextureFromFile(newPath, {});
}




