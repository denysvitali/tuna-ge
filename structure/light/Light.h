#pragma once
#include "../color/RGBColor.h"

namespace tunage {
    class Light {
    public:
        Light(RGBColor color) : m_color{color} {}
        const RGBColor color();
    private:
        RGBColor m_color;
    };
}