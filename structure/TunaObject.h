#pragma once

#include <string>
#include <glm/glm.hpp>
using namespace std;

class TunaObject {

public:
	virtual void render()=0;
	int getId();
	string getName();
	void setName(string newString);

private:
	static int idGenCount;
	int id;
	string name;
};