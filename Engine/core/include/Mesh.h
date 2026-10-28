#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "GPUResource.h"

struct Vertex {
    glm::vec3 position_;
    glm::vec3 normal_;
    glm::vec2 texCoords_;
};

class VertexBuffer : public GPUResource {
    public:
        VertexBuffer(std::vector<Vertex> vertices) : vertices_(std::move(vertices)) {}
    private:
        unsigned int loadResource() override;
        std::vector<Vertex> vertices_;
};

class ElementBuffer : public GPUResource {
    public:
        ElementBuffer(std::vector<unsigned int> indices) : indices_(std::move(indices)) {}
        unsigned int getNumIndices() const { return indices_.size(); }
    private:
        unsigned int loadResource() override;
        std::vector<unsigned int> indices_;
};

class Mesh {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) :
            VBO_(std::move(vertices)), EBO_(std::move(indices)) {}
        
        VertexBuffer& getVertexBuffer() { return VBO_; }
        ElementBuffer& getElementBuffer() { return EBO_; }

    private:
        VertexBuffer VBO_;
        ElementBuffer EBO_;
};
