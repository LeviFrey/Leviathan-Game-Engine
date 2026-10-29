#pragma once
#include "Assets/Texture.h"
#include "Assets/Shader.h"
#include "EngineAssets.h"

class Material {
    public:
        virtual ~Material() = default;
        virtual void use(Shader* shader) const {};
};

class PhongMaterial : public Material {
    public:

        enum class TextureSlot {
            Diffuse = 0,
            Specular = 1
        };

        PhongMaterial(
                Texture* diffuse = &EngineAssets::DEFAULT_TEXTURE, 
                Texture* specular = &EngineAssets::DEFAULT_TEXTURE, 
                float shininess = 32.0f, 
                float tesselation_rate = 1.0f) :
            diffuse_(diffuse), 
            specular_(specular), 
            shininess_(shininess), 
            tesselation_rate_(tesselation_rate) 
        {}

        void use(Shader* shader) const override {
            glBindTextureUnit((int)TextureSlot::Diffuse, diffuse_->getResource());
            glBindTextureUnit((int)TextureSlot::Specular, specular_->getResource());
            shader->setFloat("material.shininess", shininess_);
            shader->setFloat("material.tesselationRate", tesselation_rate_);
        }

        Texture* diffuse_;
        Texture* specular_;
        float shininess_;
        float tesselation_rate_;
}; 

class BufferMaterial : public Material {
    public:
        enum class TextureSlot {
            Buffer = 0
        };

        BufferMaterial(Texture* buffer) :
            buffer_(buffer)
        {}

        void use(Shader* shader) const override {
            glBindTextureUnit((int)TextureSlot::Buffer, buffer_->getResource());
        }
        
        Texture* buffer_;
};

class CubemapMaterial : public Material {
    public:
        enum class TextureSlot {
            Cubemap = 0
        };

        CubemapMaterial(Cubemap* cubemap = &EngineAssets::DEFAULT_CUBEMAP) :
            cubemap_(cubemap)
        {}

        void use(Shader* shader) const override {
            glBindTextureUnit((int)TextureSlot::Cubemap, cubemap_->getResource());
        }
        
        Cubemap* cubemap_;
};

