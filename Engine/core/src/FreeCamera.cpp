#include "Components/FreeCamera.h"
#include "Input/KeyboardHandler.h"
#include "Input/MouseHandler.h"
#include "Components/Transform.h"
#include "Game.h"
#include "DeltaClock.h"

void FreeCamera::update() {

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
    if (kh->getKeyPressed(GLFW_KEY_W)) {
        t->translate(DeltaClock::getDeltaTime() * speed_ * getFront());
    }
    if (kh->getKeyPressed(GLFW_KEY_S)) {
        t->translate(-DeltaClock::getDeltaTime() * speed_ * getFront());
    }
    if (kh->getKeyPressed(GLFW_KEY_A)) {
        t->translate(-DeltaClock::getDeltaTime() * glm::normalize(glm::cross(getFront(), up_)) * speed_);
    }
    if(kh->getKeyPressed(GLFW_KEY_D)) {
        t->translate(DeltaClock::getDeltaTime() * glm::normalize(glm::cross(getFront(), up_)) * speed_);
    }
}
    
    

