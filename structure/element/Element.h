#pragma once

#include "../material/Material.h"
#include "../node/Node.h"

namespace tunage {
	class Element {
	public:
		Element() = default;

		Element(Node &elem, Material &mat, glm::mat4 mtx) : node{elem}, material{mat}, matrix{mtx} {};

		void setNode(Node &node);

		void setMaterial(Material &material);

		void setMatrix(glm::mat4 matrix);

	private:
		Node node;
		Material material;
		glm::mat4 matrix;
	};
}