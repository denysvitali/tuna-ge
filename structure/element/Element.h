#pragma once

#include "../material/Material.h"
#include "../node/Node.h"

namespace tunage {
    class Element{
    public:
		Element(Node& elem, Material& mat, glm::mat4 mtx) : node{elem}, material{mat}, matrix{mtx} {};
		
    private:
		Node node;
		Material material;
		glm::mat4 matrix;
    };
}