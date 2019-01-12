#include <utility>

#include "Element.h"

void tunage::Element::setNode(Node* node) {
	this->node = node;
}

tunage::Node* tunage::Element::getNode() const {
	return node;
}

void tunage::Element::setMaterial(Material* material) {
	this->material = material;
}

tunage::Material* tunage::Element::getMaterial() const {
	return material;
}

void tunage::Element::setMatrix(glm::mat4 matrix) {
	this->matrix = matrix;
}

glm::mat4 tunage::Element::getMatrix() const {
	return matrix;
}
