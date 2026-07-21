#pragma once
#include "Camera.h"

class FPSCamera : public Camera {
    public:
        FPSCamera(
            float speed = 5.0f,
            float sensitivity = 0.15f,
            float fov = 45.0f,
            float aspect_ratio = 1.0f,
            float zNear = 0.1f,
            float zFar = 100.0f
        )
        : Camera(sensitivity, fov, aspect_ratio, zNear, zFar) {}
        void update() override;
};
