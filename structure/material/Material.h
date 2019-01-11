#pragma once

#include "../../libapi.h"
#include "../color/RGBColor.h"
#include "../object/Object.h"
#include "../texture/Texture.h"

namespace tunage {
    class LIB_API Material : public Object{
    public:
		~Material();
		Material() : Object() {};
		void render() override;
        void setAmbient(glm::vec3 light);
        void setDiffuse(glm::vec3 light);
        void setSpecular(glm::vec3 light);
        void setEmission(glm::vec3 light);
        void setShininess(int intensity);
		void setAlpha(float alpha);
        glm::vec3 getAmbient() const;
        glm::vec3 getDiffuse() const;
        glm::vec3 getSpecular() const;
        glm::vec3 getEmission() const;
        int getShininess() const;
		void setTexture(Texture* texture);
		Texture* getTexture() const;
		bool isTransparent();
    private:
		Texture* texture = nullptr;
        int shininess{};
		float alpha = 1.0f;
        glm::vec3 ambient = glm::vec3{0,0,0};
        glm::vec3 diffuse = glm::vec3{0,0,0};
        glm::vec3 specular = glm::vec3{0,0,0};
        glm::vec3 emission = glm::vec3{0,0,0};
    };
}