#pragma once

#include "../../libapi.h"
#include "../object/Object.h"
#include "../material/Material.h"
#include <vector>
#include <iostream>

namespace tunage {
	class LIB_API Node : public Object {

	public:
		Node() : Object() {}

		Node(std::string name) : Object(name) {};

		virtual void render() override;
		virtual void render(glm::mat4 pos, Material mat);

		void setMatrix(glm::mat4 matrix);

		glm::mat4 getMatrix() const;

		glm::mat4 getRenderMatrix() const;

		Node *getParent();

		void link(Node *child);

		std::vector<Node *> getChildren();

		Node *unlinkById(int id);

		Node *unlink();

	private:
		void setParent(Node *parent);

		std::vector<Node*> m_hierarchy;
		Node *m_parent = nullptr;
		glm::mat4 m_matrix = glm::mat4(1.0f);
	};
}