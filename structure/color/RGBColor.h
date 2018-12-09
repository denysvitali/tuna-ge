#pragma once
#include <glm/vec3.hpp>

namespace tunage {
    class RGBColor {
        public:
			RGBColor() {}
            RGBColor(float r, float g, float b);
            static RGBColor getColor(const char *color);
            float r() const;
            float g() const;
            float b() const;
            glm::vec3 vec() const;
            glm::vec3 operator*(float f);
        private:
            float m_r;
            float m_g;
            float m_b;
            static bool isHex(char c);
    };
}