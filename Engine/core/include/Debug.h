#pragma once

#include <iostream>

#define DEBUG

#ifdef DEBUG
    #define DEBUG_PRINT(message) \
        do { std::cerr << "[DEBUG] " << message << std::endl; } while (false)
#else
    #define DEBUG_PRINT(message) do {} while (false)
#endif

#define GL_CHECK() do { \
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) \
        std::cerr << "OpenGL error " << err \
                  << " at line " << __LINE__ << '\n'; \
} while (0)
