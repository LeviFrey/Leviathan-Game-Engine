#include "Assets/Mesh.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

unsigned int VertexBuffer::loadResource() {
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, 
        vertices_.size() * sizeof(Vertex), 
        vertices_.data(), 
        GL_STATIC_DRAW
    );
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return VBO;
}

unsigned int ElementBuffer::loadResource() {
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices_.size() * sizeof(unsigned int),
        indices_.data(),
        GL_STATIC_DRAW
    );
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return EBO;
}
