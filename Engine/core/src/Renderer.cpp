#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "Debug.h"
#include <GLFW/glfw3.h>


Renderer::Renderer(Model* model, Shader* shader, VertexArrayObject* vao) :
model_(model), main_shader_(shader), vao_(vao)
{}

void Renderer::render() {
    main_shader_->use();

    Transform* t = getSiblingComponents()->get<Transform>(); 
    glm::mat4 transform = t->getWorldMatrix();
    main_shader_->setMat4("model", transform);
    main_shader_->setFloat("time", glfwGetTime());
    
    // reconsider this definitely, why does shader need this global information?
    if (skybox_) {
        glBindTextureUnit(2, skybox_->getResource());
    }
         
    const std::vector<RenderPart>& model_parts = model_->getParts();
    for (int i = 0; i < model_parts.size(); i++) {
        Material* material = model_parts[i].material_;
        Mesh* mesh = model_parts[i].mesh_;
        material->use(main_shader_);
        
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

void Renderer::drawMesh(Mesh* mesh) {
    vao_->bind();
    vao_->bindVertexBuffer(0, mesh->getVertexBuffer());
    vao_->bindElementBuffer(mesh->getElementBuffer());
    glDrawElements(
        GL_TRIANGLES, 
        mesh->getElementBuffer().getNumIndices(),
        GL_UNSIGNED_INT,
        nullptr
    );
    glBindVertexArray(0);

    /*
    glVertexArrayVertexBuffer(render_config_->getVAO(), 0, mesh->VBO_, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(render_config_->getVAO(), mesh->EBO_);
    glDrawElements(GL_TRIANGLES, mesh->num_indices_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    */

    /*
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, mesh.indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    */
}

void Renderer::drawOutline(Mesh* mesh, glm::mat4 transform) {

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    float factor = outline_.width_ + 1.0f;
    glm::mat4 scaled_transform = glm::scale(transform, glm::vec3(factor)); 
    outline_shader_->use();
    outline_shader_->setVec4("color", outline_.color_);
    outline_shader_->setMat4("model", scaled_transform);
    drawMesh(mesh);
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    main_shader_->use();
}

void Renderer::drawNormals(Mesh* mesh, glm::mat4 transform) {
    norm_shader_->use();
    norm_shader_->setMat4("model", transform);
    drawMesh(mesh);
    main_shader_->use();
}
