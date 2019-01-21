#pragma once

namespace tunage {
	class Keyboard {
	public:
		enum Key {
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			LEFT,
			UP,
			RIGHT,
			DOWN,
			PAGE_UP,
			PAGE_DOWN,
			HOME,
			END,
			INSERT,
			UNKNOWN
		};

		static Key getKey(int glutenum);
	};
}