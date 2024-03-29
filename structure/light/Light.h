#pragma once
#include "../color/RGBColor.h"
#include "../node/Node.h"
#include "../material/Material.h"

namespace tunage {
    class LIB_API Light : public Node{
    public:
    	explicit Light() : Node{} {};
		explicit Light(const char* name) : Node{ name }{}

		//	Light on-off
		void enable();
		void disable();
		bool isEnabled();

		//	Light settings
		void setLightAmbient(glm::vec3 lightAmbient);
        glm::vec3 getLightAmbient() const;
		void setLightDiffuse(glm::vec3 lightDiffuse);
        glm::vec3 getLightDiffuse() const;
		void setLightSpecular(glm::vec3 lightSpecular);
        glm::vec3 getLightSpecular() const;
        int getSpotExponent();
        float getInfluenceRadius();
        void setSpotExponent(int val);
		void setLightDirection(glm::vec3 lightDirection);
		void setLightCutoff(float lightCutoff);
		void setLight(int light);
		void setRadius(float radius);
		void setIntensity(float f);
		void setType(unsigned int lightType);
		void setReferenceLight(Light* referenceLight);
		Light* getReferenceLight() const;
		void setInfluenceRadius(float f);

	private:
		//	Rendering methods
		void render() override;
		void render(glm::mat4 pos, Material* mat) override;
		Light* referenceLight = nullptr;

		//	Default settings
		int light = 0;	//	Light number used by FreeGLUT
		glm::vec3 lightPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 lightAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 lightDiffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 lightSpecular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec3 lightDirection = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		int spotExponent = 0; // Value from 0 to 128
		float lightCutoff;
		float radius = 3.0f;
		float intensity = 1;
		float influence = 0.0;
		bool enabled = false;
		unsigned int lightType = 0;
    };
}