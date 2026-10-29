#pragma once
#include <iostream>
#include "PathUtils.h"
#include "Assets/Texture.h"
#include "Assets/Shader.h"
#include "Assets/Mesh.h"
#include "Shapes.h"
#include "Assets/VertexArrayObject.h"
#include "Debug.h"

namespace EngineAssets {

        inline const bool start = [] {
        DEBUG_PRINT("Creating Engine Assets..");
        return true;
        }();

        inline const Image DEFAULT_IMAGE = [] {
            const unsigned char black[] = {0, 0, 0, 255};
            return Image(black, 1, 1, 4);
        }();
        inline const bool image = [] {
        DEBUG_PRINT("Created Default Image!");
        return true;
        }();


        inline Texture DEFAULT_TEXTURE(DEFAULT_IMAGE);
        inline const bool texture = [] {
        DEBUG_PRINT("Created Default Texture!");
        return true;
        }();

        inline Cubemap DEFAULT_CUBEMAP({
            &DEFAULT_IMAGE, &DEFAULT_IMAGE,
            &DEFAULT_IMAGE, &DEFAULT_IMAGE,
            &DEFAULT_IMAGE, &DEFAULT_IMAGE
        });
        inline const bool cubemap = [] {
        DEBUG_PRINT("Created Default Cubemap!");
        return true;
        }();

        inline Mesh DEFAULT_CUBE = Shapes::createCube(1.0f);
        inline const bool cube = [] {
        DEBUG_PRINT("Created Default Mesh!");
        return true;
        }();

        inline VertexArrayObject DEFAULT_VAO = VertexArrayObject();
        inline const bool vao = [] {
        DEBUG_PRINT("Created Default VAO!");
        return true;
        }();

        inline Shader PHONG_SHADER(
            PathUtils::shaderDir / "default/default.vert",
            PathUtils::shaderDir / "default/default.frag"
        );
        inline const bool phong = [] {
        DEBUG_PRINT("Created Phong shader!");
        return true;
        }();

        inline Shader SKYBOX_SHADER(
            PathUtils::shaderDir / "skybox/skybox.vert",
            PathUtils::shaderDir / "skybox/skybox.frag"
        );
        inline const bool skybox = [] {
        DEBUG_PRINT("Created Skybox shader!");
        return true;
        }();

        inline Shader DEFAULT_SCREEN_SHADER (
            PathUtils::shaderDir / "postprocess/screen.vert",
            PathUtils::shaderDir / "postprocess/defaultScreen.frag"
        );
        inline const bool screen = [] {
        DEBUG_PRINT("Created Screen shader!");
        return true;
        }();

        inline const bool finished = [] {
        DEBUG_PRINT("Finished generating Engine Assets!");
        return true;
        }();

};

