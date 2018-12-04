#pragma once
#include "../color/RGBColor.h"
#include "../node/Node.h"

namespace tunage {
    class Light : Node{
    public:
		Light(RGBColor color, std::string name) : m_color{color}, Node{ name }{}
		void render() override;
		void setLightAmbient(glm::vec3 lightAmbient);
		void setLightDiffuse(glm::vec3 lightDiffuse);
		void setLightSpecular(glm::vec3 lightSpecular);
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
		float intensity = 0.1;
		bool enabled;
    };
}