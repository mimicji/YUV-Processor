//==============================================
//	Name: yuv2rgb.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/19/2016 19:01
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

void Non_Simd::yuv2rgb(const RGB* dst_rgb, const YUV* src_yuv){
	int i, j, k = 0;
	int16_t tmp;
	for (i = 0; i < dst_rgb->getHeight(); i++) {
		for (j = 0; j < dst_rgb->getWidth(); j++) {
			//dst_rgb->r_ptr[i] 
			int loc = (i / 2)*dst_rgb->getWidth()/2 + (j / 2);
			////tmp = (int16_t)src_yuv->y_ptr[i] + 1.140 * src_yuv->v_ptr[i / 4];
			tmp = (int16_t)(src_yuv->y_ptr[k] - 16)*1.164383 + 2.017232 * (src_yuv->u_ptr[loc] - 128);
			dst_rgb->r_ptr[k] = format(tmp);
			////tmp = (int16_t)src_yuv->y_ptr[i] - 0.394 * src_yuv->u_ptr[i / 4] - 0.581 *- src_yuv->v_ptr[i / 4];
			tmp = (int16_t)(src_yuv->y_ptr[k] - 16)*1.164383 - 0.391762 * (src_yuv->u_ptr[loc] - 128) - 0.812968 * (src_yuv->v_ptr[loc] - 128);
			dst_rgb->g_ptr[k] = format(tmp);
			////tmp = (int16_t)src_yuv->y_ptr[i] + 2.032 * src_yuv->u_ptr[i / 4];
			tmp = (int16_t)(src_yuv->y_ptr[k] - 16)*1.164383 + 1.596027 * (src_yuv->v_ptr[loc] - 128);
			dst_rgb->b_ptr[k] = format(tmp);
			k++;
		}
	}
}

void MMX::yuv2rgb(const RGB* dst_rgb, const YUV* src_yuv) {
	// TODO
}

void AVX::yuv2rgb(const RGB* dst_rgb, const YUV* src_yuv) {
	// TODO
}

void SSE::yuv2rgb(const RGB* dst_rgb, const YUV* src_yuv) {
	// TODO
}

