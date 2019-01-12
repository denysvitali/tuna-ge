#include "List.h"
#include "../mesh/Mesh.h"
#include "../light/Light.h"
#include <GL/freeglut.h>

//	Recursive method to save an entire scene in the render lists by passing the root as paramether. 
//	Meshes, Lights, and Cameras are separated in different lists to order the rendering process (lights must be rendered before meshes).
//	The Camera list is used to determinate the current Camera in use.
//	If the mirrored flag is set, the current node will be saved in a "renderMirrored" list to simulate a reflection plane on the y=0 axis; 
//	all his childrens mirrored flag will be set before calling the pass method on them.
void tunage::List::pass(Node* element) {

	if (sceneRoot == nullptr) {
		sceneRoot = element;
	}
	Element listElement{ element };
	listElement.setMatrix(element->getRenderMatrix());

	if (dynamic_cast<Mesh*>(element) != nullptr) {
		Mesh* mesh = dynamic_cast<Mesh*>(element);
		Material* mat = mesh->getMaterial();
		if (element->getFlipScene()) {
			Element mirrorElement{ element };
			mirrorElement.setMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1,-1,1)) * element->getRenderMatrix());
			mirrorElement.setMaterial(mat);
			renderSequenceMirrored.push_back(mirrorElement);
		}
		listElement.setMaterial(mat);
		renderSequenceElements.push_back(listElement);
	}
	else if (dynamic_cast<Light*>(element) != nullptr)
	{
		Light* light = dynamic_cast<Light*>(element);

		lightMaterial = new Material();
		lightMaterial->setEmission(light->getLightAmbient());
		if (element->getFlipScene()) {
			Light* mirroredLight = new Light();
			*mirroredLight = *light;
			mirroredLight->clearHierarchy();
			Element mirrorElement{ mirroredLight };
			mirrorElement.setMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(1,-1,1)) * element->getRenderMatrix());
			mirrorElement.setMaterial(lightMaterial);
			renderSequenceLightsMirrored.push_back(mirrorElement);
		}

		listElement.setMaterial(lightMaterial);
		renderSequenceLights.push_back(listElement);
	}
	else if (dynamic_cast<Camera*>(element) != nullptr)
	{
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

//	Renders the elements saved in the various render lists using the first camera in the renderCamera list
void tunage::List::render() {

	if (!renderCameras.empty()) {
		renderCameras.front()->update();
		cameraMatrix = renderCameras.front()->getRenderMatrix();

		//	Mirrored elements needs to be rendered ClockWise
		glFrontFace(GL_CW);
		int countLight = 0;
		for (auto i = renderSequenceLightsMirrored.begin(); i != renderSequenceLightsMirrored.end(); ++i) {

			//	In case of mirrored lights we need to set the light number different from the normal lights.
			//	This puts a limitation of 4 lights in a mirrored scenes.
			Light* light = dynamic_cast<Light*>(&(*i->getNode()));
			light->setLight(renderSequenceLights.size() + countLight);
			countLight++;
			light->enable();
			(*i->getNode()).render(cameraMatrix * i->getMatrix(), i->getMaterial());
		}
		glFrontFace(GL_CCW);
		for (auto i = renderSequenceLights.begin(); i != renderSequenceLights.end(); ++i) {
			(*i->getNode()).render(cameraMatrix * i->getMatrix(), i->getMaterial());
		}
		glFrontFace(GL_CW);
		for (auto i = renderSequenceMirrored.begin(); i != renderSequenceMirrored.end(); ++i) {
			(*i->getNode()).render(cameraMatrix * i->getMatrix(), i->getMaterial());
		}
		glFrontFace(GL_CCW);
		for (auto i = renderSequenceElements.begin(); i != renderSequenceElements.end(); ++i) {
			(*i->getNode()).render(cameraMatrix * i->getMatrix(), i->getMaterial());
		}
	}	
}

const std::vector<tunage::Element> &tunage::List::getRenderElements() const {
	return renderSequenceElements;
}

const std::vector<tunage::Element> &tunage::List::getRenderLights() const {
	return renderSequenceLights;
}

std::vector<tunage::Camera*> &tunage::List::getRenderCameras() {
	return renderCameras;
}

//	Clears the list vectors and deallocates the light materials and the mirrored light nodes allocated in the pass method,
//	the scene root is then set to nullptr.
//	The cameras are not cleared in this method to allow a repopulation of the list without changing the cameras order
void tunage::List::clearRenderElements() {
	renderSequenceElements.clear();
	renderSequenceMirrored.clear();
	for (auto l : renderSequenceLights) {
		delete l.getMaterial();
	}
	for (auto l : renderSequenceLightsMirrored) {
		delete l.getNode();
	}
	renderSequenceLights.clear();
	renderSequenceLightsMirrored.clear();
	sceneRoot = nullptr;
}

//	If it is needed to pass a different scene, call this method togheter with clearRenderElements
void tunage::List::clearCameras() {
	renderCameras.clear();
}

//	Switches the current camera used for rendering by pushing the front camera in the list at the back
void tunage::List::switchCamera() {
	assert(!renderCameras.empty());
	renderCameras.push_back(renderCameras.front());
	renderCameras.erase(renderCameras.begin());
}

tunage::Node* tunage::List::getSceneRoot() const {
	return sceneRoot;
}

