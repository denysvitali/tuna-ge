//
// Created by dvitali on 16.11.18.
//

#ifndef TUNA_GE_RGBCOLOR_H
#define TUNA_GE_RGBCOLOR_H

namespace tunage {
    class RGBColor {
        public:
            RGBColor(float r, float g, float b);
            static RGBColor getColor(const char *color);
            const float r();
            const float g();
            const float b();
        private:
            float m_r;
            float m_g;
            float m_b;
            static bool isHex(char c);
    };
}

#endif