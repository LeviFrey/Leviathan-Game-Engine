# --------------------------
# GLAD (source-based lib)
# --------------------------
add_library(glad STATIC
    glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
)

# -------------------
# GLFW (submodule) 
# -------------------
option(USE_SYSTEM_GLFW "Use system GLFW instead of bundled" ON)

if(USE_SYSTEM_GLFW)
    find_package(glfw3 REQUIRED)
else()
    add_subdirectory(glfw)
endif()

# ------------------
# GLM (header-only)
# ------------------
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/glm
)

# -----------------------
# stb_image (header-only)
# -----------------------
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image
)

# -----------------------------
# stb_image_write (header-only)
# -----------------------------
add_library(stb_image_write INTERFACE)
target_include_directories(stb_image_write INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/stb_image_write
)
