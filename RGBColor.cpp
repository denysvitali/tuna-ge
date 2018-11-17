//
// Created by dvitali on 17.11.18.
//

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <stdio.h>
#include "RGBColor.h"

using namespace tunage;

bool RGBColor::isHex(char c){
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}

RGBColor::RGBColor(float r, float g, float b) : m_r{r}, m_g{g}, m_b{b} {}

RGBColor RGBColor::getColor(const char *color){
    char* lc_color = new char[strlen(color) + 1];
    strcpy(lc_color, color);

    int i = 0;
    while(lc_color[i]){
        lc_color[i] = static_cast<char>(tolower(lc_color[i]));
        i++;
    }
    int hexLen = static_cast<int>(strlen("#ffffff"));
    RGBColor rgbColor = RGBColor{0.0, 0.0, 0.0};
    if(lc_color[0] == '#') {
        if (strlen(lc_color) == hexLen) {
            // RGB Hex Color
            for (int i = 1; i < hexLen; i++) {
                if (!isHex(lc_color[i])) {
                    return rgbColor;
                }
            }

            char *output = new char[hexLen + 1];
            sprintf(output, "0x%s", lc_color  + 1);

            int num = (int) strtol(output, nullptr, 0);

            auto r = static_cast<float>((num >> 16 & 0xFF) / 255.0);
            auto g = static_cast<float>(((num >> 8) & 0xFF) / 255.0);
            auto b = static_cast<float>(((num) & 0xFF) / 255.0);

            rgbColor = RGBColor(r, g, b);
        }
    }

    return rgbColor;
}

const float RGBColor::r() {
    return m_r;
}

const float RGBColor::g() {
    return m_g;
}

const float RGBColor::b() {
    return m_b;
}
