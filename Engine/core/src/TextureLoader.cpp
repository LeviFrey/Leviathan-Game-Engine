#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>
#include <iostream>
#include <glad/glad.h>

Texture TextureLoader::loadTextureFromFile(
        const std::filesystem::path& path, 
        TextureFromFileContext context) 
{  
    int width, height, nrChannels;
    unsigned int ID;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, context.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, context.wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, context.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, context.magFilter);
    if (data) { 
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(
            GL_TEXTURE_2D, 
            0, 
            (format == GL_RGBA) ? GL_RGBA8 : GL_RGB8,
            width, 
            height, 
            0, 
            format, 
            GL_UNSIGNED_BYTE, 
            data
        );
        //glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout 
            << "FAILED TO LOAD TEXTURE: " << path.string() << std::endl
            << "Width,Height: " << width << ", " << height << " Channels: " << nrChannels << std::endl;
    }
    stbi_image_free(data);
    std::cout << "-- generated texture from: " << path.string() << ", with ID: " << ID << std::endl;
    return {ID};
}

Texture TextureLoader::loadHDRTextureFromFile(
        const std::filesystem::path& path,
        HDRFromFileContext context)
{
    int width, height, nrChannels;
    unsigned int ID;
    float* data = stbi_loadf(path.c_str(), &width, &height, &nrChannels, 0);
    std::cout << nrChannels << std::endl;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, context.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, context.wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, context.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, context.magFilter);
    if (data) { 
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(
            GL_TEXTURE_2D, 
            0, 
            (format == GL_RGBA) ? GL_RGBA16F : GL_RGB16F,
            width, 
            height, 
            0, 
            format, 
            GL_FLOAT, 
            data
        );
        //glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout
            << "FAILED TO LOAD HDR TEXTURE: " << path.string() << std::endl
            << "STB ERROR: :" << stbi_failure_reason() << std::endl
            << "Width,Height: " << width << ", " << height << " Channels: " << nrChannels << std::endl;
    }
    stbi_image_free(data);
    std::cout << "-- generated HDR texture from: " << path.string() << ", with ID: " << ID << std::endl;
    return {ID};

}

Texture TextureLoader::loadTextureFromData(
        const unsigned char* data, 
        unsigned int width, 
        unsigned int height,
        TextureFromDataContext context) 
{
    unsigned int ID;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, context.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, context.wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, context.minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, context.magFilter);
    glTexImage2D(
            GL_TEXTURE_2D, 
            0, 
            (context.format == GL_RGBA) ? GL_RGBA8 : GL_RGB8,
            width, 
            height, 
            0,
            context.format, 
            GL_UNSIGNED_BYTE, 
            data);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "texture generated from data failed to load!" << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return {ID};
}

Texture TextureLoader::loadCubemapFromFile(std::array<std::filesystem::path, 6> paths) {
    unsigned int ID; 
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    int width, height, nrChannels;
    unsigned char* data;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    for (unsigned int i = 0; i < paths.size(); i++) {
        data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << paths[i].string() << std::endl;
            stbi_image_free(data);
        }
    }
    return {ID};
}

Texture TextureLoader::createEmptyCubemap(
        unsigned int size, 
        EmptyCubemapContext context) 
{
    unsigned int ID;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    for (int i = 0; i < 6; i++) {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            (context.format == GL_FLOAT) ? GL_RGB16F : GL_RGB8,
            size,
            size,
            0,
            GL_RGB,
            context.format,
            nullptr
        );
    }
    
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "Oh no! Empty cubemap failed to load!" << std::endl;
    }
    return {ID};
};
