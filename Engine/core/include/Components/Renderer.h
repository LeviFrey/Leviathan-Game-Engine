#pragma once

#include "Components/Component.h"
#include "Assets/Mesh.h"
#include "Assets/Material.h"
#include "Assets/Shader.h"
#include "EngineAssets.h"
#include "Assets/Model.h"

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

        Renderer(
            Model* model = nullptr, 
            Shader* shader = &EngineAssets::PHONG_SHADER, 
            VertexArrayObject* vao = &EngineAssets::DEFAULT_VAO
        );

        // Component inherited
        void update() override {}
        
        // Renderer Utilization
        void render();
        void setShader(Shader* shader) { main_shader_ = shader; }

        // Possibly temporary config options
        OutlineConfig& getOutlineConfig() { return outline_; }
        DebugConfig& getDebugConfig() { return debug_; }

    private:
        
        // DATA
        VertexArrayObject* vao_;
        Model* model_;
        Shader* main_shader_;

        Shader* outline_shader_ = nullptr;
        Shader* norm_shader_ = nullptr;
        Cubemap* skybox_ = nullptr;


        // HELPER FUNCTIONS
        void useMaterial(Material* material);
        void drawMesh(Mesh* mesh);
        void drawNormals(Mesh* mesh, glm::mat4 transform);

        // Render Settings:
        OutlineConfig outline_{false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.05f};
        void drawOutline(Mesh* mesh, glm::mat4 transform);
        
        DebugConfig debug_;
};

