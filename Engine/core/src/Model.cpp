#include "Model.h"

Model::Model(const std::filesystem::path& path) : path_(path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path_, aiProcess_Triangulate /* | aiProcess_FlipUVs*/);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error( 
            "Assimp failed to load " + path.string() + ": " + importer.GetErrorString()
        );
    }
    
    unique_materials_.reserve(scene->mNumMaterials);
    for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
        unique_materials_.push_back(loadMaterial(scene->mMaterials[i]));
    }

    processNode(scene->mRootNode, scene);
}

std::unique_ptr<Material> Model::loadMaterial(aiMaterial* mat) {
    Texture* diffuse = 
        addUniqueTexture<Texture>(Image(loadTexturePath(mat, aiTextureType_DIFFUSE)));
    Texture* specular =
        addUniqueTexture<Texture>(Image(loadTexturePath(mat, aiTextureType_SPECULAR)));
    
    int shininess = 1.0f;
    mat->Get(AI_MATKEY_SHININESS, shininess);
    if (shininess <= 0.0f) {
        shininess = 1.0f;
    }

    return std::make_unique<PhongMaterial>(
        diffuse, 
        specular,
        shininess,
        1.0f
    );
}

std::filesystem::path Model::loadTexturePath(aiMaterial* mat, aiTextureType type) {
    aiString str;
    mat->GetTexture(type, 0, &str);
    std::string sub_path = str.C_Str();
    return path_ / sub_path;
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene); 
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    } 
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

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

    Mesh* new_mesh = addUniqueMesh(Mesh(std::move(vertices), std::move(indices)));
    Material* new_material = unique_materials_.at(mesh->mMaterialIndex).get();

    addPart(new_mesh, new_material); 
}
