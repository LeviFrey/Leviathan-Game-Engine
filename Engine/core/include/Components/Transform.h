#ifndef TRANSFORM_H
#define TRANSFORM_H
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Component.h"

/*
 * Component that stores position, rotation, scale of a game objects
 * Interacts with children/parent objects, propogating matrix to children and receiving matrix from parents
 */
class Transform : public Component { 
    public:

        void translate(glm::vec3 translation);

        //rotations
        void rotate(float radians, glm::vec3 axis);
        void rotate(glm::quat rotation);
        void setRotation(glm::quat rotation);

        void scale(glm::vec3 factor);
        void scale(float factor);

        
        glm::vec3 getLocalPosition();
        glm::vec3 getWorldPosition();
        glm::quat getLocalRotation();
        glm::quat getWorldRotation();
        glm::vec3 getScale();

        glm::mat4 getWorldMatrix();

    private:
        void makeDirty();
        glm::vec3 position_{0.0f};
        glm::quat rotation_{1, 0, 0, 0};
        glm::vec3 scale_{1.0f, 1.0f, 1.0f};

        glm::mat4 local_matrix_{1.0f};
        glm::mat4 world_matrix_{glm::mat4(1.0f)};

        bool dirty_{true};
};

#endif

