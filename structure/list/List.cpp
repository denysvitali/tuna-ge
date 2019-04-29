#include "List.h"
#include "../../tuna-ge.h"
#include "../skybox/Skybox.h"
#include "../mesh/Mesh.h"
#include "../light/Light.h"
#include <algorithm>
#include <GL/freeglut.h>

using namespace tunage;

//	Recursive method to save an entire scene in the render lists by passing the root as parameter.
//	Meshes, Lights, and Cameras are separated in different lists to order the rendering process
//	(lights must be rendered before meshes).
//	The Camera list is used to determinate the current Camera in use.
//	If the mirrored flag is set, the current node will be saved in a "renderMirrored" list to simulate a reflection plane on the y=0 axis; 
//	all his childrens mirrored flag will be set before calling the pass method on them.
void List::pass(Node* element) {

	if (sceneRoot == nullptr) {
		sceneRoot = element;
	}

	Element listElement{element};
	glm::mat4 currentRenderMatrix = parentMatrix * element->getMatrix();
	listElement.setMatrix(currentRenderMatrix);

	if (dynamic_cast<Mesh*>(element) != nullptr) {
		Mesh* mesh = dynamic_cast<Mesh*>(element);
		Material* mat = mesh->getMaterial();
		if (element->getFlipScene()) {
			Element mirrorElement{element};
			mirrorElement.setMatrix(
					glm::scale(glm::mat4(1.0f),
							glm::vec3(1, -1, 1)) * currentRenderMatrix
			);

			mirrorElement.setMaterial(mat);
			renderSequenceMirrored.push_back(mirrorElement);
		}
		listElement.setMaterial(mat);
		renderSequenceElements.push_back(listElement);
	} else if (dynamic_cast<Light*>(element) != nullptr) {
		auto* light = dynamic_cast<Light*>(element);

		lightMaterial = new Material();
		lightMaterial->setEmissive(light->getLightAmbient());
		if (element->getFlipScene()) {
			auto* mirroredLight = new Light();
			*mirroredLight = *light;
			mirroredLight->setReferenceLight(light);
			mirroredLight->clearHierarchy();
			Element mirrorElement{mirroredLight};
			mirrorElement.setMatrix(glm::scale(glm::mat4(1.0f),
					glm::vec3(1, -1, 1)) * currentRenderMatrix);

			mirrorElement.setMaterial(lightMaterial);
			renderSequenceLightsMirrored.push_back(mirrorElement);
		}

		listElement.setMaterial(lightMaterial);
		renderSequenceLights.push_back(listElement);
	} else if (dynamic_cast<Camera*>(element) != nullptr) {
		auto* camera = dynamic_cast<Camera*>(element);
		if (std::find(renderCameras.begin(), renderCameras.end(), camera) == renderCameras.end()) {
			renderCameras.push_back(camera);
		}
	}

	glm::mat4 parentTemp = parentMatrix;
	parentMatrix = currentRenderMatrix;

	for (auto i : element->getChildren()) {
		if (element->getFlipScene()) {
			i->setFlipScene(true);
		}
		pass(i);
	}

	parentMatrix = parentTemp;
}

//	Renders the elements saved in the various render lists using the first camera in the renderCamera list
void tunage::List::render() {

	
	if (renderCameras.empty()) {
		// Create a default camera
		auto camera = new Camera{"Default Camera"};
		renderCameras.push_back(camera);
	}
	renderCameras.front()->update();
	cameraMatrix = additionalModelView * renderCameras.front()->getRenderMatrix();
	if (Skybox::getCurrent() != nullptr) {
		//Stores the current shader program to re-enable it later when the skybox is done rendering
		Program* temp = Program::getCurrent();
		glm::mat4 skyboxMV = glm::translate(cameraMatrix, renderCameras.front()->getAbsolutePosition());
		float l = ((renderCameras.front()->getFarPlane() - renderCameras.front()->getNearPlane()) / sqrt(2)) / 2;
		skyboxMV = glm::scale(skyboxMV, glm::vec3(l, l, l));
		Skybox::getCurrent()->render(proj, skyboxMV);
		temp->render();
	}

	TunaGE::setProjectionMatrix(proj);

	//	Mirrored elements needs to be rendered ClockWise
	glFrontFace(GL_CW);
	int countLight = 0;
	for (auto &i : renderSequenceLightsMirrored) {
		//	In case of mirrored lights we need to set the light number different from the normal lights.
		//	This puts a limitation of 4 lights in a mirrored scenes.
		auto *light = dynamic_cast<Light *>(&(*i.getNode()));
		light->setLight(static_cast<int>(renderSequenceLights.size() + countLight));
		countLight++;

		if (light->getReferenceLight()->isEnabled())
			light->enable();
		else
			light->disable();

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

const std::vector<Element> &List::getRenderElements() const {
	return renderSequenceElements;
}

const std::vector<Element> &List::getRenderLights() const {
	return renderSequenceLights;
}

std::vector<Camera*> &List::getRenderCameras() {
	return renderCameras;
}
/*
const Skybox * tunage::List::getSkybox() const
{
	return skybox;
}

void tunage::List::setSkybox(Skybox * skybox)
{
	this->skybox = skybox;
}
*/
const glm::mat4 tunage::List::getProj() const
{
	return proj;
}

void tunage::List::setProj(glm::mat4 proj)
{
	this->proj = proj;
}

void tunage::List::setAdditionalModelView(glm::mat4 additionalModelView)
{
	this->additionalModelView = additionalModelView;
}

//	Clears the list vectors and deallocates the light materials and the mirrored light nodes allocated in the pass method,
//	the scene root is then set to nullptr.
//	The cameras are not cleared in this method to allow a repopulation of the list without changing the cameras order
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
//	If it is needed to pass a different scene, call this method togheter with clearRenderElements
	renderCameras.clear();
}

void List::switchCamera() {
//	Switches the current camera used for rendering by pushing the front camera in the list at the back
	assert(!renderCameras.empty());
	renderCameras.push_back(renderCameras.front());
	renderCameras.erase(renderCameras.begin());
}

Node* List::getSceneRoot() const {
	return sceneRoot;
}

const std::vector<Element>& List::getRenderElementsMirrored() const {
	return renderSequenceMirrored;
}

const std::vector<Element>& List::getRenderLightsMirrored() const {
	return renderSequenceLightsMirrored;
}
