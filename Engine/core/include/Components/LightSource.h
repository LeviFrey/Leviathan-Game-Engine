#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include <glm/glm.hpp>
#include "Component.h"
#include "GPU/LightData.h"

class LightSource : public Component {
    public:
        LightSource(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
            ambient_ = ambient;
            diffuse_ = diffuse;
            specular_ = specular;
        }

        LightSource() {
            ambient_ = glm::vec3(0.3f, 0.3f, 0.3f);
            diffuse_ = glm::vec3(0.5f, 0.5f, 0.5f);
            specular_ = glm::vec3(1.0f, 1.0f, 1.0f);
        }

        virtual LightData packLightData() const = 0;
        
        glm::vec3 getAmbient() const { return ambient_; }
        glm::vec3 getDiffuse() const { return diffuse_; }
        glm::vec3 getSpecular() const { return specular_; }
        void setAmbient(glm::vec3 ambient) { ambient_ = ambient; }
        void setDiffuse(glm::vec3 diffuse) { diffuse_ = diffuse; }
        void setSpecular(glm::vec3 specular) { specular_ = specular; }

        virtual ~LightSource() = default;
        bool getOn() { return on_; }
        void flipOnOff() { on_ = !on_; };
    
    private:
        bool on_ = true;
        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;
};

#endif
