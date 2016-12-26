//==============================================
//	Name: image_overlay.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/26/2016 12:47
//	All rights reserved.
//==============================================

#include "process.h"

void Non_Simd::image_overlay(const RGB* dst_rgb, const RGB* src_rgb_1, const RGB* src_rgb_2, const uint8_t alpha) {
	int i;
	uint8_t _alpha = 256 - alpha;
	for (i = 0; i < dst_rgb->getHeight() * dst_rgb->getWidth(); i++) {
		dst_rgb->r8[i] = ((uint16_t)src_rgb_1->r8[i] * alpha + (uint16_t)src_rgb_2->r8[i] * _alpha) >> 8;
		dst_rgb->g8[i] = ((uint16_t)src_rgb_1->g8[i] * alpha + (uint16_t)src_rgb_2->g8[i] * _alpha) >> 8;
		dst_rgb->b8[i] = ((uint16_t)src_rgb_1->b8[i] * alpha + (uint16_t)src_rgb_2->b8[i] * _alpha) >> 8;
	}
}

namespace MMX{
inline void image_overlay_MMX(__m64* dst, __m64* src1, __m64* src2, const uint16_t __alpha, const unsigned int loop) {
	int i = 256 - __alpha;
	_mm_empty();
	__m64 alpha = _mm_set_pi16(__alpha, __alpha, __alpha, __alpha);
	__m64 _alpha = _mm_set_pi16(i, i, i, i);
	__m64 tmp1, tmp2;
	for (i = 0; i < loop; i++) {
		tmp1 = _m_pmullw(*src1, alpha);
		tmp2 = _m_pmullw(*src2, _alpha);
		*dst = _m_psrlwi(_m_paddusw(tmp1, tmp2), 8);
		dst++;
		src1++;
		src2++;
	}
	_mm_empty();
}

inline void overlay_one_color_MMX(int16_t* dst16, uint16_t* src16_1, uint16_t* src16_2, const uint8_t alpha) {
	image_overlay_MMX((__m64*)dst16, (__m64*)src16_1, (__m64*)src16_2, alpha, WIDTH * HEIGHT / 4);
}
}

void MMX::image_overlay(const RGB* dst_rgb, const RGB* src_rgb_1, const RGB* src_rgb_2, const uint8_t alpha) {
	// In this function, it only changes the RGB16. Thus, it can not match functions in Non_Simd
	overlay_one_color_MMX(dst_rgb->r16, (uint16_t*)src_rgb_1->r16, (uint16_t*)src_rgb_2->r16, alpha);
	overlay_one_color_MMX(dst_rgb->g16, (uint16_t*)src_rgb_1->g16, (uint16_t*)src_rgb_2->g16, alpha);
	overlay_one_color_MMX(dst_rgb->b16, (uint16_t*)src_rgb_1->b16, (uint16_t*)src_rgb_2->b16, alpha);
}

namespace AVX {
	inline void image_overlay_AVX(__m256i* dst, __m256i* src1, __m256i* src2, const uint16_t __alpha, const unsigned int loop) {
		int i = 256 - __alpha;
		_mm_empty();
		__m256i alpha = _mm256_set_epi16(__alpha, __alpha, __alpha, __alpha, __alpha, __alpha, __alpha, __alpha, __alpha, __alpha, __alpha, __alpha, __alpha, __alpha, __alpha, __alpha);
		__m256i _alpha = _mm256_set_epi16(i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i);
		__m256i tmp1, tmp2;
		for (i = 0; i < loop; i++) {
			tmp1 = _mm256_mullo_epi16(*src1, alpha);
			tmp2 = _mm256_mullo_epi16(*src2, _alpha);
			*dst = _mm256_srli_epi16(_mm256_add_epi16(tmp1, tmp2), 8);
			dst++;
			src1++;
			src2++;
		}
		_mm_empty();
	}

	inline void overlay_one_color_AVX(int16_t* dst16, uint16_t* src16_1, uint16_t* src16_2, const uint8_t alpha) {
		image_overlay_AVX((__m256i*)dst16, (__m256i*)src16_1, (__m256i*)src16_2, alpha, WIDTH * HEIGHT / 16);
	}
}

void AVX::image_overlay(const RGB* dst_rgb, const RGB* src_rgb_1, const RGB* src_rgb_2, const uint8_t alpha) {
	// In this function, it only changes the RGB16. Thus, it can not match functions in Non_Simd
	overlay_one_color_AVX(dst_rgb->r16, (uint16_t*)src_rgb_1->r16, (uint16_t*)src_rgb_2->r16, alpha);
	overlay_one_color_AVX(dst_rgb->g16, (uint16_t*)src_rgb_1->g16, (uint16_t*)src_rgb_2->g16, alpha);
	overlay_one_color_AVX(dst_rgb->b16, (uint16_t*)src_rgb_1->b16, (uint16_t*)src_rgb_2->b16, alpha);
}

namespace SSE {
	inline void image_overlay_SSE(__m128i* dst, __m128i* src1, __m128i* src2, const uint16_t __alpha, const unsigned int loop) {
		int i = 256 - __alpha;
		_mm_empty();
		__m128i alpha = _mm_set_epi16(__alpha, __alpha, __alpha, __alpha, __alpha, __alpha, __alpha, __alpha);
		__m128i _alpha = _mm_set_epi16(i, i, i, i, i, i, i, i);
		__m128i tmp1, tmp2;
		for (i = 0; i < loop; i++) {
			tmp1 = _mm_mullo_epi16(*src1, alpha);
			tmp2 = _mm_mullo_epi16(*src2, _alpha);
			*dst = _mm_srli_epi16(_mm_add_epi16(tmp1, tmp2), 8);
			dst++;
			src1++;
			src2++;
		}
		_mm_empty();
	}

	inline void overlay_one_color_SSE(int16_t* dst16, uint16_t* src16_1, uint16_t* src16_2, const uint8_t alpha) {
		image_overlay_SSE((__m128i*)dst16, (__m128i*)src16_1, (__m128i*)src16_2, alpha, WIDTH * HEIGHT / 8);
	}
}

void SSE::image_overlay(const RGB* dst_rgb, const RGB* src_rgb_1, const RGB* src_rgb_2, const uint8_t alpha) {
	// In this function, it only changes the RGB16. Thus, it can not match functions in Non_Simd
	overlay_one_color_SSE(dst_rgb->r16, (uint16_t*)src_rgb_1->r16, (uint16_t*)src_rgb_2->r16, alpha);
	overlay_one_color_SSE(dst_rgb->g16, (uint16_t*)src_rgb_1->g16, (uint16_t*)src_rgb_2->g16, alpha);
	overlay_one_color_SSE(dst_rgb->b16, (uint16_t*)src_rgb_1->b16, (uint16_t*)src_rgb_2->b16, alpha);
}
// End of image_overlay.cpp