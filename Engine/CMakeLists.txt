cmake_minimum_required(VERSION 3.5)
project(graphics_project VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)


file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/core/src/*.cpp"
)

add_library(Engine STATIC ${ENGINE_SOURCES})

add_subdirectory(external)

target_include_directories(Engine PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/core/include"
)

# external libraries
target_link_libraries(Engine PUBLIC
    glfw
    glad
    glm
    stb_image
    stb_image_write
)

find_package(assimp REQUIRED)
target_link_libraries(Engine PUBLIC assimp::assimp)

# OPENGL
find_package(OpenGL REQUIRED)

#[[
# ----- GLFW -----
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)
#find_package(glfw3 3.3 REQUIRED)

# ----- ASSIMP ----
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "" FORCE)
set(ASSIMP_USE_ZLIB OFF CACHE BOOL "" FORCE)

add_subdirectory(external/assimp)

target_link_libraries(Engine PUBLIC 
    glfw 
    OpenGL::GL
    assimp)

# ----- GLM -----
find_package(glm QUIET)

if(NOT glm_FOUND)
    target_include_directories(Engine PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}/external/glm"
    )
endif()

if (TARGET OpenGL::OpenGL)
    target_link_libraries(Engine PUBLIC OpenGL::OpenGL)
elseif(TARGET OpenGL::GL)
    target_link_libraries(Engine PUBLIC OpenGL::GL)
else()
    target_link_libraries(Engine PUBLIC ${OPENGL_LIBRARIES})
    target_include_directories(Engine PUBLIC ${OPENGL_INCLUDE_DIR})
endif()
]]

