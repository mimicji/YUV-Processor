//==============================================
//	Name: alpha_blend.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/19/2016 19:01
//	All rights reserved. Only for Windows. 
//==============================================

#include "process.h"
void Non_Simd::alpha_blend(const RGB* dst_rgb, const RGB* src_rgb, const uint8_t alpha) {
	int i;
	for(i=0; i<dst_rgb->getHeight()*dst_rgb->getWidth(); i++) {
		dst_rgb->r_ptr[i] = ((uint16_t)src_rgb->r_ptr[i] * alpha) >> 8;
		dst_rgb->g_ptr[i] = ((uint16_t)src_rgb->g_ptr[i] * alpha) >> 8;
		dst_rgb->b_ptr[i] = ((uint16_t)src_rgb->b_ptr[i] * alpha) >> 8;
	}
}

void Non_Simd::image_overlay(const RGB* dst_rgb, const RGB* src_rgb_1, const RGB* src_rgb_2, const uint8_t alpha) {
	int i;
	uint8_t _alpha = 256 - alpha;
	for (i = 0; i<dst_rgb->getHeight()*dst_rgb->getWidth(); i++) {
		dst_rgb->r_ptr[i] = ((uint16_t)src_rgb_1->r_ptr[i] * alpha + (uint16_t)src_rgb_2->r_ptr[i] * _alpha) >> 8;
		dst_rgb->g_ptr[i] = ((uint16_t)src_rgb_1->g_ptr[i] * alpha + (uint16_t)src_rgb_2->g_ptr[i] * _alpha) >> 8;
		dst_rgb->b_ptr[i] = ((uint16_t)src_rgb_1->b_ptr[i] * alpha + (uint16_t)src_rgb_2->b_ptr[i] * _alpha) >> 8;
	}
}

void MMX::alpha_blend(const RGB* dst_rgb, const RGB* src_rgb, const uint8_t alpha) {
	// TODO
}

void MMX::image_overlay(const RGB* dst_rgb, const RGB* src_rgb_1, const RGB* src_rgb_2, const uint8_t alpha) {
	// TODO
}

void AVX::alpha_blend(const RGB* dst_rgb, const RGB* src_rgb, const uint8_t alpha) {
	// TODO
}

void AVX::image_overlay(const RGB* dst_rgb, const RGB* src_rgb_1, const RGB* src_rgb_2, const uint8_t alpha) {
	// TODO
}

void SSE::alpha_blend(const RGB* dst_rgb, const RGB* src_rgb, const uint8_t alpha) {
	// TODO
}

void SSE::image_overlay(const RGB* dst_rgb, const RGB* src_rgb_1, const RGB* src_rgb_2, const uint8_t alpha) {
	// TODO
}