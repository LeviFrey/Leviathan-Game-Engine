#include "Tools/CubemapConverter.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "PathUtils.h"
#include "Shapes.h"
#include <iostream>

void CubemapConverter::init() {
    int size = 1024;
    glGenFramebuffers(1, &captureFBO_);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO_);

    glGenRenderbuffers(1, &captureRBO_);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO_);
    
    std::filesystem::path vertex = PathUtils::shaderDir / "equirectToCube/equirectToCube.vert";
    std::filesystem::path fragment = PathUtils::shaderDir / "equirectToCube/equirectToCube.frag";
    shader_ = ShaderLoader::loadShaderFromFile(vertex, fragment);
    
    cube_ = Shapes::createCube(1.0f);
    initialized_ = true;
}

Texture CubemapConverter::equirectangularToCubemap(const std::filesystem::path& path) {

    // Load Equirect Texture into GPU buffers
    Texture equirectTexture = TextureLoader::loadTextureFromFile(path, 
        {
        .wrapT = GL_CLAMP_TO_EDGE
        }
    );
    
    // load empty cubemap to draw to and eventually return
    int size = 1024;
    Texture cubemap = TextureLoader::createEmptyCubemap(size, 
            {
            .format = GL_UNSIGNED_BYTE
            });
    
    // Init helper obejcts if needed
    if (!initialized_) {
        init();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO_);
    
    // describe camera Coordinates for rendering
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

    glm::mat4 projection = glm::perspective(
        glm::radians(90.0f),
        1.0f,
        0.1f,
        10.0f
    );

    glEnable(GL_DEPTH_TEST);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, size, size);

    // Set default shader options
    shader_.use();
    shader_.setMat4("projection", projection);
    glBindVertexArray(cube_.VAO_);
    glActiveTexture(GL_TEXTURE0);
    shader_.setInt("equirectTexture", 0);
    glBindTexture(GL_TEXTURE_2D, equirectTexture.ID_);

    for (int i = 0; i < 6; i++) {

        // bind current face
        glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                cubemap.ID_,
                0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set shader variables
        shader_.setMat4("view", views[i]);
        
        // Draw onto cube face
        glDrawElements(GL_TRIANGLES, cube_.indices_.size(), GL_UNSIGNED_INT, 0);
    }

    // reset state
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    glDeleteTextures(1, &equirectTexture.ID_);

    return cubemap;
}
