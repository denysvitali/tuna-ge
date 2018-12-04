#pragma once

#include "../color/RGBColor.h"

namespace tunage {
    class Material {
    public:
        Material(){};
        void setAmbient(glm::vec3 light);
        void setDiffuse(glm::vec3 light);
        void setSpecular(glm::vec3 light);
        void setEmission(glm::vec3 light);
        void setShininess(int intensity);
        glm::vec4 getAmbient() const;
        glm::vec4 getDiffuse() const;
        glm::vec4 getSpecular() const;
        glm::vec4 getEmission() const;
        int getShininess() const;
    private:
        int shininess;
        glm::vec4 ambient = glm::vec4{0,0,0,1};
        glm::vec4 diffuse = glm::vec4{0,0,0,1};
        glm::vec4 specular = glm::vec4{0,0,0,1};
        glm::vec4 emission = glm::vec4{0,0,0,1};
    };
}