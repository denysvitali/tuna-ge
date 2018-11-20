#include <stdafx.h>
#include "TunaNode.h"

void TunaNode::setMatrix(glm::mat4 matrix)
{
	nodeMatrix = matrix;
}

void TunaNode::link(TunaNode* child)
{
	hierarchy.push_back(child);
}

TunaNode* TunaNode::unlinkByName(string name)
{
	for (auto i = hierarchy.begin(); i != hierarchy.end(); ++i) {
		if ((*i)->getName() == name) {
			TunaNode* node = *i;
			hierarchy.erase(i);
			return node;
		}
	}
}

TunaNode* TunaNode::unlinkById(int id)
{
	for (auto i = hierarchy.begin(); i != hierarchy.end(); ++i) {
		if ((*i)->getId() == id) {
			TunaNode* node = *i;
			hierarchy.erase(i);
			return node;
		}
	}
}
