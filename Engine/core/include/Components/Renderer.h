#pragma once

#include "Component.h"
#include "AssetTypes.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "RenderDataConfig.h"

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

        Renderer(const Model& model, const Shader& shader);

        // Main utilization
        void render();
        void update() override {}

        // Possibly temporary config options
        OutlineConfig& getOutlineConfig() { return outline_; }
        DebugConfig& getDebugConfig() { return debug_; }

    private:
        const RenderDataConfig& render_config_;
        const Model& model_;
        const Shader& shader_;
        void useMaterial(const Material& material);
        void drawMesh(const Mesh& mesh);
        void drawNormals(const Mesh& mesh, glm::mat4 transform);

        // Render Settings:
        OutlineConfig outline_{false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.05f};
        void drawOutline(const Mesh& mesh, glm::mat4 transform);
        
        DebugConfig debug_;
};

