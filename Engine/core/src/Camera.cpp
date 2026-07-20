#define GLM_ENABLE_EXPERIMENTAL
#include "Components/Camera.h"
#include "Input/KeyboardHandler.h"
#include "Input/MouseHandler.h"
#include "DeltaClock.h"
#include "Game.h"
#include "Components/Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

glm::vec3 Camera::getFront() {
    return getGameObject()->getComponent<Transform>()->getWorldRotation() * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Camera::getPosition() {
    return getGameObject()->getComponent<Transform>()->getWorldPosition();
}

glm::mat4 Camera::getView() {
    return glm::lookAt(getPosition(), getPosition() + getFront(), up_);
}

glm::mat4 Camera::getProjection() {
    return glm::perspective(glm::radians(fov_), aspect_ratio_, zNear_, zFar_);
}

CameraData Camera::getData() {
    return {getView(), getProjection(), glm::vec4(getPosition(), 1.0f)};
}
