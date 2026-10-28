

#include "Shader.h"
#include <filesystem>

class ShaderLoader {
    public:
    static Shader loadShaderFromFile(
            const std::filesystem::path& vertex_path, 
            const std::filesystem::path& fragment_path,
            const std::filesystem::path& geometry_path = {});
};
