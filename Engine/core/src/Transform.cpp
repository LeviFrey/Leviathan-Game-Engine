#include "Components/Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.h"

void Transform::translate(glm::vec3 translation) {
    position_ += translation;
    makeDirty();
}

void Transform::rotate(float radians, glm::vec3 axis) {
    glm::quat new_rotation = glm::angleAxis(radians, glm::normalize(axis));
    rotation_ = new_rotation * rotation_;
    makeDirty();
}

void Transform::rotate(glm::quat rotation) {
    rotation_ = rotation * rotation_;
    makeDirty();
}

void Transform::setRotation(glm::quat rotation) {
    rotation_ = rotation;
    makeDirty();
}

void Transform::scale(glm::vec3 factor) {
    scale_ *= factor;
    makeDirty();
}

void Transform::scale(float factor) {
    scale_ *= factor;
    makeDirty();
}

glm::vec3 Transform::getLocalPosition() {
    return position_;
}

glm::vec3 Transform::getWorldPosition() {
    return glm::vec3(getWorldMatrix()[3]);
}

glm::quat Transform::getLocalRotation() {
    return rotation_;
}

glm::quat Transform::getWorldRotation() {
    GameObject* parent = getGameObject()->getParent();
    if (parent && parent->hasComponent<Transform>()) {
        return parent->getComponent<Transform>()->getWorldRotation() * rotation_;
    }
    return rotation_;
}

glm::vec3 Transform::getScale() {
    return scale_;
}

glm::mat4 Transform::getWorldMatrix() {
    if (dirty_) {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position_);
        glm::mat4 rotationMatrix = glm::toMat4(rotation_);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale_);
        local_matrix_ = translationMatrix * rotationMatrix * scaleMatrix;
        GameObject* parent = getGameObject()->getParent();
        if (parent && parent->hasComponent<Transform>()) {
            world_matrix_ = parent->getComponent<Transform>()->getWorldMatrix() * local_matrix_;
        } else {
            world_matrix_ = local_matrix_;
        }
        dirty_ = false;
    }
    return world_matrix_;
}

void Transform::makeDirty() {
    dirty_ = true;
    for (auto* child : getGameObject()->getChildren()) {
        if (child->hasComponent<Transform>()) {
            child->getComponent<Transform>()->makeDirty();
        }
    }
}
