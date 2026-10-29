#include "RenderPipeline.h"

RenderPipeline::RenderPipeline(
        unsigned int width, 
        unsigned int height,
        Window* window) :
    frame_buffer_{width, height},
    screen_(width, height, frame_buffer_.getTexture()),
    window_(window)
{}
    
void RenderPipeline::renderScreen(
        const std::vector<GameObject*>& objects,
        const std::vector<LightSource*>& lights,
        Camera* camera
) {

    writeToUBOs(lights, camera);

    if (use_frame_buffer_) {
        frame_buffer_.bind();
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    resetScreen();

    for (GameObject* obj : objects) {
        if (obj->getComponents()->contains<Renderer>()) {
            obj->getComponents()->get<Renderer>()->render();
        }
    }

    if (skybox_obj_.getTexture()) {
        glDepthFunc(GL_LEQUAL);
        skybox_obj_.getComponents()->get<Renderer>()->render();
        glDepthFunc(GL_LESS);
    }
    
    if (use_frame_buffer_) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        screen_.getComponents()->get<Renderer>()->render();
    }

    glfwSwapBuffers(window_->getGLFWPtr());
    
}

void RenderPipeline::writeToUBOs(
    const std::vector<LightSource*>& lights,
    Camera* camera
) {
    LightBlock light_block;
    int lightsFound = 0;
    for (int i = 0; i < lights.size(); i++) {
        if (lights[i]->getOn()) {
            light_block.light_data[lightsFound++] = lights[i]->packLightData();
        }
    }
    light_block.lightCount[0] = lightsFound;
    light_ubo_.bufferData(&light_block);
        
    // fill Camera UBO
    CameraData cameraData = camera->getData();
    camera_ubo_.bufferData(&cameraData); 
}

void RenderPipeline::resetScreen() {
    glStencilMask(0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glStencilMask(0x00);
    glEnable(GL_DEPTH_TEST);
}

void RenderPipeline::resetRenderConfig() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x00);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glClearColor(255.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f, 1.0f);
}
