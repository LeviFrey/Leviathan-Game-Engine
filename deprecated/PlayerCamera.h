#pragma once
#include "GameObject.h"
#include "Components/Transform.h"
#include "Components/Camera.h"

class PlayerCamera : public GameObject {
    public:
        explicit PlayerCamera() {
            addComponent<Transform>();
            addComponent<Camera>();
        }
};

