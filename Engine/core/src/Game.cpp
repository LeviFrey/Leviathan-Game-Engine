// From Engine
#include "Game.h"
#include "Components/Renderer.h"
#include "GameObject.h"
#include "Components/LightSource.h"
#include "DeltaClock.h"
#include "PathUtils.h"
#include "AssetManager.h"
#include "TextureLoader.h"
#include "RenderBindings.h"
#include "CameraData.h"

// From Standard Library
#include <iostream>

// From External
#include <glad/glad.h>
#include <glm/glm.hpp>

void printVectr(glm::vec3 vector) {
    std::cout << vector.x << " " << vector.y << " " << vector.z << std::endl;
}

void Game::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    GamePtr(window)->getCamera()->setAspectRatio((float)width/(float)height);
}

Game* Game::GamePtr(GLFWwindow* window) {
    return static_cast<Game*>(glfwGetWindowUserPointer(window));
}

void Game::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    GamePtr(window)->getKeyboardHandler()->handleKeyPress(window, key, scancode, action, mods);
}

void Game::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
    GamePtr(window)->getMouseHandler()->handleMouseMovement(window, xpos, ypos);
}

void Game::scrollCallback(GLFWwindow* window, double xpos, double ypos) {
    GamePtr(window)->getMouseHandler()->handleMouseScroll(window, xpos, ypos);
}

MouseHandler* Game::getMouseHandler() {
    return &mouse_handler_;
}

KeyboardHandler* Game::getKeyboardHandler() {
    return &keyboard_handler_;
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
Game::Game(int window_width, int window_height) {

    // intialize window
    window_width_ = window_width;
    window_height_ = window_height;
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "ENGINE ERROR: with code: " << error << " and desc: " << description << std::endl;
    });

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);

    window_ = glfwCreateWindow(window_width_, window_height_, "GAME START", NULL, NULL); 
    if (window_ == NULL) {
        std::cout << "ENGINE ERROR: GLFW window failed to create!" << std::endl;
    } 
    glfwMakeContextCurrent(window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "ENGINE ERROR: Failed to intialize GLAD!" << std::endl;
    }   
    
    glViewport(0, 0, window_width_, window_height_);
    
    // Initialize rendering state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x00);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glClearColor(23.0f/255.0f, 19.0f/255.0f, 19.0f/255.0f, 1.0f);
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // set callback functions
    glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, Game::frameBufferSizeCallback);
    glfwSetKeyCallback(window_, Game::keyCallback);
    glfwSetCursorPosCallback(window_, Game:: mouseMoveCallback);

    // Intialize input handlers:
    mouse_handler_ = MouseHandler();
    keyboard_handler_ = KeyboardHandler(); 
    
    // Gather Assets
    PathUtils::init();
    AssetManager::init();

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
    
    /*
     * Generate only other Frame Buffer
     */
    glGenFramebuffers(1, &frame_buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
    
    // create texture buffer
    buffer_texture_ = TextureLoader::loadTextureFromData(
            NULL, 
            window_width_, 
            window_height_,
            {}
            );
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer_texture_.ID_, 0);

    // create render buffer object
    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width_, window_height_);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    postEffect_ = AssetManager::defaultShaders().noPostEffect_;
}


/*
 * Only called once after all objects have been generated
 */
void Game::Loop() {

    if (camera_ == nullptr) { std::cout << "Engine Error: No Camera Set" << std::endl; };

    while(!glfwWindowShouldClose(window_)) {

        // gather user input and update input handlers
        keyboard_handler_.clearStates();
        glfwPollEvents();
        mouse_handler_.update();  

        // check exit
        if (keyboard_handler_.getKeyDown(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window_, true);
        }

        DeltaClock::tick();
        // Update all game objects
        for (GameObject* obj : game_objects_) {
            obj->updateComponents();
        }

        writeToUBOs();
        
        bool useFrameBuffer = true;

        // Draw to frame buffer for cool post processing effects
        if (useFrameBuffer) {
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
        } else {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        // Reset Graphics buffers/masks for clean rendering
        glStencilMask(0xFF);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilMask(0x00);
        glEnable(GL_DEPTH_TEST);

        
        // call render function for each object
        for (GameObject* obj : game_objects_) {
            if(obj->hasComponent<Renderer>()) {
                Renderer* r = obj->getComponent<Renderer>();
                r->render();
            }
        }
        if (hasSkybox_) { drawSkybox();}

        if (useFrameBuffer)
            drawScreenBuffer();

        // call debug function
        if (debugFunction_) {
            debugFunction_(*this);
        }
        
        // Update screen
        glfwSwapBuffers(window_);
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
    CameraData cameraData = camera_->getData();
    glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraData), &cameraData); 
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

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

void Game::applyGlobalUniforms(const Shader& shader) {
    if (hasSkybox_) {
        const Texture& texture = AssetManager::accessAsset<Texture>(skyboxTexture_);
        glBindTextureUnit((int)TextureBinding::Skybox, texture.ID_);
    }
    shader.setFloat("time", glfwGetTime());
}

void Game::setCamera(Camera* camera) {
    camera_ = camera;
    //camera_->setAspectRatio((float)window_width_/(float)window_height_);
}
