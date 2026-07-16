#pragma once
#include <glad/glad.h>
#include <filesystem>

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

/*
 * Helpful tool to convert equirectangular images to cubemaps
 */
class CubemapConverter {
    public:
        static Texture equirectangularToCubemap(const std::filesystem::path& path);
    private:
        static void init();
        inline static GLuint captureFBO_ = 0;
        inline static GLuint captureRBO_ = 0;
        inline static bool initialized_ = false;
        inline static Shader shader_ = {0};
        inline static Mesh cube_ = {};
};
