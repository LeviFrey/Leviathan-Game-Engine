#include "Engine.h"

class CameraMount : public GameObject {
    public:
        CameraMount() {
            addComponent<Camera>();
            addComponent<Transform>();
        }
};


class PlayerController : public Component {
    public:
void printVec(glm::vec3 v) {
    std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}

        PlayerController(
            CameraMount* head, 
            InputHandler* input,
            float walk_speed = 5.0f,
            float look_speed = 20.0f
        ) : 
        head_(head), input_(input), look_speed_(look_speed), walk_speed_(walk_speed)
        {}
        
        // From Component
        void update() override {
            MouseHandler& mh = input_->mouse_handler;
            KeyboardHandler& kh = input_->keyboard_handler;
            
            // Handle head rotation
            Transform* head_transform = head_->getComponents()->get<Transform>();
            float delta_yaw = mh.getMouseXOffset() * look_speed_ * (float)DeltaClock::getDeltaTime();
            float delta_pitch = -mh.getMouseYOffset() * look_speed_ * (float)DeltaClock::getDeltaTime();

            
            float new_pitch = pitch + delta_pitch;
            if (new_pitch > 80.0f) {
                delta_pitch = 80.0f - pitch;
                pitch = 80.0f;
            } else if (new_pitch < -80.0f) {
                delta_pitch = -80.0F - pitch;
                pitch = -80.0f;
            } else {
                pitch = pitch + delta_pitch;
            }

            yaw += delta_yaw;
            std::cout << "pitch: " << pitch << std::endl;
            std::cout << "yaw: " << yaw << std::endl;

            glm::quat yaw_rotation = 
                glm::angleAxis(glm::radians(delta_yaw), glm::vec3(0.0f, -1.0f, 0.0f)
            );

            glm::quat pitch_rotation =
                glm::angleAxis(glm::radians(delta_pitch), head_transform->getLocalRotation() * glm::vec3(0.0f, 0.0f, 1.0f)
            );

            glm::quat rotation = yaw_rotation * pitch_rotation;
            head_transform->rotate(rotation);
            
            rotation = head_transform->getLocalRotation();
            printVec(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
            printVec(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
            printVec(rotation * glm::vec3(0.0f, 0.0f, 1.0f));


            Transform* transform = getSiblingComponents()->get<Transform>();

            glm::vec3 look_direction = 
                head_transform->getLocalRotation() * glm::vec3(1.0f, 0.0f, 0.0f);


            glm::vec3 walk_direction = glm::normalize(
                glm::vec3(look_direction.x, 0.0f, look_direction.z)
            );

            glm::vec3 walk_vector{0};

            if (kh.getKeyPressed(GLFW_KEY_W)) {
                walk_vector = walk_direction;
            }

            if (kh.getKeyPressed(GLFW_KEY_S)) {
                walk_vector = -1.0f * walk_direction; 
            }

            if (kh.getKeyPressed(GLFW_KEY_D)) {
                glm::vec3 up{0.0f, 1.0f, 0.0f};
                walk_vector = glm::normalize(
                    glm::cross(walk_direction, up)
                );
            }
            
            if (kh.getKeyPressed(GLFW_KEY_A)) {
                glm::vec3 up{0.0f, 1.0f, 0.0f};
                walk_vector = -1.0f * glm::normalize(
                    glm::cross(walk_direction, up)
                );
            }

            transform->translate(walk_speed_ * DeltaClock::getDeltaTime() * walk_vector);
        }
        
        float yaw = 0;
        float pitch = 0;
        float walk_speed_;
        float look_speed_;

    private:
        CameraMount* head_;
        InputHandler* input_;
         
};

class Player : public GameObject {
    public:
        Player(InputHandler* input) {
            camera_mount = addChild<CameraMount>();
            camera_mount->addComponent<Camera>();
            addComponent<PlayerController>(camera_mount, input);
            addComponent<Transform>();
        }

        Camera* getCamera() {
            return camera_mount->getComponents()->get<Camera>();
        }

        CameraMount * camera_mount;
};
