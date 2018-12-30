#include <utility>

#pragma once

#include "../node/Node.h"
#include "../material/Material.h"
#include "../element/Element.h"
#include <vector>
#include <map>
#include <iostream>

namespace tunage {
	class List : public Object {

	public:
		List() : Object() {}

		explicit List(std::string name) : Object(std::move(name)) {};

		void pass(Node &element);

	private:
		Material defaultMaterial;
		std::vector<Element> renderSequenceElements;
		std::vector<Element> renderSequenceLightElements;
	};
}