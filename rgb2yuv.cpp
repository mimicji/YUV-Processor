//==============================================
//	Name: rgb2yuv.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/19/2016 23:37
//	All rights reserved. Only for Windows. 
//==============================================


#include "process.h"
namespace{
_forceinline inline uint8_t format(int16_t input) {
	if (input > 255) {
		return (uint8_t)255;
	}
	if (input < 0) {
		return (uint8_t)0;
	}
	return (uint8_t)input;
}
}

void Non_Simd::rgb2yuv(const YUV* dst_yuv, const RGB* src_rgb) {
	int i;
	for (i = 0; i < dst_yuv->height * dst_yuv->width; i++) {
			dst_yuv->y_ptr[i] = format(0.256788 * src_rgb->r_ptr[i] + 0.504129 * src_rgb->g_ptr[i] + 0.097906 * src_rgb->b_ptr[i] +16);
	}
	int j, k=0;
	for (i = 0; i < dst_yuv->height; i+=2) {
		for (j = 0; j < dst_yuv->width; j+=2) {
			dst_yuv->v_ptr[k] = format(-0.148223 * src_rgb->r_ptr[i * dst_yuv->width + j] - 0.290993 * src_rgb->g_ptr[i * dst_yuv->width + j] + 0.439216 * src_rgb->b_ptr[i * dst_yuv->width + j] + 128);
			dst_yuv->u_ptr[k] = format( 0.439216 * src_rgb->r_ptr[i * dst_yuv->width + j] - 0.367788 * src_rgb->g_ptr[i * dst_yuv->width + j] - 0.071427 * src_rgb->b_ptr[i * dst_yuv->width + j] + 128);
			++k;
		}
	}
}

void MMX::rgb2yuv(const YUV* dst_yuv, const RGB* src_rgb) {
	// TODO
}

void AVX::rgb2yuv(const YUV* dst_yuv, const RGB* src_rgb) {
	// TODO
}

void SSE::rgb2yuv(const YUV* dst_yuv, const RGB* src_rgb) {
	// TODO
}