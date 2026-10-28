#pragma once

#include <glm/glm.hpp>
#include "Components/LightSource.h"
#include "Components/Transform.h"

class AreaLight : public LightSource {
    public:
        AreaLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
            : LightSource(ambient, diffuse, specular) {}

        AreaLight() : LightSource() {}

        LightData packLightData() const override {
            LightData data;
            data.ambient_ = glm::vec4(getAmbient(), 1.0f);
            data.diffuse_ = glm::vec4(getDiffuse(), 1.0f);
            data.specular_ = glm::vec4(getSpecular(), 1.0f);
                
            Transform* t = getSiblingComponent<Transform>();
            data.position_type_ = glm::vec4(t->getWorldPosition(), 1.0f);
            data.position_type_[3] = LightType::AREA;

            return data;
        }

    private:
};
