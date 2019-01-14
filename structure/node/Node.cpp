#include <utility>

#include <stdexcept>
#include "Node.h"

namespace tunage {

	Node::~Node() {
		for (auto node : m_hierarchy) {
			//delete node;
		}
		std::cout << "distrutto id: " << getId() << std::endl;
	}

	void Node::render() {
		throw std::runtime_error("render() called on a pure Node object");
	}

	void Node::render(glm::mat4 pos, Material* mat) {
		throw std::runtime_error("render() called on a pure Node object");
	}

	void Node::setMatrix(glm::mat4 matrix) {
		m_matrix = matrix;
	}

	void Node::setFlipScene(bool flipScene) {
		this->flipScene = flipScene;
	}

	bool Node::getFlipScene() const {
		return flipScene;
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

	//	Links a node as a children of the current node
	void Node::link(Node* child) {
		child->setParent(this);
		m_hierarchy.push_back(child);
	}

	//	Unlinks a children with the specified id (and sets said children parent node to nullptr)
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

	//	Unlinks the current node from the parent node
	Node* Node::unlink() {
		if (m_parent != nullptr) {
			return m_parent->unlinkById(getId());
		}
		return nullptr;
	}

	//	Unlinks this node from the parents and all childrens from this node (only from the point of view of this node).
	//	Mainly used on duplicate nodes if the hierarchy is not needed or before deleting them.
	void Node::clearHierarchy() {
		m_parent = nullptr;
		m_hierarchy.clear();
	}

	//	Recursive method that searches and returns a node with the specified name from the current node onwards.
	Node* Node::getSceneElementByName(const char* name) {
		if (!strcmp(this->getName(), name)) return this;

		for (Node* node : m_hierarchy) {
			Node* returnOfBranch = node->getSceneElementByName(name);
			if (returnOfBranch != nullptr) return returnOfBranch;
		}
		return nullptr;

	}

	glm::mat4 Node::getMatrix() const {
		return m_matrix;
	}

	//	Recursive method that returns the render matrix of the current node by multiplying the current matrix recursively with the parent ones.
	glm::mat4 Node::getRenderMatrix() const {
		glm::mat4 composedMatrix;
		if (m_parent != nullptr) {
			composedMatrix = m_parent->getRenderMatrix() * m_matrix;
			return composedMatrix;
		}
		return m_matrix;
	}

	void Node::setAllMaterials(std::vector<Material*> allMaterials) {
		for(auto* material : allMaterials){
			this->allMaterials[material->getName()] = material;
		}
	}

	std::vector<Material*> Node::getAllMaterials() const {
		std::vector<Material*> materials{};
		for(const auto &material : allMaterials){
			materials.push_back(material.second);
		}
		return materials;
	}
};