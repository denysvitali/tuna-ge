#include <GL/freeglut.h>
#include "Mouse.h"

using namespace tunage;

Mouse::Button Mouse::getButton(int glut_macro) {
	switch(glut_macro){
		case GLUT_LEFT_BUTTON:
			return Mouse::Button::LEFT;
		case GLUT_MIDDLE_BUTTON:
			return Mouse::Button::MIDDLE;
		case GLUT_RIGHT_BUTTON:
			return Mouse::Button::RIGHT;
	}

	return Mouse::Button::UNKNOWN;
}