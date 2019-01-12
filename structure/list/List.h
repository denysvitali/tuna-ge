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
	    List(std::string name) : Object(name) {};
		void pass(Node* element);
		void render() override;
		void setCameraMatrix(glm::mat4 cameraMatrix);
		void clearRenderElements();
		void clearCameras();
		const std::vector<Element>& getRenderElements() const;
		const std::vector<Element>& getRenderLights() const;
		std::vector<Camera*>& getRenderCameras();
		void switchCamera();
		Node* getSceneRoot() const;

	private:
		Node* sceneRoot = nullptr;
		glm::mat4 cameraMatrix;
		Material* lightMaterial;
		std::vector<Element> renderSequenceElements;
		//std::vector<Element> renderSequenceTransparentElements;
		std::vector<Element> renderSequenceMirrored;
		std::vector<Element> renderSequenceLights;
		std::vector<Element> renderSequenceLightsMirrored;
        std::vector<Camera*> renderCameras;
	};
}