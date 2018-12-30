#include "Element.h"

void tunage::Element::setNode(Node &node) {
	this->node = node;
}

void tunage::Element::setMaterial(Material &material) {
	this->material = material;
}

void tunage::Element::setMatrix(glm::mat4 matrix) {
	this->matrix = matrix;
}
