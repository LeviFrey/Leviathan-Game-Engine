#define GLM_ENABLE_EXPERIMENTAL
#include "Components/Camera.h"
#include "Components/Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

glm::vec3 Camera::getFront() {
    Transform* t = getSiblingComponents()->get<Transform>();
    return t->getWorldRotation() * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Camera::getPosition() {
    Transform* t = getSiblingComponents()->get<Transform>();
    return t->getWorldPosition();
}

glm::mat4 Camera::getView() {
    return glm::lookAt(getPosition(), getPosition() + getFront(), up_);
}

glm::mat4 Camera::getProjection() {
    return glm::perspective(glm::radians(fov_), aspect_ratio_, zNear_, zFar_);
}

CameraData Camera::getData() {
    glm::mat4 view = getView();
    glm::mat4 proj = getProjection();
    glm::vec3 pos = getPosition();
    return {view, proj, glm::vec4(pos, 1.0f)};
}
