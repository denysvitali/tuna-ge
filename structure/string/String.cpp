#include "../../tuna-ge.h"
#include "String.h"


using namespace tunage;

const char* String::data() {
	return m_content.data();
}

unsigned long String::size(){
	return m_content.size();
}

bool String::operator==(const String& o) const {
	return this->m_content == o.m_content;
}
