#include "../stdafx.h"
#include "TunaObject.h"
#include <utility>

using namespace tunage;

int TunaObject::idGenCount = 0;

TunaObject::TunaObject()
{
	m_id = idGenCount;
	idGenCount++;
}

TunaObject::TunaObject(std::string name) : m_name{std::move(name)} {
    m_id = idGenCount;
    idGenCount++;
}

int TunaObject::getId()
{
	return m_id;
}

std::string TunaObject::getName()
{
	return m_name;
}

void TunaObject::setName(std::string newName)
{
	m_name = newName;
}
