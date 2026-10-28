#include "ShaderLoader.h"
#include <iostream>
#include <fstream>

unsigned int compileShader(const std::filesystem::path& path, GLenum type) {
    std::ifstream file;
    std::stringstream stream;
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    file.open(path.string());
    stream << file.rdbuf();
    file.close();
    std::string codeString = stream.str();
    const char* code = codeString.c_str();

    unsigned int id;
    int success;
    char infoLog[512];
    id = glCreateShader(type);
    glShaderSource(id, 1, &code, NULL);
    glCompileShader(id);

    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout 
            <<"ERROR::SHADER::COMPILATION::COMPILATION_FAILED (" 
            << path.string() 
            << ")" 
            << std::endl;
        std::cerr
            << infoLog
            << std::endl;
    }
    return id;
}

Shader ShaderLoader::loadShaderFromFile(
        const std::filesystem::path& vertex_path, 
        const std::filesystem::path& fragment_path,
        const std::filesystem::path& geometry_path) {
    unsigned int ID = glCreateProgram();
    unsigned int vertexID;
    unsigned int geometryID;
    unsigned int fragmentID;
    vertexID = compileShader(vertex_path, GL_VERTEX_SHADER);
    glAttachShader(ID, vertexID);
    if (!geometry_path.empty()) {
        geometryID = compileShader(geometry_path, GL_GEOMETRY_SHADER);
        glAttachShader(ID, geometryID);
    }
    fragmentID = compileShader(fragment_path, GL_FRAGMENT_SHADER);
    glAttachShader(ID, fragmentID);

    glLinkProgram(ID);
    
    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout
            << "ERROR::SHADER::PROGRAM::LINKING_FAILURE"
            << std::endl
            << infoLog
            << std::endl;
    }

    glDeleteShader(vertexID);
    if (!geometry_path.empty()) {
        glDeleteShader(geometryID);
    }
    glDeleteShader(fragmentID);

    return {ID};
}
