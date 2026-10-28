// From Engine
#include "Game.h"
#include "GameObject.h"
#include "Components/LightSource.h"
#include "DeltaClock.h"
#include "RenderBindings.h"
#include "CameraData.h"
#include "Components/Camera.h"
#include "Debug.h"
#include "Components/FreeCamera.h"

// From Standard Library
#include <iostream>

// From External
#include <glad/glad.h>
#include <glm/glm.hpp>

void Game::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    GamePtr(window)->getCamera()->setAspectRatio((float)width/(float)height);
}

Game* Game::GamePtr(GLFWwindow* window) {
    return static_cast<Game*>(glfwGetWindowUserPointer(window));
}

void Game::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    GamePtr(window)->input_.keyboard_handler.handleKeyPress(window, key, scancode, action, mods);
}

void Game::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    GamePtr(window)->input_.mouse_handler.handleMouseMovement(window, xpos, ypos);
}

void Game::scrollCallback(GLFWwindow* window, double xpos, double ypos) {
    GamePtr(window)->input_.mouse_handler.handleMouseScroll(window, xpos, ypos);
}

void Game::addGameObject(GameObject* obj) {
    game_objects_.push_back(obj);
}

void Game::addLightSource(LightSource* light) {
    light_sources_.push_back(light);
}

void Game::setDebugFunction(std::function<void(Game&)> debugFunction) {
    debugFunction_ = debugFunction;
}

/*
 * Creates OpenGL context and spawns window
 */
Game::Game(int window_width, int window_height) :
    window_width_(window_width), window_height_(window_height),
    window_(window_width, window_height, "GAME"),
    render_pipeline_(window_width, window_height, &window_)
{

    // set callback functions
    glfwSetWindowUserPointer(window_.getGLFWPtr(), this);
    glfwSetFramebufferSizeCallback(window_.getGLFWPtr(), Game::frameBufferSizeCallback);
    glfwSetKeyCallback(window_.getGLFWPtr(), Game::keyCallback);
    glfwSetCursorPosCallback(window_.getGLFWPtr(), Game:: mouseMoveCallback);

    DEBUG_PRINT("Creating Game...");

    // generate light UBO
    glGenBuffers(1, &lightUBO_);
    glBindBuffer(GL_UNIFORM_BUFFER, lightUBO_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightBlock), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, (int)UBOBinding::Lights, lightUBO_);

    // generate camera UBO
    glGenBuffers(1, &cameraUBO_);
    glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraData), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, (int)UBOBinding::Camera, cameraUBO_);
    
    // Setup camera
    camera_.addComponent<FreeCamera>(&input_);
    camera_.addComponent<Transform>();
    addGameObject(&camera_);

    // Setup Render Pipeline
    render_pipeline_.resetRenderConfig();
}


/*
 * Only called once after all objects have been generated
 */
void Game::Loop() {

    DEBUG_PRINT("Starting Game Loop");

    while(!glfwWindowShouldClose(window_.getGLFWPtr())) {

        // gather user input and update input handlers
        input_.keyboard_handler.clearStates();
        glfwPollEvents();
        input_.mouse_handler.update(); 

        // check exit
        if (input_.keyboard_handler.getKeyDown(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window_.getGLFWPtr(), true);
        }

        DeltaClock::tick();

        // Update all game objects
        for (GameObject* obj : game_objects_) {
            obj->updateComponents();
        }

        writeToUBOs();

        render_pipeline_.renderScreen(game_objects_);
        
        // call debug function
        if (debugFunction_) {
            debugFunction_(*this);
        }
    }
}

void Game::writeToUBOs() {
    // fill Light UBO
    LightBlock light_block;
    int lightsFound = 0;
    for (int i = 0; i < light_sources_.size(); i++) {
        if (light_sources_[i]->getOn()) {
            light_block.light_data[lightsFound++] = light_sources_[i]->packLightData();
        }
    }
    light_block.lightCount[0] = lightsFound;
    glBindBuffer(GL_UNIFORM_BUFFER, lightUBO_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightBlock), &light_block);
        
    // fill Camera UBO
    FreeCamera* c = camera_.getComponents()->get<FreeCamera>();
    CameraData cameraData = c->getData();
    glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraData), &cameraData); 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

/*
void Game::drawScreenBuffer() {
    glDisable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    const Shader& shader = AssetManager::accessAsset<Shader>(postEffect_);
    const Mesh& quad = AssetManager::accessAsset<Mesh>(AssetManager::defaultMeshes().quad_);

    shader.use();
    glBindTextureUnit((int)TextureBinding::PostProcess, buffer_texture_.ID_);
    RenderDataConfig& rdc = AssetManager::accessAsset<RenderDataConfig>(AssetManager::defaultRDC());
    rdc.bind();
    glBindVertexBuffer(0, quad.VBO_, 0, sizeof(Vertex));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.EBO_);
    //glBindVertexArray(quad.VAO_);
    glDrawElements(GL_TRIANGLES, quad.num_indices_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Game::drawSkybox() {
    glDepthFunc(GL_LEQUAL);
    const Shader& shader = AssetManager::accessAsset<Shader>(AssetManager::defaultShaders().skybox_);
    const Mesh& mesh = AssetManager::accessAsset<Mesh>(AssetManager::defaultMeshes().cube_);

    shader.use();
    applyGlobalUniforms(shader);
    RenderDataConfig& rdc = AssetManager::accessAsset<RenderDataConfig>(AssetManager::defaultRDC());
    rdc.bind();
    glBindVertexBuffer(0, mesh.VBO_, 0, sizeof(Vertex));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO_);
    //glBindVertexArray(mesh.VAO_);
    glDrawElements(GL_TRIANGLES, mesh.num_indices_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}
*/
