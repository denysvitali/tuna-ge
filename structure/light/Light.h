#pragma once
#include "../color/RGBColor.h"
#include "../node/Node.h"

namespace tunage {
    class LIB_API Light : public Node{
    public:
    	Light() : Node{} {};
		Light(RGBColor color, std::string name) : m_color{color}, Node{ name }{}
		void render() override;
		void render(glm::mat4 pos, Material mat) override;
		void setLightAmbient(glm::vec3 lightAmbient);
        glm::vec3 getLightAmbient() const;
		void setLightDiffuse(glm::vec3 lightDiffuse);
        glm::vec3 getLightDiffuse() const;
		void setLightSpecular(glm::vec3 lightSpecular);
        glm::vec3 getLightSpecular() const;
		void setLightDirection(glm::vec3 lightDirection);
		void setLightCutoff(float lightCutoff);
		void setLight(int light);
		void setMatrix(glm::mat4 matrix);
		void setIntensity(float f);
		void enable();
		void disable();

        RGBColor getColor() const;
		void setColor(RGBColor color);
    private:
        RGBColor m_color;
		int light = 0;
		glm::vec3 lightPosition = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 lightAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 lightDiffuse = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 lightSpecular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec3 lightDirection = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		float lightCutoff;
		float intensity = 0.5;
		bool enabled;
    };
}