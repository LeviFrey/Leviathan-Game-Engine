#pragma once
#include <glm/glm.hpp>

struct alignas(16) CameraData {
    glm::mat4 view_;
    glm::mat4 projection_;
    glm::vec4 viewPos_;
};
