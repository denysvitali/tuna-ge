#pragma once
#include "../color/RGBColor.h"
#include "../node/Node.h"

namespace tunage {
    class Light : Node{
    public:
//		Light(RGBColor color, std::string name) : m_color{color}, Node{ name }{}
		Light(RGBColor color) : m_color{ color }{}
		void render() override;
		void enableLight();
		void setLightPosition(glm::vec3 lightPosition);
		void setLightAmbient(glm::vec3 lightAmbient);
		void setLightDiffuse(glm::vec3 lightDiffuse);
		void setLightSpecular(glm::vec3 lightSpecular);
		void setLightDirection(glm::vec3 lightDirection);
		void setLightCutoff(float lightCutoff);
		void setLight(int light);

        RGBColor getColor() const;
		void setColor(RGBColor color);
    private:
        RGBColor m_color;
		int light = 0;
		glm::vec3 lightPosition;
		glm::vec4 lightAmbient;
		glm::vec4 lightDiffuse;
		glm::vec4 lightSpecular;
		glm::vec3 lightDirection;
		float lightCutoff;
    };
}