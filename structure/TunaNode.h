#pragma once
#include "TunaObject.h"
#include <vector>
#include <iostream>

namespace tunage {
	class TunaNode : public TunaObject {

	public:
	    TunaNode(std::string name) : TunaObject(name) {};
		void render() override;
		void setMatrix(glm::mat4 matrix);
		glm::mat4 getMatrix();
		TunaNode* getParent();
		void link(TunaNode *child);
		std::vector<TunaNode*> getChildren();
		TunaNode* unlinkById(int id);

	private:
		void setParent(TunaNode *parent);
		std::vector<TunaNode*> m_hierarchy;
		TunaNode *m_parent = nullptr;
		glm::mat4 m_matrix;
	};
}