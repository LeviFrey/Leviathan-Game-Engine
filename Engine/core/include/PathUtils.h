#pragma once
#include <filesystem>
#include <unistd.h>
#include <limits.h>

namespace PathUtils {

inline std::filesystem::path getExecutableDir() {
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) -1);
    if (len == -1) return {};
    buffer[len] = '\0';
    return std::filesystem::path(buffer).parent_path();
}


inline std::filesystem::path executableDir = getExecutableDir();
inline std::filesystem::path projectDir = executableDir.parent_path();
inline std::filesystem::path assetDir   = projectDir / "Engine/resources";
inline std::filesystem::path shaderDir  = assetDir / "shaders";
inline std::filesystem::path objectDir  = assetDir / "objects";
inline std::filesystem::path textureDir = assetDir / "textures";

};


