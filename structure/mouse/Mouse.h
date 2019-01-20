#pragma once

namespace tunage {
	class Mouse {
	public:
		enum Button {
			LEFT,
			MIDDLE,
			RIGHT,
			UNKNOWN
		};
		static Button getButton(int glut_macro);
	};
}