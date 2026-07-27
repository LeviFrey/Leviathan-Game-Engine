#pragma once
#include <glm/glm.hpp>
#include <vector>

using namespace std;
struct Vertex {
    glm::vec3 position_;
    glm::vec3 normal_;
    glm::vec2 texCoords_;
};

class Mesh {
    public:
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
        Mesh() = default;
        void setUpMesh();

        unsigned int VBO_;
        //unsigned int VAO_;
        unsigned int EBO_;
        vector<Vertex> vertices_;
        vector<unsigned int> indices_;
};
