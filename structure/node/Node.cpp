#include <stdexcept>
#include "Node.h"

namespace tunage {
	void Node::render() {
		throw std::runtime_error("render() called on a pure Node object");
	}

	void Node::renderReflection() {
		throw std::runtime_error("render() called on a pure Node object");
	}

	void Node::render(glm::mat4 pos, Material mat) {
		throw std::runtime_error("render() called on a pure Node object");
	}

	void Node::setMatrix(glm::mat4 matrix) {
		m_matrix = matrix;
	}

	void Node::setParent(Node* parent) {
		this->m_parent = parent;
	}

	Node* Node::getParent() const {
		return m_parent;
	}

	std::vector<Node*> Node::getChildren() {
		return m_hierarchy;
	}

	void Node::link(Node* child) {
		child->setParent(this);
		m_hierarchy.push_back(child);
	}

	Node* Node::unlinkById(int id) {
		for (auto i = m_hierarchy.begin(); i != m_hierarchy.end(); ++i) {
			if ((*i)->getId() == id) {
				(*i)->setParent(nullptr);
				Node* node = *i;
				m_hierarchy.erase(i);
				return node;
			}
		}
		return nullptr;
	}

	Node* Node::unlink() {
		if (m_parent != nullptr) {
			return m_parent->unlinkById(getId());
		}
		return nullptr;
	}

	Node* Node::getSceneElementByName(const char* name) {
		if (this->getName() == name) return this;

		for (Node* node : m_hierarchy) {
			Node* returnOfBranch = node->getSceneElementByName(name);
			if (returnOfBranch != nullptr) return returnOfBranch;
		}
		return nullptr;

	}

	glm::mat4 Node::getMatrix() const {
		return m_matrix;
	}

	glm::mat4 Node::getRenderMatrix() const {
		glm::mat4 composedMatrix;
		if (m_parent != nullptr) {
			composedMatrix = m_parent->getRenderMatrix() * m_matrix;
			return composedMatrix;
		}
		return m_matrix;
	}

	void Node::setMirror(bool val) {
		this->mirror = val;
	}

	void Node::renderReflection(glm::mat4 pos, Material mat) {

	}
};