//
// Created by dvitali on 17.11.18.
//
#include "Vertex.h"

using namespace tunage;

glm::vec3 tunage::Vertex::getPos() const {
	return pos;
}

glm::vec3 tunage::Vertex::getNorm() const {
	return normal;
}

glm::vec2 tunage::Vertex::getUV() const {
	return uv;
}
