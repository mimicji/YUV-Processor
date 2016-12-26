//==============================================
//	Name: alpha_blend.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/24/2016 01:18
//	All rights reserved.
//==============================================

#include "process.h"

void Non_Simd::alpha_blend(const RGB* dst_rgb, const RGB* src_rgb, const uint8_t alpha) {
	int i;
	for (i = 0; i < dst_rgb->getHeight() * dst_rgb->getWidth(); i++) {
		dst_rgb->r8[i] = ((uint16_t)src_rgb->r8[i] * alpha) >> 8;
		dst_rgb->g8[i] = ((uint16_t)src_rgb->g8[i] * alpha) >> 8;
		dst_rgb->b8[i] = ((uint16_t)src_rgb->b8[i] * alpha) >> 8;
	}
}

namespace MMX{
inline void alpha_blend_MMX(__m64* dst, __m64* src, const uint16_t _alpha, const unsigned int loop) {
	int i = 0;
	_mm_empty();
	__m64 alpha = _mm_set_pi16(_alpha, _alpha, _alpha, _alpha);
	__m64 tmp;
	for (i = 0; i < loop; i++) {
		tmp = _m_pmullw(*src, alpha);
		tmp = _m_psrlwi(tmp, 8);
		*dst = tmp;
		dst++;
		src++;
	}
	_mm_empty();
}

inline void blend_one_color_MMX(int16_t* dst16, uint16_t* src16, const uint8_t alpha) {
	alpha_blend_MMX((__m64*)dst16, (__m64*)src16, alpha, WIDTH * HEIGHT / 4);
}
}

void MMX::alpha_blend(const RGB* dst_rgb, const RGB* src_rgb, const uint8_t alpha) {
	// In this function, it only changes the RGB16. Thus, it can not match functions in Non_Simd
	blend_one_color_MMX(dst_rgb->r16, (uint16_t*)src_rgb->r16, alpha);
	blend_one_color_MMX(dst_rgb->g16, (uint16_t*)src_rgb->g16, alpha);
	blend_one_color_MMX(dst_rgb->b16, (uint16_t*)src_rgb->b16, alpha);
}

namespace AVX {
	inline void alpha_blend_AVX(__m256i* dst, __m256i* src, const uint16_t _alpha, const unsigned int loop) {
		int i = 0;
		_mm_empty();
		__m256i alpha = _mm256_set_epi16(_alpha, _alpha, _alpha, _alpha, _alpha, _alpha, _alpha, _alpha, _alpha, _alpha, _alpha, _alpha, _alpha, _alpha, _alpha, _alpha);
		__m256i tmp;
		for (i = 0; i < loop; i++) {
			tmp = _mm256_mullo_epi16(*src, alpha);
			tmp = _mm256_srli_epi16(tmp, 8);
			*dst = tmp;
			dst++;
			src++;
		}
		_mm_empty();
	}

	inline void blend_one_color_AVX(int16_t* dst16, uint16_t* src16, const uint8_t alpha) {
		alpha_blend_AVX((__m256i*)dst16, (__m256i*)src16, alpha, WIDTH * HEIGHT / 16);
	}
}

void AVX::alpha_blend(const RGB* dst_rgb, const RGB* src_rgb, const uint8_t alpha) {
	// In this function, it only changes the RGB16. Thus, it can not match functions in Non_Simd
	blend_one_color_AVX(dst_rgb->r16, (uint16_t*)src_rgb->r16, alpha);
	blend_one_color_AVX(dst_rgb->g16, (uint16_t*)src_rgb->g16, alpha);
	blend_one_color_AVX(dst_rgb->b16, (uint16_t*)src_rgb->b16, alpha);
}

namespace SSE{
inline void alpha_blend_SSE(__m128i* dst, __m128i* src, const uint16_t _alpha, const unsigned int loop) {
	int i = 0;
	_mm_empty();
	__m128i alpha = _mm_set_epi16(_alpha, _alpha, _alpha, _alpha, _alpha, _alpha, _alpha, _alpha);
	__m128i tmp;
	for (i = 0; i < loop; i++) {
		tmp = _mm_mullo_epi16(*src, alpha);
		tmp = _mm_srli_epi16(tmp, 8);
		*dst = tmp;
		dst++;
		src++;
	}
	_mm_empty();
}

inline void blend_one_color_SSE(int16_t* dst16, uint16_t* src16, const uint8_t alpha) {
	alpha_blend_SSE((__m128i*)dst16, (__m128i*)src16, alpha, WIDTH * HEIGHT / 8);
}
}

void SSE::alpha_blend(const RGB* dst_rgb, const RGB* src_rgb, const uint8_t alpha) {
	// In this function, it only changes the RGB16. Thus, it can not match functions in Non_Simd
	blend_one_color_SSE(dst_rgb->r16, (uint16_t*)src_rgb->r16, alpha);
	blend_one_color_SSE(dst_rgb->g16, (uint16_t*)src_rgb->g16, alpha);
	blend_one_color_SSE(dst_rgb->b16, (uint16_t*)src_rgb->b16, alpha);
}
// End of alpha_blend.cpp