#include "Window.h"
#include "Debug.h"
#include <iostream>
#include <glad/glad.h>

Window::Window(unsigned int width, unsigned int height, const std::string& name) 
: width_(width), height_(height)
{
    glfwSetErrorCallback([](int error, const char* description) {
            std::cerr 
            << "ENGINE ERROR: with code: " << error 
            << " and desc: " << description 
            << std::endl;
    });

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);

    window_ptr_ = glfwCreateWindow(width_, height_, name.c_str(), NULL, NULL);
    if (window_ptr_ == NULL) {
        std::cout << "ENGINE ERROR: GLFW window failed to create!" << std::endl;
    }
    glfwMakeContextCurrent(window_ptr_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        DEBUG_PRINT("Engine Error: Failed to initialize GLAD!");
    }

    glViewport(0, 0, width_, height_);

    glfwSetInputMode(window_ptr_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


GLFWwindow* Window::getGLFWPtr() { 
    return window_ptr_;
}
