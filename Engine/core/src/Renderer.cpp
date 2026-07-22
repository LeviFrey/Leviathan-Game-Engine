#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "GameObject.h"
#include "Game.h"
#include "AssetManager.h"
#include "RenderBindings.h"
#include <iostream>

Renderer::Renderer(ModelID model_id, ShaderID shader_id) : 
    model_id_(model_id),
    shader_id_(shader_id)
{}

void Renderer::render() {
    const Shader& shader = AssetManager::getAssetFromID<Shader>(shader_id_);
    shader.use();
    glm::mat4 transform = getGameObject()->getComponent<Transform>()->getWorldMatrix();
    shader.setMat4("model", transform);
    getGameObject()->getGame()->applyGlobalUniforms(shader);
    const Model& model = AssetManager::getAssetFromID<Model>(model_id_);

    for (int i = 0; i < model.parts_.size(); i++) {
        const Mesh& mesh = AssetManager::getAssetFromID<Mesh>(model.parts_[i].mesh_);
        const Material& material = AssetManager::getAssetFromID<Material>(model.parts_[i].material_);
        useMaterial(material, shader);
        
        if (outline_.active_) {
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
        }

        drawMesh(mesh);

        if (outline_.active_) {
            drawOutline(shader, mesh, transform);
        }

        if (debug_.visualizeNormals_) {
            drawNormals(shader, mesh, transform);
        }
    }
}

/*
 * Ran each time for every mesh, need to find a way to maybe map by material
 */
void bindTexture(const Shader& shader, unsigned int bind_num, const std::string& name, TextureID id) {
    glActiveTexture(GL_TEXTURE0 + bind_num);
    shader.setInt(name, bind_num);
    const Texture& texture = AssetManager::getAssetFromID<Texture>(id);
    glBindTexture(GL_TEXTURE_2D, texture.ID_);
}

void Renderer::useMaterial(const Material& material, const Shader& shader) {
    shader.use();
    const Texture& diffuse = AssetManager::getAssetFromID<Texture>(material.diffuse_);
    const Texture& specular = AssetManager::getAssetFromID<Texture>(material.specular_);
    glBindTextureUnit((int)TextureBinding::Diffuse, diffuse.ID_);
    glBindTextureUnit((int)TextureBinding::Specular, specular.ID_);
    shader.setFloat("material.shininess", material.shininess_);
    shader.setFloat("material.tesselationRate", material.tesselationRate_);
}

void Renderer::drawMesh(const Mesh& mesh) {
    glBindVertexArray(mesh.VAO_);
    glDrawElements(GL_TRIANGLES, mesh.indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::drawOutline(const Shader& shader, const Mesh& mesh, glm::mat4 transform) {
    const Shader& outline_shader = AssetManager::getAssetFromID<Shader>(AssetManager::defaultShaders().outline_);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    float factor = outline_.width_ + 1.0f;
    glm::mat4 scaled_transform = glm::scale(transform, glm::vec3(factor)); 
    outline_shader.use();
    outline_shader.setVec4("color", outline_.color_);
    outline_shader.setMat4("model", scaled_transform);
    getGameObject()->getGame()->applyGlobalUniforms(outline_shader);
    drawMesh(mesh);
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    shader.use();
}

void Renderer::drawNormals(const Shader& shader, const Mesh& mesh, glm::mat4 transform) {
    const Shader& normShader = AssetManager::getAssetFromID<Shader>(AssetManager::defaultShaders().visualizeNormals_);
    normShader.use();
    normShader.setMat4("model", transform);
    drawMesh(mesh);
    shader.use();
}
