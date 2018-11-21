#pragma once

#include <RGBColor.h>

namespace tunage {
    class Material {
    public:
        Material(RGBColor color) : m_color{color} {};
        RGBColor color();
    private:
        RGBColor m_color;
    };
}