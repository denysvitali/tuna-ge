#include <stdafx.h>
#include "TunaNode.h"

void TunaNode::render()
{
	std::cout << "Called render on a pure Node object" << std::endl;
}

void TunaNode::setMatrix(glm::mat4 matrix)
{
	nodeMatrix = matrix;
}

void TunaNode::setParent(TunaNode* parent)
{
	this->parent = parent;
}

TunaNode* TunaNode::getParent()
{
	return parent;
}

void TunaNode::link(TunaNode* child)
{
	child->setParent(this);
	hierarchy.push_back(child);
}

TunaNode* TunaNode::unlinkById(int id)
{
	for (auto i = hierarchy.begin(); i != hierarchy.end(); ++i) {
		if ((*i)->getId() == id) {
			(*i)->setParent(nullptr);
			TunaNode* node = *i;
			hierarchy.erase(i);
			return node;
		}
	}
}
