#include "RenderDataConfig.h"


void RenderDataConfig::init() {
    glCreateVertexArrays(1, &VAO_);
    GLuint base_binding = 0;

    addAttribute(3, offsetof(Vertex, position_), base_binding);
    addAttribute(3, offsetof(Vertex, normal_), base_binding);
    addAttribute(2, offsetof(Vertex, texCoords_), base_binding);

    // iterate once per vertex
    setBindingDivisor(base_binding, 0);
    /*
    glVertexArrayAttribFormat(VAO_, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position_));
    glVertexArrayAttribBinding(VAO_, 0, 0);

    glVertexArrayAttribFormat(VAO_, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal_));
    glVertexArrayAttribBinding(VAO_, 1, 0);

    glVertexArrayAttribFormat(VAO_, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoords_));
    glVertexArrayAttribBinding(VAO_, 2, 0);

    glEnableVertexArrayAttrib(VAO_, 0);
    glEnableVertexArrayAttrib(VAO_, 1);
    glEnableVertexArrayAttrib(VAO_, 2);

    location_index_ = 3;
    */
}


void RenderDataConfig::addAttribute(int dim, size_t offset, GLuint binding) {
    if (location_index_ == 0) { init(); }
    glVertexArrayAttribFormat(VAO_, location_index_, dim, GL_FLOAT, GL_FALSE, offset);
    glVertexArrayAttribBinding(VAO_, location_index_, binding);
    glEnableVertexArrayAttrib(VAO_, location_index_);
    location_index_++;
}

void RenderDataConfig::setBindingDivisor(GLuint binding, GLuint divisor) {
    glVertexArrayBindingDivisor(VAO_, binding, divisor);
}
