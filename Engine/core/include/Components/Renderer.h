#pragma once

#include "Component.h"
#include "AssetTypes.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

/*
 * Strictly Manipulates and Stores instructions (VAO) on how to read rendering data
 * TODO: allow for dynamic addition of data, such as positions for instance rendering
 */
class RenderDataConfig {
    public:
    void init() {
        glGenVertexArrays(1, &VAO_);
        glBindVertexArray(VAO_);

        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position_));
        glVertexAttribBinding(0, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal_));
        glVertexAttribBinding(1, 0);

        glEnableVertexAttribArray(2);
        glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoords_));
        glVertexAttribBinding(2, 0);

        glBindVertexArray(0);
    }
    void bind() {
        glBindVertexArray(VAO_);
    }
    
    private:
    unsigned int VAO_;
};

struct OutlineConfig {
    bool active_ = false;
    glm::vec4 color_;
    float width_;
};

struct DebugConfig {
    bool visualizeNormals_ = false;
};


class Renderer : public Component {
    public:
        Renderer(ModelID model, ShaderID shader);
        void render();
        void update() override {}
        ModelID getModel() { return model_id_; }
        ShaderID getShader() { return shader_id_; }
        OutlineConfig& getOutlineConfig() { return outline_; }
        DebugConfig& getDebugConfig() { return debug_; }
    private:
        std::vector<unsigned int> parts_;
        RenderDataConfigID setting_;
        ModelID model_id_;
        ShaderID shader_id_;
        void useMaterial(const Material& material, const Shader& shader);
        void drawMesh(const Mesh& mesh);
        void drawNormals(const Shader& shader, const Mesh& mesh, glm::mat4 transform);

        // Render Settings:
        OutlineConfig outline_{false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.05f};
        void drawOutline(const Shader& shader, const Mesh& mesh, glm::mat4 transform);
        
        DebugConfig debug_;
};

