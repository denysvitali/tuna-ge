#pragma once
#include "../node/Node.h"
#include "../material/Material.h"
#include "../element/Element.h"
#include <vector>
#include <map>
#include <iostream>

namespace tunage {
	class List: public Object {

	public:
		List() : Object() {}
	    List(std::string name) : Object(name) {};
		void pass(Node& element);
		void render() override;
		void setCameraMatrix(glm::mat4 camera);
		std::vector<Element> getRenderElements() const;
		std::vector<Element> getRenderLights() const;

	private:
		glm::mat4 camera;
		Material defaultMaterial;
		std::vector<Element> renderSequenceElements;
		std::vector<Element> renderSequenceLights;
	};
}