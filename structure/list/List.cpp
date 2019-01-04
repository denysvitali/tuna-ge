#include "List.h"
#include "../mesh/Mesh.h"
#include "../light/Light.h"

void tunage::List::pass(Node* element){
	Element listElement{element};
	listElement.setMatrix((*element).getRenderMatrix());
	
	if (dynamic_cast<Mesh*>(element) != nullptr)
	{
		Mesh* mesh = dynamic_cast<Mesh*>(element);
		Material mat = mesh->getMaterial();
		listElement.setMaterial(mat);
		renderSequenceElements.push_back(listElement);
		
	}
	else if (dynamic_cast<Light*>(element) != nullptr)
	{
        defaultMaterial.setAmbient(glm::vec3(1.0f, 1.0f, 1.0f));
        defaultMaterial.setSpecular(glm::vec3(0.6f, 0.6f, 0.6f));
        defaultMaterial.setShininess(120);
        defaultMaterial.setDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
        defaultMaterial.setEmission(glm::vec3(1.0f, 1.0f, 1.0f));
		listElement.setMaterial(defaultMaterial);
		renderSequenceLights.push_back(listElement);
	}

	for (auto i : (*element).getChildren()) {
		pass(i);
	}

}

void tunage::List::render()
{
	for (auto i = renderSequenceLights.begin(); i != renderSequenceLights.end(); ++i) {
		(*i->getNode()).render(camera * i->getMatrix(), i->getMaterial());
	}
	for (auto i = renderSequenceElements.begin(); i != renderSequenceElements.end(); ++i) {
		(*i->getNode()).render(camera * i->getMatrix(), i->getMaterial());
	}
}

void tunage::List::setCameraMatrix(glm::mat4 camera)
{
	this->camera = camera;
}

const std::vector<tunage::Element>& tunage::List::getRenderElements() const{
	return renderSequenceElements;
}

const std::vector<tunage::Element>& tunage::List::getRenderLights() const {
	return renderSequenceLights;
}
