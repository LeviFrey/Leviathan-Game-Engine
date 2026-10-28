#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <glm/glm.hpp>
#include "Components/LightSource.h"
#include "Shader.h"
#include "Components/Transform.h"

class PointLight : public LightSource {
    public:
        PointLight(float constant, float linear, float quadratic,
            glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
            : LightSource (ambient, diffuse, specular) 
        {
            constant_ = constant;
            linear_ = linear;
            quadratic_ = quadratic;
        }

        PointLight(float constant, float linear, float quadratic)
            : LightSource() 
        {
            constant_ = constant;
            linear_ = linear;
            quadratic_ = quadratic;
        }

        PointLight() : LightSource() {
            constant_ = 1.0f;
            linear_ = 0.1f;
            quadratic_ = 0.03f;
        }

        LightData packLightData() const override {
            LightData data;
            data.ambient_ = glm::vec4(getAmbient(), 1.0f);
            data.diffuse_ = glm::vec4(getDiffuse(), 1.0f);
            data.specular_ = glm::vec4(getSpecular(), 1.0f);
            data.attenuation_ = glm::vec4(constant_, linear_, quadratic_, 1.0f); 
            
            Transform* t = getSiblingComponent<Transform>();
            if (t) {
                data.position_type_ = glm::vec4(t->getWorldPosition(), 1.0f);
            }
            data.position_type_[3] = LightType::POINT;
            return data;

        }

        void setConstant(float constant) { constant_ = constant; }
        void setLinear(float linear) { linear_ = linear; }
        void setQuadratic(float quadratic) { quadratic_ = quadratic; }
        float getConstant() { return constant_; }
        float getLinear() { return linear_; }
        float getQuadratic() { return quadratic_; }

    private:
        float constant_;
        float linear_;
        float quadratic_;
};

#endif
