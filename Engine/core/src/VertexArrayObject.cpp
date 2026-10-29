#include "Assets/VertexArrayObject.h"
#include "Assets/Mesh.h"
#include <stdexcept>
#include <iostream>

VertexArrayObject::VertexArrayObject() {
    setBinding(0, {0, sizeof(Vertex)});

    setAttribute(0, {0, 3, offsetof(Vertex, position_)});
    setAttribute(1, {0, 3, offsetof(Vertex, normal_)});
    setAttribute(2, {0, 2, offsetof(Vertex, texCoords_)});
}



void VertexArrayObject::bindVertexBuffer(unsigned int binding_location, VertexBuffer& VBO) {

    auto it = bindings_.find(binding_location);

    if (it == bindings_.end()) {
        throw std::invalid_argument(
            "Could not bind VBO to VAO"
        );
    }

    glVertexArrayVertexBuffer(
        getResource(), 
        binding_location, 
        VBO.getResource(), 
        0, 
        it->second.stride
    );
}


void VertexArrayObject::bindElementBuffer(ElementBuffer& EBO) {
    glVertexArrayElementBuffer(getResource(), EBO.getResource());
}


void VertexArrayObject::setBinding(
    unsigned int binding_location,
    Binding binding_config
) {

    auto [it, inserted] = bindings_.insert_or_assign(binding_location, binding_config);

    makeDirty();
}



void VertexArrayObject::setAttribute(
    unsigned int attribute_location,
    Attribute attribute_config
) {
    
    // A missing binding isn't really a problem and has no behavior.
    // But we alert anyway, in case of unintentional configuration.
    auto binding_it = bindings_.find(attribute_config.binding_index);

    if (binding_it == bindings_.end()) {
        std::cerr
            << "POSSIBLE_ERROR::VERTEX_ARRAY_OBJECT: "
            << "Binding attribute: " << attribute_location
            << ", at binding location: " << attribute_config.binding_index
            << ", does not currently exist." << std::endl;
    }

    auto [it, inserted] = attributes_.insert_or_assign(attribute_location, attribute_config);

    makeDirty();
}



unsigned int VertexArrayObject::loadResource() {

    // init GPU resource
    unsigned int VAO;
    glCreateVertexArrays(1, &VAO);
    
    // set divisor configs
    for (const auto& [binding_location, binding] : bindings_) {

        glVertexArrayBindingDivisor(VAO, binding_location, binding.divisor);
    }

    for (const auto& [attribute_location, attribute] : attributes_) {

        // describe each attribute format in the binding
        glVertexArrayAttribFormat(
            VAO, 
            attribute_location, 
            static_cast<GLuint>(attribute.dimension),
            GL_FLOAT,
            GL_FALSE,
            static_cast<GLuint>(attribute.offset)
        );
            
        // register the attribute location
        glVertexArrayAttribBinding(
            VAO,
            attribute_location,
            attribute.binding_index
        );
        
        // enable attribute
        glEnableVertexArrayAttrib(
            VAO,
            attribute_location
        );

    }
    return VAO;
}

void VertexArrayObject::print() {
    std::cout << "VAO Resource: " << getResource() << std::endl;
    for (auto& [b_loc, b] : bindings_) {
        std::cout
            << "Bind Location: " << b_loc << ", "
            << "Divisor: " << b.divisor << ", "
            << "Stride: " << b.stride << std::endl;
    }

    for (auto& [a_loc, a] : attributes_) {
        std::cout
            << "Attribute Location: " << a_loc << ", "
            << "Index: " << a.binding_index << ", "
            << "Dimension: " << a.dimension << ", "
            << "Offset: " << a.offset << std::endl;
    }
}
