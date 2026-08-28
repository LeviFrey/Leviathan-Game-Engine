#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "GameObject.h"
#include "Game.h"
#include "AssetManager.h"
#include "RenderBindings.h"

Renderer::Renderer(Model* model, Shader* shader) : 
    model_(model),
    shader_(shader) {
    setting_ = AssetManager::defaultRDC();
}

void Renderer::render() {
    shader_->use();
    glm::mat4 transform = getGameObject()->getComponent<Transform>()->getWorldMatrix();
    shader_->setMat4("model", transform);
    getGameObject()->getGame()->applyGlobalUniforms(*shader_);

    for (int i = 0; i < model_->parts_.size(); i++) {
        Material material = model_->parts_[i].material_;
        Mesh mesh = model_->parts_[i].mesh_;
        useMaterial(material);
        
        if (outline_.active_) {
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);
        }

        drawMesh(mesh);

        if (outline_.active_) {
            drawOutline(mesh, transform);
        }

        if (debug_.visualizeNormals_) {
            drawNormals(mesh, transform);
        }
    }
}

/*
 * Ran each time for every mesh, need to find a way to maybe map by material
 */
void bindTexture(const Shader& shader, unsigned int bind_num, const std::string& name, TextureID id) {
    glActiveTexture(GL_TEXTURE0 + bind_num);
    shader.setInt(name, bind_num);
    const Texture& texture = AssetManager::accessAsset<Texture>(id);
    glBindTexture(GL_TEXTURE_2D, texture.ID_);
}

void Renderer::useMaterial(const Material& material) {
    shader_->use();
    glBindTextureUnit((int)TextureBinding::Diffuse, material.diffuse_.ID_);
    glBindTextureUnit((int)TextureBinding::Specular, material.specular_.ID_);
    shader_->setFloat("material.shininess", material.shininess_);
    shader_->setFloat("material.tesselationRate", material.tesselationRate_);
}

void Renderer::drawMesh(const Mesh& mesh) {
    RenderDataConfig& rdc = AssetManager::accessAsset<RenderDataConfig>(setting_);
    rdc.bind();
    glVertexArrayVertexBuffer(rdc.getVAO(), 0, mesh.VBO_, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(rdc.getVAO(), mesh.EBO_);
    glDrawElements(GL_TRIANGLES, mesh.num_indices_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    /*
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, mesh.indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    */
}

void Renderer::drawOutline(const Mesh& mesh, glm::mat4 transform) {
    const Shader& outline_shader = AssetManager::accessAsset<Shader>(AssetManager::defaultShaders().outline_);

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
    shader_->use();
}

void Renderer::drawNormals(const Mesh& mesh, glm::mat4 transform) {
    const Shader& normShader = AssetManager::accessAsset<Shader>(AssetManager::defaultShaders().visualizeNormals_);
    normShader.use();
    normShader.setMat4("model", transform);
    drawMesh(mesh);
    shader_->use();
}
