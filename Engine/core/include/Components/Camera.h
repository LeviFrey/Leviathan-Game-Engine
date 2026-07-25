#pragma once
#include <glm/glm.hpp>
#include "Components/Component.h"
#include "CameraData.h"
#include <iostream>

class Camera : public Component {
    public:
        Camera(
            float sensitivity = 0.15f,
            float fov = 45.0f,
            float aspect_ratio = 1.0f,
            float zNear = 0.1f,
            float zFar = 100.0f
        ) :
        sensitivity_(sensitivity), 
        fov_(fov), 
        aspect_ratio_(aspect_ratio),
        zNear_(zNear),
        zFar_(zFar)
        { up_ = glm::vec3(0.0f, 1.0f, 0.0f); }

        glm::mat4 getView();
        glm::mat4 getProjection();
        CameraData getData();
        virtual glm::vec3 getFront();
        virtual glm::vec3 getPosition();

        void setSensitivity(float sensitivity) { sensitivity_ = sensitivity; }
        void setAspectRatio(float ratio) { aspect_ratio_ = ratio; }

    protected:
        float fov_;
        float sensitivity_;
        glm::vec3 up_;
        float aspect_ratio_;
        float zNear_;
        float zFar_;
};
