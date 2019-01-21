//
// Created by dvitali on 20.01.19.
//

#include "Font.h"
#include <GL/freeglut.h>

namespace tunage {
	void* Font::getFont(tunage::FontType ft) {
		switch (ft) {
			case BITMAP_8_BY_13:
				return GLUT_BITMAP_8_BY_13;
			case STROKE_ROMAN:
				return GLUT_STROKE_ROMAN;
			case STROKE_MONO_ROMAN:
				return GLUT_STROKE_MONO_ROMAN;
			case BITMAP_9_BY_15:
				return GLUT_BITMAP_9_BY_15;
			case BITMAP_TIMES_ROMAN_10:
				return GLUT_BITMAP_TIMES_ROMAN_10;
			case BITMAP_TIMES_ROMAN_24:
				return GLUT_BITMAP_TIMES_ROMAN_24;
			case BITMAP_HELVETICA_10:
				return GLUT_BITMAP_HELVETICA_10;
			case BITMAP_HELVETICA_12:
				return GLUT_BITMAP_HELVETICA_12;
			case BITMAP_HELVETICA_18:
				return GLUT_BITMAP_HELVETICA_18;
		}
		return nullptr;
	}
}