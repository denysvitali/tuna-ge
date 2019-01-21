#include <GL/freeglut.h>
#include "Keyboard.h"

using namespace tunage;

Keyboard::Key Keyboard::getKey(int glutenum) {
	switch (glutenum) {
		case GLUT_KEY_F1:
			return Key::F1;
		case GLUT_KEY_F2:
			return Key::F2;
		case GLUT_KEY_F3:
			return Key::F3;
		case GLUT_KEY_F4:
			return Key::F4;
		case GLUT_KEY_F5:
			return Key::F5;
		case GLUT_KEY_F6:
			return Key::F6;
		case GLUT_KEY_F7:
			return Key::F7;
		case GLUT_KEY_F8:
			return Key::F8;
		case GLUT_KEY_F9:
			return Key::F9;
		case GLUT_KEY_F10:
			return Key::F10;
		case GLUT_KEY_F11:
			return Key::F11;
		case GLUT_KEY_F12:
			return Key::F12;
		case GLUT_KEY_LEFT:
			return Key::LEFT;
		case GLUT_KEY_UP:
			return Key::UP;
		case GLUT_KEY_RIGHT:
			return Key::RIGHT;
		case GLUT_KEY_DOWN:
			return Key::DOWN;
		case GLUT_KEY_PAGE_UP:
			return Key::PAGE_UP;
		case GLUT_KEY_PAGE_DOWN:
			return Key::PAGE_DOWN;
		case GLUT_KEY_HOME:
			return Key::HOME;
		case GLUT_KEY_END:
			return Key::END;
		case GLUT_KEY_INSERT:
			return Key::INSERT;
	}

	return Key::UNKNOWN;
}