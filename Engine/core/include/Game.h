#pragma once

// From Standard
#include <vector>
#include <functional>

// From External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// From Engine
#include "Input/MouseHandler.h"
#include "Input/KeyboardHandler.h"
#include "Components/Camera.h"
#include "Components/Renderer.h"
#include "Components/LightSource.h"
#include "Shader.h"
#include "Texture.h"
#include "AssetTypes.h"
#include "GameObject.h"

class Game {
    public:
        Game(int window_width, int window_height);
        void setPostProcessingEffect(ShaderID shader) { postEffect_ = shader; }
        void setSkybox(TextureID texture) {
            hasSkybox_ = true;
            skyboxTexture_ = texture;
        }
        
        static void frameBufferSizeCallback(GLFWwindow* window, int width, int height); 
        static Game* GamePtr(GLFWwindow* window);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
        static void scrollCallback(GLFWwindow* window, double xpos, double ypos);

        MouseHandler* getMouseHandler();
        KeyboardHandler* getKeyboardHandler();
        Camera* getCamera() { return camera_; }

        void setCamera(Camera* camera);
        void setDebugFunction(std::function<void(Game&)> debugFunction);
        
        void Loop();
        void addGameObject(GameObject* obj);
        void addLightSource(LightSource* light);
        void applyGlobalUniforms(const Shader& shader);
    private:
        // -- Window config --
        int window_width_;
        int window_height_;
        std::function<void(Game&)> debugFunction_;
        
        // -- GLFW library stuff --
        GLFWwindow* window_;
        
        // -- Handlers --
        MouseHandler mouse_handler_;
        KeyboardHandler keyboard_handler_;

        //  -- Game object handling --
        std::vector<GameObject*> game_objects_;
        std::vector<LightSource*> light_sources_;
        Camera* camera_;
        
        // -- Global data UBOs --
        GLuint lightUBO_;
        GLuint cameraUBO_;
        
        // -- Frame Buffer config -- 
        ShaderID postEffect_;
        unsigned int frame_buffer_;
        unsigned int rbo_;
        Texture buffer_texture_;

        // -- skybox config --
        TextureID skyboxTexture_;
        bool hasSkybox_ = false;

        /*
         * Functions used for rendering pipeline
         */
        void writeToUBOs();
        void drawSkybox();
        void drawScreenBuffer();
};
