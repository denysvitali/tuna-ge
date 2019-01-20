#pragma once

namespace tunage{

	enum FontType {
		STROKE_ROMAN,
		STROKE_MONO_ROMAN,
		BITMAP_9_BY_15,
		BITMAP_8_BY_13,
		BITMAP_TIMES_ROMAN_10,
		BITMAP_TIMES_ROMAN_24,
		BITMAP_HELVETICA_10,
		BITMAP_HELVETICA_12,
		BITMAP_HELVETICA_18
	};

	class Font {
	public:
		static void* getFont(FontType ft);
	};
};