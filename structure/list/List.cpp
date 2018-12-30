#include "List.h"
#include "../mesh/Mesh.h"
#include "../light/Light.h"

void tunage::List::pass(Node &element) {
	Element listElement;
	listElement.setNode(element);
	listElement.setMatrix(element.getRenderMatrix());

	if (dynamic_cast<Mesh *>(&element) == nullptr) {
		listElement.setMaterial(defaultMaterial);
	} else {
		Mesh *mesh = dynamic_cast<Mesh *>(&element);
		Material mat = mesh->getMaterial();
		listElement.setMaterial(mat);
	}
	if (dynamic_cast<Light *>(&element) == nullptr) {
		renderSequenceElements.push_back(listElement);
	} else {
		renderSequenceLightElements.push_back(listElement);
	}

	for (auto &i : element.getChildren()) {
		pass(*i);
	}

}
