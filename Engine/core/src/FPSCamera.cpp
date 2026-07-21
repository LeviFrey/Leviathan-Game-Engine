#include "Components/FPSCamera.h"
#include "Input/KeyboardHandler.h"
#include "Input/MouseHandler.h"
#include "Components/Transform.h"
#include "Game.h"

void FPSCamera::update() {
    KeyboardHandler* kh = getGameObject()->getGame()->getKeyboardHandler();
    MouseHandler* mh = getGameObject()->getGame()->getMouseHandler();
    Transform* t = getGameObject()->getComponent<Transform>();
    
    yaw_ += mh->getMouseXOffset() * sensitivity_;
    pitch_ -= mh->getMouseYOffset() * sensitivity_;
    
    if (pitch_ > 89.0f) { pitch_ = 89.0f; }
    if (pitch_ < -89.0f) { pitch_ = -89.0f; }

    glm::quat rotation = 
        glm::angleAxis(glm::radians(yaw_), glm::vec3(0.0f, -1.0f, 0.0f)) *
        glm::angleAxis(glm::radians(pitch_), glm::vec3(0.0f, 0.0f, 1.0f));
    t->setRotation(rotation);
}
