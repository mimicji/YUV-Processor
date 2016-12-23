//==============================================
//	Name: process.h
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/19/2016 19:01
//	All rights reserved.
//==============================================

// Header for MMX
#include <mmintrin.h> 
// Header for AVX
#include <immintrin.h>
// Header for SSE2
#include <emmintrin.h>

#include <time.h>
#include <fstream>
#include "rgb.h"
#include "yuv.h"

using std::cerr;
using std::endl;

#define WIDTH 1920
#define HEIGHT 1080

namespace Non_Simd {
	void yuv2rgb(const RGB* dst_rgb, const YUV* src_yuv);
	void rgb2yuv(const YUV* dst_yuv, const RGB* src_rgb);
	void alpha_blend(const RGB* dst_rgb, const RGB* src_rgb, const uint8_t alpha);
	void image_overlay(const RGB* dst_rgb, const RGB* src_rgb_1, const RGB* src_rgb_2, const uint8_t alpha);
}

namespace MMX {
	void yuv2rgb(const RGB* dst_rgb, const YUV* src_yuv);
	void rgb2yuv(const YUV* dst_yuv, const RGB* src_rgb);
	void alpha_blend(const RGB* dst_rgb, const RGB* src_rgb, const uint8_t alpha);
	void image_overlay(const RGB* dst_rgb, const RGB* src_rgb_1, const RGB* src_rgb_2, const uint8_t alpha);
}

namespace AVX {
	void yuv2rgb(const RGB* dst_rgb, const YUV* src_yuv);
	void rgb2yuv(const YUV* dst_yuv, const RGB* src_rgb);
	void alpha_blend(const RGB* dst_rgb, const RGB* src_rgb, const uint8_t alpha);
	void image_overlay(const RGB* dst_rgb, const RGB* src_rgb_1, const RGB* src_rgb_2, const uint8_t alpha);
}

namespace SSE {
	void yuv2rgb(const RGB* dst_rgb, const YUV* src_yuv);
	void rgb2yuv(const YUV* dst_yuv, const RGB* src_rgb);
	void alpha_blend(const RGB* dst_rgb, const RGB* src_rgb, const uint8_t alpha);
	void image_overlay(const RGB* dst_rgb, const RGB* src_rgb_1, const RGB* src_rgb_2, const uint8_t alpha);
}
// End of process.h