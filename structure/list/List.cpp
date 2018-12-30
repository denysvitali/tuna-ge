#include "List.h"
#include "../mesh/Mesh.h"
#include "../light/Light.h"

void tunage::List::pass(Node& element){
	Element listElement{element};
	listElement.setMatrix(element.getRenderMatrix());
	
	if (dynamic_cast<Mesh*>(&element) != nullptr)
	{
		Mesh* mesh = dynamic_cast<Mesh*>(&element);
		Material mat = mesh->getMaterial();
		listElement.setMaterial(mat);
		renderSequenceElements.push_back(listElement);
		
	}
	else if (dynamic_cast<Light*>(&element) != nullptr)
	{
		listElement.setMaterial(defaultMaterial);
		renderSequenceLights.push_back(listElement);
	}

	for (auto i = element.getChildren().begin(); i != element.getChildren().end(); ++i) {
		pass(*(*i));
	}

}

void tunage::List::render()
{
	for (auto i = renderSequenceLights.begin(); i != renderSequenceLights.end(); ++i) {
		i->getNode().render(camera * i->getMatrix, i->getMaterial);
	}
	for (auto i = renderSequenceElements.begin(); i != renderSequenceElements.end(); ++i) {
		i->getNode().render(camera * i->getMatrix, i->getMaterial);
	}
}

void tunage::List::setCameraMatrix(glm::mat4 camera)
{
	this->camera = camera;
}

std::vector<tunage::Element> tunage::List::getRenderElements() const{
	return renderSequenceElements;
}

std::vector<tunage::Element> tunage::List::getRenderLights() const{
	return renderSequenceLights;
}
