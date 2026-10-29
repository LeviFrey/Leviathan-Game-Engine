#pragma <once>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Creates an OpenGL window context
class Window {
    public:
        Window(unsigned int width, unsigned int height, const std::string& name = "");
        GLFWwindow* getGLFWPtr();
    private:
        GLFWwindow* window_ptr_;
        unsigned int width_; 
        unsigned int height_;
};
