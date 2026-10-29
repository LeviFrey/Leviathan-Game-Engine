// From Engine
#include "Game.h"
#include "GameObject.h"
#include "Components/LightSource.h"
#include "DeltaClock.h"
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
    window_(window_width, window_height, "GAME"),
    render_pipeline_(window_width, window_height, &window_)
{

    // set callback functions
    glfwSetWindowUserPointer(window_.getGLFWPtr(), this);
    glfwSetFramebufferSizeCallback(window_.getGLFWPtr(), Game::frameBufferSizeCallback);
    glfwSetKeyCallback(window_.getGLFWPtr(), Game::keyCallback);
    glfwSetCursorPosCallback(window_.getGLFWPtr(), Game:: mouseMoveCallback);

    DEBUG_PRINT("Creating Game...");

    // TODO: add configurable camera. For now, camera is locked to this option
    camera_.addComponent<FreeCamera>(&input_);
    camera_.addComponent<Transform>();
    addGameObject(&camera_);
}


/*
 * Only called once after all objects have been generated
 */
void Game::Loop() {

    DEBUG_PRINT("Starting Game Loop");
    render_pipeline_.resetRenderConfig();

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

        render_pipeline_.renderScreen(
            game_objects_, 
            light_sources_, 
            camera_.getComponents()->get<FreeCamera>()
        );
        
        // call debug function
        if (debugFunction_) {
            debugFunction_(*this);
        }
    }
}
