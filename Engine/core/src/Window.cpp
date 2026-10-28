#include "Window.h"
#include <iostream>

Window::Window(unsigned int width, unsigned int height, const std::string& name) 
: width_(width), height_(height)
{
    /*
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
    glfwMakeContextCurrent(window_);

    of (!gladLoadGLLoader((GLADloadProc)glfwGetProcAddress)) {
        */
}
