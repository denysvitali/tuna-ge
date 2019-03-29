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

		//	Material settings
        void setAmbient(glm::vec3 light);
		glm::vec3 getAmbient() const;
        void setDiffuse(glm::vec3 light);
		glm::vec3 getDiffuse() const;
        void setSpecular(glm::vec3 light);
		glm::vec3 getSpecular() const;
        void setEmissive(glm::vec3 light);
		glm::vec3 getEmissive() const;
        void setShininess(int intensity);
		int getShininess() const;
		void setAlpha(float alpha);
		bool isTransparent();
		float getAlpha();
        
        //	Texture methods
		void setTexture(Texture* texture);
		Texture* getTexture() const;
    private:
		friend class Mesh;
		friend class Light;
		//	Rendering methods
		void render() override;
		//	Default settings
		Texture* texture = nullptr;
        int shininess;
		float alpha = 1.0f;	//	Alpha channel used for material transparency
        glm::vec3 ambient = glm::vec3{0,0,0};
        glm::vec3 diffuse = glm::vec3{0,0,0};
        glm::vec3 specular = glm::vec3{0,0,0};
        glm::vec3 emission = glm::vec3{0,0,0};
    };
}