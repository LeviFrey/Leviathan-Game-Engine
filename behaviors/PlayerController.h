#include "Engine.h"
class PlayerController : public Component {
    public:
        float move_speed_;
        float look_speed_;
        float yaw_ = 0;
        float pitch_ = 0;
        glm::vec3 look_direction_{1.0f, 0.0f, 0.0f};
        PlayerController(float move_speed = 3.0f, float look_speed = 10.0f) 
        : move_speed_(move_speed), look_speed_(look_speed) {}
        void update() {
            KeyboardHandler* kh = getGameObject()->getGame()->getKeyboardHandler();
            MouseHandler* mh = getGameObject()->getGame()->getMouseHandler();
            Transform* Pt = getGameObject()->getParent()->getComponent<Transform>();
            Transform* Lt = getGameObject()->getComponent<Transform>();
            
            yaw_ += DeltaClock::getDeltaTime() * mh->getMouseXOffset() * look_speed_;
            pitch_ -= DeltaClock::getDeltaTime() * mh->getMouseYOffset() * look_speed_;
            if (pitch_ > 89.0f) { pitch_ = 89.0f; }
            if (pitch_ < -89.0f) { pitch_ = -89.0f; }
            glm::quat rotation =
                glm::angleAxis(glm::radians(yaw_), glm::vec3(0.0f, -1.0f, 0.0f)) *
                glm::angleAxis(glm::radians(pitch_), glm::vec3(0.0f, 0.0f, 1.0f));
            look_direction_ = rotation * glm::vec3(1.0f, 0.0f, 0.0);
            Lt->setRotation(rotation);
            glm::vec3 walk_direction = 
                glm::normalize(glm::vec3(look_direction_.x, 0.0f, look_direction_.z));
            if (kh->getKeyPressed(GLFW_KEY_W)) {
                Pt->translate(DeltaClock::getDeltaTime() * move_speed_ * walk_direction);
            }
            if (kh->getKeyPressed(GLFW_KEY_S)) {
                Pt->translate(-DeltaClock::getDeltaTime() * move_speed_ * walk_direction);
            }
            if (kh->getKeyPressed(GLFW_KEY_D)) {
                Pt->translate(DeltaClock::getDeltaTime() * glm::normalize(glm::cross(walk_direction, glm::vec3(0.0f, 1.0f, 0.0f))) * move_speed_);
            }
            if(kh->getKeyPressed(GLFW_KEY_A)) {
                Pt->translate(-DeltaClock::getDeltaTime() * glm::normalize(glm::cross(walk_direction, glm::vec3(0.0f, 1.0f, 0.0f))) * move_speed_);
            }
        }      
};

