#pragma once
#include "GameObject.h"
#include "EngineAssets.h"
#include "Components/Renderer.h"
#include "Components/Transform.h"

class Skybox : public GameObject {
    public:
        Skybox(Cubemap* texture = nullptr) {
            model_ = std::make_unique<Model>();
            material_ = model_->addUniqueMaterial<CubemapMaterial>(texture);

            model_->addPart(&EngineAssets::DEFAULT_CUBE, material_); 
            
            addComponent<Renderer>(
                model_.get(), 
                &EngineAssets::SKYBOX_SHADER
            );

            addComponent<Transform>();
        }

        void setTexture(Cubemap* texture) {
            material_->cubemap_ = texture; 
        }

        Cubemap const * getTexture() const { 
            return material_->cubemap_; 
        }

    private:

        // model-owned material
        CubemapMaterial* material_ = nullptr;

        // owned model
        std::unique_ptr<Model> model_ = nullptr;
};
