#include <stdafx.h>
#include "TunaObject.h"

int TunaObject::idGenCount = 0;

TunaObject::TunaObject()
{
	id = idGenCount;
	idGenCount++;
}

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
