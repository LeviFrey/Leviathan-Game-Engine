#pragma once
#include "Image.h"
#include "GPUResource.h"
#include <array>
#include <filesystem>
#include <glad/glad.h>

class TextureBase : public GPUResource {};

struct TextureConfig {
    GLuint wrapT = GL_REPEAT; // GL_REPEAT, GL_CLAMP_TO_EDGE
    GLuint wrapS = GL_REPEAT; // GL_REPEAT, GL_CLAMP_TO_EDGE
};

class Texture : public TextureBase {
    public:


        Texture(Image&& image, TextureConfig config = {}); 
        Texture(const Image& image, TextureConfig config = {}); 
        Texture(const std::filesystem::path& path, TextureConfig config = {});

        ~Texture() override {
            unload(); 
        }
        
        // Heres a good move operator I think??
        Texture& operator=(Texture&& other) noexcept {
            if (this != &other) {

                // transfer resource
                takeResource(std::move(other)); 

                // transfer image
                unique_image_ = std::move(other.unique_image_);
                image_ = std::exchange(other.image_, nullptr);
                config_ = std::exchange(other.config_, TextureConfig());
            }
            return *this;
        }
                

    private:

        unsigned int loadResource() override;
        void deleteResource() override;

        // CPU side
        std::unique_ptr<Image> unique_image_; 
        const Image* image_;
        TextureConfig config_;
};

class Cubemap : public TextureBase {
    public:

        // Faces should be input as: 
        // positive x, negative x,
        // positive y, negative y,
        // positive z, negative z
        Cubemap(std::array<Image, 6>&& faces);
        Cubemap(std::array<const Image*, 6> faces);
        Cubemap(const std::array<std::filesystem::path, 6>& paths);

    private:
        unsigned int loadResource() override;

        std::array<std::unique_ptr<Image>, 6> unique_faces_;
        std::array<const Image*, 6> faces_;


};
