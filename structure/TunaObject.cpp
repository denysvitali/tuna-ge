#include "TunaObject.h"
#include "../stdafx.h"

int TunaObject::idGenCount = 0;

int TunaObject::getId()
{
	return id;
}

string TunaObject::getName()
{
	return name;
}

void TunaObject::setName(string newName)
{
	name = newName;
}
