#include "Input/MouseHandler.h"

void MouseHandler::handleMouseMovement(GLFWwindow* window, double xpos, double ypos) {
    lastXPos_ = xpos_;
    lastYPos_ = ypos_;
    xpos_ = xpos;
    ypos_ = ypos;
}

void MouseHandler::handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
    lastScrollOffset_ = yoffset;
}

void MouseHandler::update() {
    mouseXOffset_ = xpos_ - lastXPos_;
    mouseYOffset_ = ypos_ - lastYPos_;
    lastXPos_ = xpos_;
    lastYPos_ = ypos_;

    currentScrollOffset_ = lastScrollOffset_;
    lastScrollOffset_ = 0;
}

float MouseHandler::getMouseXOffset() { return mouseXOffset_; }
float MouseHandler::getMouseYOffset() { return mouseYOffset_; } 
