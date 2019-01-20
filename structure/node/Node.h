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
		explicit Node(const char* name) : Object(name) {};

		//	Rendering methods
		virtual void render() override;
		virtual void render(glm::mat4 pos, Material* mat);

		//	Scene methods
		void link(Node *child);
		std::vector<Node *> getChildren();
		Node* unlinkById(int id);
		Node* unlink();
		void clearHierarchy();
		Node* getParent() const;

		//	Recursive methods to iterate the scene
		Node* getSceneElementByName(const char* name);
		virtual glm::mat4 getRenderMatrix() const;

		void setMatrix(glm::mat4 matrix);
		glm::mat4 getMatrix() const;
		void setFlipScene(bool flipScene);
		bool getFlipScene() const;

	private:
		//	Scene methods
		void setParent(Node *parent);

		//	Scene fields
		std::vector<Node*> m_hierarchy;
		Node *m_parent = nullptr;

		glm::mat4 m_matrix = glm::mat4(1.0f);
		bool flipScene = false;	//	Flag to indicate at the rendering list from which node to start mirroring the scene
	};
}