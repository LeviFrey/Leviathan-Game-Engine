#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <filesystem>

#include "GPUResource.h"

struct ShaderProgram {
    std::filesystem::path path;
    std::string code;
    GLenum type;
};

class Shader : public GPUResource {
    public:

        Shader(
            const std::filesystem::path& vertex_path,
            const std::filesystem::path& fragment_path,
            const std::filesystem::path& geometry_path = {}
        );

        void use() { glUseProgram(getResource()); }
        
        void setBool(const std::string &name, bool value);
        void setInt(const std::string &name, int value);
        void setFloat(const std::string &name, float value);
        void setMat4(const std::string &name, glm::mat4 value);
        void setVec3(const std::string &name, glm::vec3 value);
        void setVec4(const std::string &name, glm::vec4 value);

    private:
        unsigned int loadResource() override;

        std::string readFile(const std::filesystem::path& path);
        unsigned int compileProgram(const ShaderProgram& program);

        
        ShaderProgram vertex_;
        ShaderProgram fragment_;
        ShaderProgram geometry_;
        
        unsigned int gpu_resource_ = 0;
};
