#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <glm/glm.hpp>
#include "Components/LightSource.h"
#include "Components/Transform.h"
#include "GameObject.h"
#include <cmath>
#include <glm/gtc/constants.hpp>

class SpotLight : public LightSource {
    public:
        SpotLight(glm::vec3 direction, float innerCutoff, float outerCutoff,
            glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
            : LightSource (ambient, diffuse, specular) 
        {
            direction_ = direction;
            innerCutoff_ = innerCutoff;
            outerCutoff_ = outerCutoff;
        }


        SpotLight(glm::vec3 direction, float innerCutoff, float outerCutoff)
            : LightSource() 
        {
            direction_ = direction;
            innerCutoff_ = innerCutoff;
            outerCutoff_ = outerCutoff;
        }

        SpotLight() : LightSource() {
            direction_ = glm::vec3(0.0f, -1.0f, 0.0f);
            innerCutoff_ = 10.0f;
            outerCutoff_ = 20.0f;
        }
        
        LightData packLightData() const override {
            LightData data;
            data.ambient_ = glm::vec4(getAmbient(), 1.0f);
            data.diffuse_ = glm::vec4(getDiffuse(), 1.0f);
            data.specular_ = glm::vec4(getSpecular(), 1.0f);
            data.spotlight_ = glm::vec4(
                    std::cos(glm::radians(innerCutoff_)), 
                    std::cos(glm::radians(outerCutoff_)), 
                    1.0f, 1.0f);
            data.direction_ = glm::vec4(direction_, 1.0f);
            
            Transform* t = getSiblingComponent<Transform>();
            data.position_type_ = glm::vec4(t->getWorldPosition(), 1.0f);
            data.position_type_[3]  = LightType::SPOT;

            return data;
        }

        glm::vec3 getDirection() { return direction_; }
        void setDirection(glm::vec3 direction) { direction_ = direction; }
        float getInnerCutoff() { return innerCutoff_; }
        float getOuterCutoff() { return outerCutoff_; }
        void setInnerCutoff(float cutoff) { innerCutoff_ = cutoff; }
        void setOuterCutoff(float cutoff) { outerCutoff_ = cutoff; }
    private:
        glm::vec3 direction_;
        float innerCutoff_;
        float outerCutoff_;
};

#endif
