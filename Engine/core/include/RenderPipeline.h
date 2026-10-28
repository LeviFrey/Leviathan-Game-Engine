#pragma once

// Objects
#include "Objects/ScreenObject.h"
#include "Objects/Skybox.h"
#include "FrameBuffer.h"
#include <GLFW/glfw3.h>


class RenderPipeline {
    public:

        RenderPipeline(unsigned int width, unsigned int height);

        void renderScreen(const std::vector<GameObject*>& objects);
        
        void setScreenDimensions(unsigned int width, unsigned int height);
        Skybox* getSkybox() { return skybox_obj_.get(); }

        void setWindow(GLFWwindow* window) { window_ = window; }
        void resetRenderConfig();
        
    private:

        // Pipeline functions
        void resetScreen();
        
        // 1) draw on the frame buffers texture
        // 2) draw the screen object onto the screen using the texture
        FrameBuffer frame_buffer_;
        
        // Optional render options
        bool use_frame_buffer_ = false;
        std::unique_ptr<ScreenObject> screen_; // for post processing
        std::unique_ptr<Skybox> skybox_obj_ = nullptr; 

        GLFWwindow* window_;
};
