#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <iostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "Engine.h"


/*
 * Usage: Converts equirectangular JPG/PNG from source file to 6 cubemap faces placed into dest folder
 * arg 0: source file
 * arg 1: destination folder
 * arg 2: cubemap face size
 */
int main(int argc, char* args[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << args[0]
                  << " <source file> <destination folder> <cubemap face size>\n";
        return 1;
    }
    std::cout 
        << "Converting equirectangular image to cubemap faces" << std::endl 
        << "Source File: " << args[1] << std::endl 
        << "Destination Folder: " << args[2] << std::endl
        << "Cubemap face size: " << args[3] << std::endl;

    std::filesystem::path path, destFolder;
    int size;
    GLuint captureFBO, captureRBO;

    path = args[1];
    destFolder = args[2];
    size = std::stoi(args[3]);

    glfwInit();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(
            1, 1,
            "",
            nullptr,
            nullptr);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "FAILED TO LOAD GLAD" << std::endl;
    }
    Texture equirectTexture = TextureLoader::loadTextureFromFile(
        std::filesystem::path(path),
        {
        .wrapT = GL_CLAMP_TO_EDGE
        }
    );

    Texture cubemap = TextureLoader::createEmptyCubemap(
        size,
        {
        .format = GL_UNSIGNED_BYTE
        }
    );
    
    // Create Frame Buffer
    glGenFramebuffers(1, &captureFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    
    glGenRenderbuffers(1, &captureRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
    
    // Load Shader
    PathUtils::init();
    std::filesystem::path vertex = PathUtils::shaderDir / "equirectToCube/equirectToCube.vert";
    std::filesystem::path fragment = PathUtils::shaderDir / "equirectToCube/equirectToCube.frag";
    Shader shader = ShaderLoader::loadShaderFromFile(vertex, fragment);
    
    Mesh mesh = Shapes::createCube(1.0f);

    glm::vec3 center = glm::vec3(0.0f);
    glm::mat4 views[6] = 
    {
        glm::lookAt(center, glm::vec3(1,0,0),  glm::vec3(0,-1,0)),
        glm::lookAt(center, glm::vec3(-1,0,0), glm::vec3(0,-1,0)),
        glm::lookAt(center, glm::vec3(0,1,0),  glm::vec3(0,0,1)),
        glm::lookAt(center, glm::vec3(0,-1,0), glm::vec3(0,0,-1)),
        glm::lookAt(center, glm::vec3(0,0,1),  glm::vec3(0,-1,0)),
        glm::lookAt(center, glm::vec3(0,0,-1), glm::vec3(0,-1,0))
    };

    std::string names[6] =
    {
        "right.jpg", "left.jpg",
        "top.jpg", "bottom.jpg",
        "front.jpg", "back.jpg"
    };

    glm::mat4 projection = glm::perspective(
        glm::radians(90.0f),
        1.0f,
        0.1f,
        10.0f
    );

    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,size,size);
    shader.use();
    shader.setMat4("projection", projection);
    glBindVertexArray(mesh.VAO_);
    glActiveTexture(GL_TEXTURE0);
    shader.setInt("equirectTexture", 0);
    glBindTexture(GL_TEXTURE_2D, equirectTexture.ID_);

    for (int i = 0; i < 6; i++) {
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            cubemap.ID_,
            0
        );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.setMat4("view", views[i]);
        glDrawElements(GL_TRIANGLES, mesh.indices_.size(), GL_UNSIGNED_INT, 0);
        
        std::vector<unsigned char> pixels(size * size * 3);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glReadPixels(0, 0,
            size, size,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            pixels.data()
        );

        std::filesystem::path out_path = destFolder / names[i];
        stbi_write_png(
            out_path.c_str(),
            size,
            size,
            3,
            pixels.data(),
            size*3
        );
        std::cout << "Cubeface: " << out_path.c_str() << " write success" << std::endl;
    }

    return 0; 
}
