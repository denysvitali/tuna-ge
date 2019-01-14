#pragma once

#include <FreeImage.h>

namespace testing_utils {
	class ImageComparator {
	public:
		/*
		 * Compare expected to result,
		 * ASSERT_EQ will fail if the two images aren't exactly the same
		 * (same size, same pixel data)
		 */
		static void compare(FIBITMAP* bmp1, FIBITMAP* bmp2);
	};
}