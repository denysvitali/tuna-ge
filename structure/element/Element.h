#pragma once

#include "../../libapi.h"
#include "../material/Material.h"
#include "../node/Node.h"

namespace tunage {

    class LIB_API Element{
    public:
		explicit Element(Node* elem) : node{ elem }{};

		void setNode(Node* node);
		Node* getNode() const;
		void setMaterial(Material* material);
		Material* getMaterial() const;
		void setMatrix(glm::mat4 matrix);
		glm::mat4 getMatrix() const;
		
    private:
		Node* node;
		Material* material;
		glm::mat4 matrix = glm::mat4(1.0);
    };
}