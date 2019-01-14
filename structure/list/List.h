#pragma once

#include "../../libapi.h"
#include "../node/Node.h"
#include "../material/Material.h"
#include "../element/Element.h"
#include "../camera/Camera.h"
#include <vector>
#include <map>
#include <iostream>

namespace tunage {
	class LIB_API List: public Object {

	public:
		List() : Object() {}
	    List(const char* name) : Object(name) {};

		void switchCamera();
		Node* getSceneRoot() const;
		const std::vector<Element>& getRenderElements() const;
		const std::vector<Element>& getRenderLights() const;
		std::vector<Camera*>& getRenderCameras();

		//	Fill and render methods
		void pass(Node* element);
		void render() override;

		//	Clear methods
		void clearRenderElements();
		void clearCameras();

	private:
		//	Root of the current scene passed in the list
		Node* sceneRoot = nullptr;

		//	Render matrix of the first camera in the renderCameras list
		glm::mat4 cameraMatrix;

		//	Materials used for lights
		Material* lightMaterial;

		//	Render lists
		std::vector<Element> renderSequenceElements = std::vector<Element>{};
		std::vector<Element> renderSequenceMirrored = std::vector<Element>{};
		std::vector<Element> renderSequenceLights = std::vector<Element>{};
		std::vector<Element> renderSequenceLightsMirrored = std::vector<Element>{};
        std::vector<Camera*> renderCameras = std::vector<Camera*>{};
	};
}