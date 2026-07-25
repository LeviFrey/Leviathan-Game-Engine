#pragma once

#include <filesystem>
#include "Texture.h"    
#include <array>
#include <glad/glad.h>

struct TextureLoadContext {
    GLenum internalFormat = GL_RGBA;
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;
    GLenum wrapS = GL_REPEAT; // GL_REPEAT, GL_CLAMP_TO_EDGE
    GLenum wrapT = GL_REPEAT; // GL_REPEAT, GL_CLAMP_TO_EDGE
    GLenum wrapR = GL_REPEAT; // GL_REPEAT, GL_CLAMP_TO_EDGE
    GLenum minFilter = GL_LINEAR; // GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR
    GLenum magFilter = GL_LINEAR; // GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR
};

class TextureLoader {
    public:
        
        // Used for loading textures that use 8 bit per channel, with alpha and not with alpha
        struct TextureFromFileContext {
            GLenum wrapS = GL_REPEAT;
            GLenum wrapT = GL_REPEAT;
            GLenum minFilter = GL_LINEAR;
            GLenum magFilter = GL_LINEAR;
        };
        static Texture loadTextureFromFile(
                const std::filesystem::path& path, 
                TextureFromFileContext context);
        
        struct TextureFromDataContext {
            GLenum wrapS = GL_REPEAT;
            GLenum wrapT = GL_REPEAT;
            GLenum minFilter = GL_LINEAR;
            GLenum magFilter = GL_LINEAR;
            GLenum format = GL_RGBA;
        };
        static Texture loadTextureFromData(
                const unsigned char* data,
                unsigned int width,
                unsigned int height,
                TextureFromDataContext context);

        struct HDRFromFileContext {
            GLenum wrapS = GL_REPEAT;
            GLenum wrapT = GL_REPEAT;
            GLenum minFilter = GL_LINEAR;
            GLenum magFilter = GL_LINEAR; 
        };
        static Texture loadHDRTextureFromFile(
                const std::filesystem::path& path,
                HDRFromFileContext context);
 

        // posx, negx, posy, negy, posz, negz
        static Texture loadCubemapFromFile(
            std::array<std::filesystem::path, 6> paths);

        struct EmptyCubemapContext {
            GLenum format = GL_FLOAT;
        };
        static Texture createEmptyCubemap(
                unsigned int size, 
                EmptyCubemapContext context);
};
