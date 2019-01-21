#include <GL/freeglut.h>
#include "Button.h"

using namespace tunage;

Button::State Button::getState(int glutmacro){
	switch(glutmacro){
		case GLUT_UP:
			return Button::State::UP;
		case GLUT_DOWN:
			return Button::State::DOWN;
	}

	return Button::State::UNKNOWN;
}