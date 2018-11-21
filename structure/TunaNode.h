#pragma once
#include "TunaObject.h"
#include <vector>
#include <iostream>

class TunaNode : public TunaObject {

public:
	void render() override;
	void setMatrix(glm::mat4 matrix);
	TunaNode* getParent();
	void link(TunaNode* child);
	TunaNode* unlinkById(int id);
private:
	void setParent(TunaNode* parent);
	vector<TunaNode*> hierarchy;
	TunaNode* parent;
	glm::mat4 nodeMatrix;
};