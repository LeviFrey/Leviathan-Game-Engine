#pragma once
#include <GLFW/glfw3.h>
class DeltaClock {
    public:
        /*
        updates to the current time
        */
        static void tick();
        static float getDeltaTime();
    private:
        static float lastTime_;
        static float deltaTime_;
};
