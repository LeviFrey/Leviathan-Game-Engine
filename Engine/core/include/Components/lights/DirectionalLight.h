#ifndef DIRECTIONAL_LIGHT_SOURCE_H
#define DIRECTIONAL_LIGHT_SOURCE_H

#include "Components/AllComponents.h"
#include "Components/LightSource.h"

class DirectionalLight : public LightSource {
    public: 
        DirectionalLight(
                glm::vec3 direction,
                glm::vec3 ambient, 
                glm::vec3 diffuse, 
                glm::vec3 specular) 
            : LightSource(ambient, diffuse, specular) {
            direction_ = direction;
        }

        DirectionalLight() : LightSource() {
            direction_ = glm::vec3(0.0f, -1.0f, 0.0f);
        }

        DirectionalLight(glm::vec3 direction) : LightSource() {
            direction_ = direction;
        }
        /*
        void applyUniforms(Shader* shader) override {
            LightSource::applyUniforms(shader);
            shader->setVec3("light.direction", direction_);
        }
        */

        LightData packLightData() const override {
            LightData data;
            data.ambient_ = glm::vec4(getAmbient(), 1.0f);
            data.diffuse_ = glm::vec4(getDiffuse(), 1.0f);
            data.specular_ = glm::vec4(getSpecular(), 1.0f);
            data.direction_ = glm::vec4(direction_, 1.0f);

            Transform* t = getGameObject()->getComponent<Transform>();
            if (t) {
                data.position_type_ = glm::vec4(t->getWorldPosition(), 1.0f);
            }
            data.position_type_[3] = LightType::DIRECTIONAL;
            return data; 
        }

    private:
        glm::vec3 direction_;
};

#endif
