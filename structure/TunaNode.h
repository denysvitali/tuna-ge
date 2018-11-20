#pragma once
#include "TunaObject.h"
#include <vector>

class TunaNode : public TunaObject {

public:
	void setMatrix(glm::mat4 matrix);
	void link(TunaNode* child);
	TunaNode* unlinkByName(string name);
	TunaNode* unlinkById(int id);
private:

	vector<TunaNode*> hierarchy;
	glm::mat4 nodeMatrix;
};