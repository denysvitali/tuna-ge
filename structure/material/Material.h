#pragma once

#include "../color/RGBColor.h"
#include "../node/Node.h"

namespace tunage {
    class Material : Node{
    public:
        Material(RGBColor color) : m_color{color}{};
        RGBColor color();
		void render() override;
    private:
        RGBColor m_color;
    };
}