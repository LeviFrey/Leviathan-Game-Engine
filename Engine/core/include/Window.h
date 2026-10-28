#pragma <once>
#include <string>
#include <GLFW/glfw3.h>

class Window {
    public:
        Window(unsigned int width, unsigned int height, const std::string& name = "");
    private:
        GLFWwindow* window_ptr_;
        unsigned int width_; 
        unsigned int height_;
};
