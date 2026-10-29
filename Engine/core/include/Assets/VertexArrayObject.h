#pragma once
#include <glad/glad.h>
#include <cstddef>
#include <map>
#include <iostream>
#include "GPUResource.h"
#include "Mesh.h"

struct Attribute {

    // which binding does it belong to
    unsigned int binding_index;

    unsigned int dimension;

    // offset inside struct object sent to GPU. 
    // Can use offsetof(Type, field_name).
    size_t offset;
};

struct Binding {

    // Divisor options for shader iteration.
    // 0: per vertex.
    // 1: per instance.
    // n: per n instances.
    unsigned int divisor = 0;

    // size of each buffered object
    size_t stride;
};

class VertexArrayObject : public GPUResource {
    public:

        VertexArrayObject();

        void setBinding(
            unsigned int binding_location,
            Binding binding_config
        );

        void setAttribute(
            unsigned int attribute_location,
            Attribute config
        );
            
        void bindVertexBuffer(unsigned int binding_location, VertexBuffer& VBO);
        void bindElementBuffer(ElementBuffer& EBO);

        void bind() { 
            glBindVertexArray(getResource()); 
        }

        void print();
    
    private:
        std::map<unsigned int, Attribute> attributes_; 
        std::map<unsigned int, Binding> bindings_;
        unsigned int loadResource() override;
};
