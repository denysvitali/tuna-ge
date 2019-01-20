#include <utility>

#include "Object.h"
#include <utility>

using namespace tunage;

int Object::idGenCount = 0;

Object::Object() {
	m_id = idGenCount;
	m_name = String{""};
	idGenCount++;
}

Object::Object(const char* name) : m_name{name} {
	m_id = idGenCount;
	idGenCount++;
}

int Object::getId() {
	return m_id;
}

String Object::getName() {
	return m_name;
}

void Object::setName(const char* newName) {
	m_name = String{newName};
}
