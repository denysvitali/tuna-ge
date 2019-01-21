#pragma once

namespace tunage {
	class Button {
	public:
		enum State {
			UP,
			DOWN,
			UNKNOWN
		};

		static Button::State getState(int glutmacro);
	};
};