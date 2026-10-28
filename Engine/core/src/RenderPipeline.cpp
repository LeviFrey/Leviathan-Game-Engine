#include "RenderPipeline.h"

RenderPipeline::RenderPipeline(
        unsigned int width, 
        unsigned int height,
        Window* window) :
    frame_buffer_{width, height},
    screen_(std::make_unique<ScreenObject>(width, height, frame_buffer_.getTexture())),
    window_(window)
{}
    
void RenderPipeline::renderScreen(const std::vector<GameObject*>& objects) {

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

    if (skybox_obj_) {
        skybox_obj_->getComponents()->get<Renderer>()->render();
    }
    
    if (use_frame_buffer_) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        screen_->getComponents()->get<Renderer>()->render();
    }

    glfwSwapBuffers(window_->getGLFWPtr());
    
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
