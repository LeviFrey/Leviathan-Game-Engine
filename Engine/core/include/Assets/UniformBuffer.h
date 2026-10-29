#pragma once

#include <glad/glad.h>

#include "GPUResource.h"

template <typename T>
class UniformBuffer : public GPUResource {
    public:
        UniformBuffer(int binding_index) :
            binding_index_(binding_index)
        {}

        void bufferData(T* data) {
            glBindBuffer(GL_UNIFORM_BUFFER, getResource());
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), data);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

    private:
        int binding_index_; 
        unsigned int loadResource() override {
            unsigned int UBO;
            glGenBuffers(1, &UBO);
            glBindBuffer(GL_UNIFORM_BUFFER, UBO);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(T), nullptr, GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, binding_index_, UBO);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            return UBO;
        }
};
