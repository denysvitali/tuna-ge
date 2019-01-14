#include <gtest/gtest.h>
#include "ImageComparator.h"

using namespace testing_utils;

void ImageComparator::compare(FIBITMAP* expected, FIBITMAP* rendered) {
	int r_w = FreeImage_GetWidth(rendered);
	int r_h = FreeImage_GetHeight(rendered);
	int r_bpp = FreeImage_GetBPP(rendered);

	int e_w = FreeImage_GetWidth(expected);
	int e_h = FreeImage_GetHeight(expected);
	int e_bpp = FreeImage_GetBPP(expected);

	ASSERT_EQ(e_w, r_w);
	ASSERT_EQ(e_h, r_h);
	ASSERT_EQ(e_bpp, r_bpp);

	for(int i=0; i<e_h; i++){
		BYTE* sl1 = FreeImage_GetScanLine(expected, i);
		BYTE* sl2 = FreeImage_GetScanLine(rendered, i);

		for(int j=0; j<e_w; j++){
			ASSERT_EQ(sl1[j], sl2[j]);
		}
	}
}
