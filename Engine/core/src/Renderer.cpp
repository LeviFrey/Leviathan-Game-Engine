#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "GameObject.h"
#include "Game.h"
#include "AssetManager.h"
#include "EngineAssets.h"
#include "RenderBindings.h"

Renderer::Renderer(const Model& model, const Shader& shader) : 
    model_(model),
    shader_(shader), 
    render_config_(AssetManager::getAssetByID<RenderDataConfig>(EngineAssets::getDefaultRDC())) {}

void Renderer::render() {
    shader_.use();
    glm::mat4 transform = getGameObject()->getComponent<Transform>()->getWorldMatrix();
    shader_.setMat4("model", transform);
    getGameObject()->getGame()->applyGlobalUniforms(shader_);
    
    const std::vector<RenderPart>& model_parts = model_.getParts();
    for (int i = 0; i < model_parts.size(); i++) {
        const Material& material = model_parts[i].material_;
        const Mesh& mesh = model_parts[i].mesh_;
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

void Renderer::useMaterial(const Material& material) {
    shader_.use();
    glBindTextureUnit((int)TextureBinding::Diffuse, material.diffuse_.ID_);
    glBindTextureUnit((int)TextureBinding::Specular, material.specular_.ID_);
    shader_.setFloat("material.shininess", material.shininess_);
    shader_.setFloat("material.tesselationRate", material.tesselationRate_);
}

void Renderer::drawMesh(const Mesh& mesh) {
    render_config_.bind();
    glVertexArrayVertexBuffer(render_config_.getVAO(), 0, mesh.VBO_, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(render_config_.getVAO(), mesh.EBO_);
    glDrawElements(GL_TRIANGLES, mesh.num_indices_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    /*
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, mesh.indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    */
}

void Renderer::drawOutline(const Mesh& mesh, glm::mat4 transform) {
    const ShaderID& outline_shader_id = EngineAssets::getOutlineShader();
    const Shader& outline_shader = AssetManager::getAssetByID<Shader>(outline_shader_id);

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
    shader_.use();
}

void Renderer::drawNormals(const Mesh& mesh, glm::mat4 transform) {
    const ShaderID& norm_shader_id = EngineAssets::getVisualizeNormalShader();
    const Shader& norm_shader = AssetManager::getAssetByID<Shader>(norm_shader_id);
    norm_shader.use();
    norm_shader.setMat4("model", transform);
    drawMesh(mesh);
    shader_.use();
}
