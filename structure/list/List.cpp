#include "List.h"
#include "../mesh/Mesh.h"
#include "../light/Light.h"
#include <algorithm>
#include <GL/freeglut.h>

//	Recursive method to save an entire scene in the render lists by passing the root as paramether. 
//	Meshes, Lights, and Cameras are separated in different lists to order the rendering process (lights must be rendered before meshes).
//	The Camera list is used to determinate the current Camera in use.
//	If the mirrored flag is set, the current node will be saved in a "renderMirrored" list to simulate a reflection plane on the y=0 axis; 
//	all his childrens mirrored flag will be set before calling the pass method on them.

using namespace tunage;

void List::pass(Node* element) {

	if (sceneRoot == nullptr) {
		sceneRoot = element;
	}
	Element listElement{element};
	listElement.setMatrix(element->getRenderMatrix());

	if (dynamic_cast<Mesh*>(element) != nullptr) {
		Mesh* mesh = dynamic_cast<Mesh*>(element);
		Material* mat = mesh->getMaterial();
		if (element->getFlipScene()) {
			Element mirrorElement{element};
			mirrorElement.setMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1, -1, 1)) * element->getRenderMatrix());
			mirrorElement.setMaterial(mat);
			renderSequenceMirrored.push_back(mirrorElement);
		}
		listElement.setMaterial(mat);
		renderSequenceElements.push_back(listElement);
	} else if (dynamic_cast<Light*>(element) != nullptr) {
		auto* light = dynamic_cast<Light*>(element);

		lightMaterial = new Material();
		lightMaterial->setEmission(light->getLightAmbient());
		if (element->getFlipScene()) {
			auto* mirroredLight = new Light();
			*mirroredLight = *light;
			mirroredLight->clearHierarchy();
			Element mirrorElement{mirroredLight};
			mirrorElement.setMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1, -1, 1)) * element->getRenderMatrix());
			mirrorElement.setMaterial(lightMaterial);
			renderSequenceLightsMirrored.push_back(mirrorElement);
		}

		listElement.setMaterial(lightMaterial);
		renderSequenceLights.push_back(listElement);
	} else if (dynamic_cast<Camera*>(element) != nullptr) {
		Camera* camera = dynamic_cast<Camera*>(element);
		if (std::find(renderCameras.begin(), renderCameras.end(), camera) == renderCameras.end()) {
			renderCameras.push_back(camera);
		}
	}

	for (auto i : element->getChildren()) {
		if (element->getFlipScene()) {
			i->setFlipScene(true);
		}
		pass(i);
	}
}

void List::render() {
	if (!renderCameras.empty()) {
		renderCameras.front()->update();
		cameraMatrix = renderCameras.front()->getRenderMatrix();
	}

	glFrontFace(GL_CW);
	int countLight = 0;
	for (auto &i : renderSequenceLightsMirrored) {
		auto* light = dynamic_cast<Light*>(&(*i.getNode()));
		light->setLight(static_cast<int>(renderSequenceLights.size() + countLight));
		countLight++;
		light->enable();
		(*i.getNode()).render(cameraMatrix * i.getMatrix(), i.getMaterial());
	}
	glFrontFace(GL_CCW);
	for (auto &k : renderSequenceLights) {
		(*k.getNode()).render(cameraMatrix * k.getMatrix(), k.getMaterial());
	}
	glFrontFace(GL_CW);
	for (auto &i : renderSequenceMirrored) {
		(*i.getNode())
				.render(cameraMatrix * i.getMatrix(), i.getMaterial());
	}
	glFrontFace(GL_CCW);
	for (auto &j : renderSequenceElements) {
		(*j.getNode())
				.render(cameraMatrix * j.getMatrix(), j.getMaterial());
	}

}

void List::setCameraMatrix(glm::mat4 cameraMatrix) {
	this->cameraMatrix = cameraMatrix;
}

const std::vector<Element> &List::getRenderElements() const {
	return renderSequenceElements;
}

const std::vector<Element> &List::getRenderLights() const {
	return renderSequenceLights;
}

std::vector<Camera*> &List::getRenderCameras() {
	return renderCameras;
}

void List::clearRenderElements() {
	sceneRoot = nullptr;
	renderSequenceElements.clear();
	renderSequenceMirrored.clear();

	for (auto l : renderSequenceLightsMirrored) {
		delete l.getNode();
	}

	for (auto l : renderSequenceLights) {
		delete l.getMaterial();
	}
	renderSequenceLights.clear();
	renderSequenceLightsMirrored.clear();
}

void List::clearCameras() {
	renderCameras.clear();
}

void List::switchCamera() {
	assert(!renderCameras.empty());
	renderCameras.push_back(renderCameras.front());
	renderCameras.erase(renderCameras.begin());
}

Node* List::getSceneRoot() const {
	return sceneRoot;
}

