#pragma once
#include "Camera.h"

class FreeCamera : public Camera {
    public:
        FreeCamera(
            float speed = 5.0f,
            float sensitivity = 50.0f,
            float fov = 45.0f,
            float aspect_ratio = 1.0f,
            float zNear = 0.1f,
            float zFar = 100.0f
        ) 
        : Camera(sensitivity, fov, aspect_ratio, zNear, zFar), speed_(speed) {
            yaw_ = 0;
            pitch_ = 0;
        }

        void update() override;
        float getYaw() { return yaw_; }
        float getPitch() { return pitch_; }

    private:
        float speed_;
        float yaw_;
        float pitch_;
};

