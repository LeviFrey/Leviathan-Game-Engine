#pragma once

// From Standard
#include <vector>
#include <functional>

// From External
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// From Engine
#include "Components/Camera.h"
#include "Components/LightSource.h"
#include "AssetTypes.h"
#include "GameObject.h"
#include "RenderPipeline.h"
#include "AssetManager.h"
#include "Input/InputHandler.h"

class Game {
    public:
        Game(int window_width, int window_height);

        void setSkybox(TextureID texture) {
            hasSkybox_ = true;
            skyboxTexture_ = texture;
        }

        Camera* getCamera() { 
            return camera_.getComponents()->get<Camera>();
        }
        
        // TODO: make these non-static
        static Game* GamePtr(GLFWwindow* window);
        static void frameBufferSizeCallback(GLFWwindow* window, int width, int height); 
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
        static void scrollCallback(GLFWwindow* window, double xpos, double ypos);
        void setDebugFunction(std::function<void(Game&)> debugFunction);

        RenderPipeline& getRenderPipeline() { return render_pipeline_; }

        void Loop();
        void addGameObject(GameObject* obj);
        void addLightSource(LightSource* light);
        InputHandler& getInputHandler() { return input_; }
    private:
 
        // -- Game Context --
        AssetManager asset_manager_;
        Window window_;
        InputHandler input_;

        // -- Game Loop Processing abstractions --
        RenderPipeline render_pipeline_;

        // -- Window config --
        int window_width_;
        int window_height_;
        std::function<void(Game&)> debugFunction_;
        
        //  -- Game object handling --
        std::vector<GameObject*> game_objects_;
        std::vector<LightSource*> light_sources_;

        GameObject camera_;
        
        // -- Global data UBOs --
        GLuint lightUBO_;
        GLuint cameraUBO_;
        
        // -- skybox config --
        TextureID skyboxTexture_;
        bool hasSkybox_ = false;

        /*
         * Functions used for rendering pipeline
         */
        void writeToUBOs();
};
