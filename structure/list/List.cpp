#include "List.h"
#include "../mesh/Mesh.h"
#include "../light/Light.h"

void tunage::List::pass(Node* element) {

	if (sceneRoot == nullptr) {
		sceneRoot = element;
	}

	Element listElement{ element };
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
		auto light = dynamic_cast<Light*>(element);
		lightMaterial.setEmission(light->getLightAmbient());
		listElement.setMaterial(lightMaterial);
		renderSequenceLights.push_back(listElement);
	}
	else if (dynamic_cast<Camera*>(element) != nullptr)
	{
		auto camera = dynamic_cast<Camera*>(element);
		renderCameras.push_back(camera);
	}
	for (auto i : (*element).getChildren()) {
		pass(i);
	}
}

void tunage::List::render()
{
	if(!renderCameras.empty()){
		renderCameras.front()->updateCamera();
		cameraMatrix = renderCameras.front()->getRenderMatrix();
	}
	for (auto i = renderSequenceLights.begin(); i != renderSequenceLights.end(); ++i) {
		(*i->getNode()).render(cameraMatrix * i->getMatrix(), i->getMaterial());
	}
	for (auto i = renderSequenceElements.begin(); i != renderSequenceElements.end(); ++i) {
		(*i->getNode()).render(cameraMatrix * i->getMatrix(), i->getMaterial());
	}
}

void tunage::List::setCameraMatrix(glm::mat4 cameraMatrix)
{
	this->cameraMatrix = cameraMatrix;
}

const std::vector<tunage::Element>& tunage::List::getRenderElements() const{
	return renderSequenceElements;
}

const std::vector<tunage::Element>& tunage::List::getRenderLights() const {
	return renderSequenceLights;
}

std::vector<tunage::Camera*>& tunage::List::getRenderCameras() {
    return renderCameras;
}

void tunage::List::clear() {
	sceneRoot = nullptr;
	renderCameras.clear();
	renderSequenceLights.clear();
	renderSequenceElements.clear();
}

void tunage::List::switchCamera() {
	assert(!renderCameras.empty());
	renderCameras.push_back(renderCameras.front());
	renderCameras.erase(renderCameras.begin());
}

tunage::Node* tunage::List::getSceneRoot() const{
	return sceneRoot;
}

