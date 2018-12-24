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
		void render() override;
		void pass(Node& element);

	private:
		std::vector<Element> renderSequenceElements;
	};
}