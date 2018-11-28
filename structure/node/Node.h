#pragma once
#include "../object/Object.h"
#include <vector>
#include <iostream>

namespace tunage {
	class Node : public Object {

	public:
	    Node(std::string name) : Object(name) {};
		void render() override;
		void setMatrix(glm::mat4 matrix);
		glm::mat4 getMatrix() const;
		glm::mat4 getRenderMatrix() const;
		Node* getParent();
		void link(Node *child);
		std::vector<Node*> getChildren();
		Node* unlinkById(int id);
		Node* unlink();

	private:
		void setParent(Node *parent);
		std::vector<Node*> m_hierarchy;
		Node *m_parent = nullptr;
		glm::mat4 m_matrix;
	};
}