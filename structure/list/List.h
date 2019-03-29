#pragma once

#include "../../libapi.h"
#include "../node/Node.h"
#include "../fbo/Fbo.h"
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
		const std::vector<Element>& getRenderElementsMirrored() const;
		const std::vector<Element>& getRenderLights() const;
		const std::vector<Element>& getRenderLightsMirrored() const;
		std::vector<Camera*>& getRenderCameras();

		//	Fill and render methods
		void pass(Node* element);

		//	Clear methods
		void clearRenderElements();
		void clearCameras();

	private:

		
		friend class TunaGE;
		void render() override;

		//	Root of the current scene passed in the list
		Node* sceneRoot = nullptr;

		//	Render matrix of the first camera in the renderCameras list
		glm::mat4 cameraMatrix;

		//  Render matrix of the last parent node (used for pass())
		glm::mat4 parentMatrix = glm::mat4(1.0f);

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