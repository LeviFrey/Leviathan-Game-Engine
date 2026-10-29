#include "Assets/Texture.h"
#include <glad/glad.h>

Texture::Texture(Image&& image, TextureConfig config) :
    unique_image_(std::make_unique<Image>(std::move(image))),
    image_(unique_image_.get()),
    config_(config)
{}

Texture::Texture(const Image& image, TextureConfig config) : 
    unique_image_(nullptr),
    image_(&image),
    config_(config)
{}

Texture::Texture(const std::filesystem::path& path, TextureConfig config) :
    unique_image_(std::make_unique<Image>(path)),
    image_(unique_image_.get()),
    config_(config)
{}

unsigned int Texture::loadResource() {
    unsigned int tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB8,
        image_->getWidth(), image_->getHeight(),
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image_->getData()
    );

    glBindTexture(GL_TEXTURE_2D, 0);
    return tex_id;
}

void Texture::deleteResource() {
    unsigned int id = getResourceID();
    glDeleteTextures(1, &id);
}

// -- CUBEMAP --

Cubemap::Cubemap(std::array<std::filesystem::path, 6> paths) {
    for (int i = 0; i < 6; i++) {
        unique_faces_[i] = std::make_unique<Image>(paths[i]);
        faces_[i] = unique_faces_[i].get();
    }
}

Cubemap::Cubemap(std::array<Image, 6>&& faces) {
    for (int i = 0; i < 6; i++) {
        unique_faces_[i] = std::make_unique<Image>(std::move(faces[i]));
        faces_[i] = unique_faces_[i].get();
    }
}

Cubemap::Cubemap(std::array<const Image*, 6> faces) :
    faces_(faces)
{}
    

unsigned int Cubemap::loadResource() {
    unsigned int tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < 6; i++) {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            GL_RGB8,
            faces_.at(i)->getWidth(),
            faces_.at(i)->getHeight(),
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            faces_.at(i)->getData()
        );
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    return tex_id;
}
