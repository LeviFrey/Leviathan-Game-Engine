#include "Shader.h"
#include <fstream>
#include <iostream>

Shader::Shader(
    const std::filesystem::path& vertex_path,
    const std::filesystem::path& fragment_path,
    const std::filesystem::path& geometry_path
) {

    vertex_ = {
        vertex_path, 
        readFile(vertex_path), 
        GL_VERTEX_SHADER
    };

    fragment_ = {
        fragment_path, 
        readFile(fragment_path), 
        GL_FRAGMENT_SHADER
    };

    if (!geometry_path.empty()) {

        geometry_ = {
            geometry_path, 
            readFile(geometry_path), 
            GL_GEOMETRY_SHADER
        };

    } else {

        geometry_ = {
            {}, 
            {}, 
            GL_GEOMETRY_SHADER
        };

    }
}

std::string Shader::readFile(const std::filesystem::path& path) {
    std::ifstream file;
    std::stringstream stream;
    file.open(path.string());
    if (!file.is_open()) {
        throw std::runtime_error("ERROR::SHADER, failed to open file " + path.string());
    }
    stream << file.rdbuf();
    file.close();
    return stream.str();
}


unsigned int Shader::compileProgram(const ShaderProgram& program) {
    if (program.code.empty()) { return 0; }

    unsigned int shader_id;
    int success;
    char info_log[512];
    shader_id = glCreateShader(program.type);
    const char* code_char = program.code.c_str();
    glShaderSource(shader_id, 1, &code_char, NULL);
    glCompileShader(shader_id);

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED (" 
                  << program.path.string() << ")" << std::endl;
        std::cerr << info_log << std::endl;
    }
    
    return shader_id;
}

unsigned int Shader::loadResource() {
    unsigned int shader_id = glCreateProgram();
    
    unsigned int vertex_id = compileProgram(vertex_);
    glAttachShader(shader_id, vertex_id);

    unsigned int geometry_id = compileProgram(geometry_);
    if (geometry_id) {
        glAttachShader(shader_id, geometry_id);
    }

    unsigned int fragment_id = compileProgram(fragment_);
    glAttachShader(shader_id, fragment_id);

    glLinkProgram(shader_id);
    int success;
    char info_log[512];
    glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_id, 512, NULL, info_log);
        std::cout << "Error::SHADER::PROGRAM::LINKING_FAILURE" << std::endl;
        std::cout << info_log << std::endl;
    }

    glDeleteShader(vertex_id);
    glDeleteShader(geometry_id);
    glDeleteShader(fragment_id);

    return shader_id;
}


void Shader::setBool(const std::string &name, bool value){
    int location = glGetUniformLocation(getResource(), name.c_str());
    glUniform1i(location, (int)value);
}

void Shader::setInt(const std::string &name, int value){
    int location = glGetUniformLocation(getResource(), name.c_str());
    glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, float value){
    int location = glGetUniformLocation(getResource(), name.c_str());
    glUniform1f(location, value);
}

void Shader::setVec3(const std::string &name, glm::vec3 value){
    int location = glGetUniformLocation(getResource(), name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string &name, glm::mat4 value) {
    int location = glGetUniformLocation(getResource(), name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec4(const std::string &name, glm::vec4 value) {
    int location = glGetUniformLocation(getResource(), name.c_str());
    glUniform4fv(location, 1, glm::value_ptr(value));
}
