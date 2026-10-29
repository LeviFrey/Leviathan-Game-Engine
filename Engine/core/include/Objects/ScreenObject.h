#pragma once
#include "GameObject.h"
#include "Assets/Shader.h"
#include "Shapes.h"
#include "Assets/Texture.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"
#include "EngineAssets.h"

class ScreenObject : public GameObject {
    public:

        ScreenObject(
            unsigned int width, 
            unsigned int height,
            Texture* texture,
            Shader* post_effect_shader = &EngineAssets::DEFAULT_SCREEN_SHADER

        ) 
            : width_(width), height_(height) 
        {
            model_ = std::make_unique<Model>();
            material_ = model_->addUniqueMaterial<BufferMaterial>(texture); 
            material_->buffer_ = texture;

            Mesh mesh = Shapes::createQuad(); // not the right size at all
            Mesh* mesh_ptr = model_->addUniqueMesh(mesh);

            model_->addPart(mesh_ptr, material_);
            
            addComponent<Renderer>(model_.get(), post_effect_shader);
            addComponent<Transform>();
        }
        
        void setPostEffect(Shader* post_effect_shader) {
            Renderer* r = getComponents()->get<Renderer>();
            r->setShader(post_effect_shader);
        }

        unsigned int getWidth() { return width_; }
        unsigned int getHeight() { return height_; }

        void setTexture(Texture* texture) {
            material_->buffer_ = texture;
        }

    private:

        BufferMaterial* material_;
        std::unique_ptr<Model> model_;

        // screen configuration 
        unsigned int width_;
        unsigned int height_;
}; 
