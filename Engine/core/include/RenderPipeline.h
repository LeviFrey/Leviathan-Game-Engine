#pragma once

// Objects
#include "Objects/ScreenObject.h"
#include "Objects/Skybox.h"
#include "Assets/FrameBuffer.h"
#include "Window.h"
#include "Assets/UniformBuffer.h"
#include "GPU/RenderBindings.h"
#include "Components/LightSource.h"
#include "Components/Camera.h"
#include <GLFW/glfw3.h>


class RenderPipeline {
    public:

        RenderPipeline(unsigned int width, unsigned int height, Window* window);

        void renderScreen(
            const std::vector<GameObject*>& objects,
            const std::vector<LightSource*>& lights,
            Camera* camera
        );
        
        void setScreenDimensions(unsigned int width, unsigned int height);

        Skybox& getSkybox() { return skybox_obj_; }

        void resetRenderConfig();
        
    private:

        // Pipeline functions
        void resetScreen();
        void writeToUBOs(
            const std::vector<LightSource*>& lights,
            Camera* camera
        );

        // -- UBOs --
        UniformBuffer<LightBlock> light_ubo_{
            static_cast<int>(UBOBinding::Lights)
        };

        UniformBuffer<CameraData> camera_ubo_{
            static_cast<int>(UBOBinding::Camera)
        };
 
        // 1) draw on the frame buffers texture
        // 2) draw the screen object onto the screen using the texture
        FrameBuffer frame_buffer_;
        
        // Optional render options
        bool use_frame_buffer_ = true;
        ScreenObject screen_; // for post processing
        Skybox skybox_obj_; 

        Window* window_;
};
