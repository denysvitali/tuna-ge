#include "Object.h"
#include <utility>

using namespace tunage;

int Object::idGenCount = 0;

Object::Object()
{
	m_id = idGenCount;
	//idGenCount++;
}

Object::Object(std::string name) : m_name{std::move(name)} {
    m_id = idGenCount;
   // idGenCount++;
}

int Object::getId()
{
	return m_id;
}

std::string Object::getName()
{
	return m_name;
}

void Object::setName(std::string newName)
{
	m_name = newName;
}
