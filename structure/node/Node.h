#pragma once

#include "../../libapi.h"
#include "../object/Object.h"
#include "../material/Material.h"
#include <map>
#include <vector>


namespace tunage {
	class LIB_API Node : public Object {

	public:

		virtual ~Node();
		Node() : Object() {}

		Node(std::string name) : Object(name) {};

		void render() override;
		virtual void render(glm::mat4 pos, Material* mat);

		void setMatrix(glm::mat4 matrix);

		void setFlipScene(bool flipScene);

		bool getFlipScene() const;

		glm::mat4 getMatrix() const;

		virtual glm::mat4 getRenderMatrix() const;

		Node* getParent() const;

		void link(Node *child);

		std::vector<Node *> getChildren();

		Node* unlinkById(int id);

		Node* unlink();

		void clearHierarchy();

		Node* getSceneElementByName(const char* name);

		void setAllMaterials(std::map<std::string, Material*> allMaterials);

		std::map<std::string, Material*> getAllMaterials() const;

	private:
		void setParent(Node *parent);
		std::vector<Node*> m_hierarchy;
		std::map<std::string, Material*> allMaterials;
		Node *m_parent = nullptr;
		glm::mat4 m_matrix = glm::mat4(1.0f);
		bool flipScene = false;
	};
}