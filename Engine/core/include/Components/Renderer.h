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
        glCreateVertexArrays(1, &VAO_);

        glVertexArrayAttribFormat(VAO_, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position_));
        glVertexArrayAttribBinding(VAO_, 0, 0);

        glVertexArrayAttribFormat(VAO_, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal_));
        glVertexArrayAttribBinding(VAO_, 1, 0);

        glVertexArrayAttribFormat(VAO_, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoords_));
        glVertexArrayAttribBinding(VAO_, 2, 0);

        glEnableVertexArrayAttrib(VAO_, 0);
        glEnableVertexArrayAttrib(VAO_, 1);
        glEnableVertexArrayAttrib(VAO_, 2);

        location_index_ = 3;
    }

    void addAttribute(int dim, size_t offset) {
        if (location_index_ == 0) { init(); }
        glVertexArrayAttribFormat(VAO_, location_index_, dim, GL_FLOAT, GL_FALSE, offset);
        glVertexArrayAttribBinding(VAO_, location_index_, 1);
        glEnableVertexArrayAttrib(VAO_, location_index_);
        glVertexArrayBindingDivisor(VAO_, 1, 1);
        location_index_++;
    }
    unsigned int getVAO() { return VAO_; }

    void bind() {
        glBindVertexArray(VAO_);
    }
    
    private:
    unsigned int VAO_;
    int location_index_ = 0;
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

        Renderer(Model* model, Shader* shader);

        // Main utilization
        void render();
        void update() override {}

        // Possibly temporary config options
        OutlineConfig& getOutlineConfig() { return outline_; }
        DebugConfig& getDebugConfig() { return debug_; }

    private:
        RenderDataConfigID setting_;
        Model* model_;
        Shader* shader_;
        void useMaterial(const Material& material);
        void drawMesh(const Mesh& mesh);
        void drawNormals(const Mesh& mesh, glm::mat4 transform);

        // Render Settings:
        OutlineConfig outline_{false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.05f};
        void drawOutline(const Mesh& mesh, glm::mat4 transform);
        
        DebugConfig debug_;
};

