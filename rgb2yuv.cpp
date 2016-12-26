//==============================================
//	Name: rgb2yuv.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/24/2016 01:18
//	All rights reserved.
//==============================================


#include "process.h"
namespace{
inline uint8_t format(int16_t input) {
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
			dst_yuv->y8[i] = format(0.256788 * src_rgb->r8[i] + 0.504129 * src_rgb->g8[i] + 0.097906 * src_rgb->b8[i] +16);
	}
	int j, k=0;
	for (i = 0; i < dst_yuv->height; i+=2) {
		for (j = 0; j < dst_yuv->width; j+=2) {
			dst_yuv->v8[k] = format(-0.148223 * src_rgb->r8[i * dst_yuv->width + j] - 0.290993 * src_rgb->g8[i * dst_yuv->width + j] + 0.439216 * src_rgb->b8[i * dst_yuv->width + j] + 128);
			dst_yuv->u8[k] = format( 0.439216 * src_rgb->r8[i * dst_yuv->width + j] - 0.367788 * src_rgb->g8[i * dst_yuv->width + j] - 0.071427 * src_rgb->b8[i * dst_yuv->width + j] + 128);
			++k;
		}
	}
}

namespace MMX {
	int16_t tmp_r[WIDTH * HEIGHT / 4];
	int16_t tmp_g[WIDTH * HEIGHT / 4];
	int16_t tmp_b[WIDTH * HEIGHT / 4];
	static const int16_t RGB_Y[3] = { 0.256788 * (1 << 16),  0.004129 * (1 << 16),  0.097906 * (1 << 16) }; // offset: 0 -0.5 0
	static const int16_t RGB_U[3] = { 0.439216 * (1 << 16), -0.367788 * (1 << 16), -0.071427 * (1 << 16) }; // offset: 0 0 0
	static const int16_t RGB_V[3] = { -0.148223 * (1 << 16), -0.290993 * (1 << 16),  0.439216 * (1 << 16) }; // offset: 0 0 0
	static const __m64 OFFSET_128 = _mm_set_pi16(128, 128, 128, 128);
	static const __m64 OFFSET_16 = _mm_set_pi16(16, 16, 16, 16);
}

void MMX::rgb2yuv(const YUV* dst_yuv, const RGB* src_rgb) {

	int i, j, k;
	__m64 tmp;


	_mm_empty();

	// RGB to Y Channel
	__m64* dst = (__m64*) dst_yuv->y16;
	__m64* src_r = (__m64*)src_rgb->r16;
	__m64* src_g = (__m64*)src_rgb->g16;
	__m64* src_b = (__m64*)src_rgb->b16;
	const __m64 R_Y = _mm_set_pi16(RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0]);
	const __m64 G_Y = _mm_set_pi16(RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1]);
	const __m64 B_Y = _mm_set_pi16(RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2]);
	for (i = 0; i < dst_yuv->height * dst_yuv->width / 4; i++) {
		// R Channel to Y Channel
		*dst = _m_pmulhw(*src_r, R_Y); // Y = R * 0.256788
		
		// G Channel to Y Channel
		tmp = _m_pmulhw(*src_g, G_Y);
		*dst = _m_paddsw(tmp, *dst); // Y += G * 0.004129
		tmp = _m_psrlwi(*src_g, 1);
		*dst = _m_paddsw(tmp, *dst); // Y += G >> 1;

		// B Channel to Y Channel
		tmp = _m_pmulhw(*src_b, B_Y);
		*dst = _m_paddsw(tmp, *dst); // Y += B * 0.097906

		// Add offset
		*dst = _m_paddsw(*dst, OFFSET_16); // Y += 16

		// increase iterators
		dst++;
		src_r++;
		src_g++;
		src_b++;
	}
	// End of RGB to Y Channel
	


	for (i = 0, k = 0; i < dst_yuv->height; i += 2) {
		for (j = 0; j < dst_yuv->width; j += 2, ++k) {
			tmp_r[k] = src_rgb->r16[i * dst_yuv->width + j];
			tmp_g[k] = src_rgb->g16[i * dst_yuv->width + j];
			tmp_b[k] = src_rgb->b16[i * dst_yuv->width + j];
		}
	}

	// RGB to U Channel
	dst = (__m64*) dst_yuv->u16;
	src_r = (__m64*) tmp_r;
	src_g = (__m64*) tmp_g;
	src_b = (__m64*) tmp_b;
	const __m64 R_U = _mm_set_pi16(RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0]);
	const __m64 G_U = _mm_set_pi16(RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1]);
	const __m64 B_U = _mm_set_pi16(RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2]);
	for (i = 0; i < WIDTH * HEIGHT / 16; i++) {
		// R Channel to U Channel
		*dst = _m_pmulhw(*src_r, R_U); // U = R * 0.439216

		// G Channel to U Channel
		tmp = _m_pmulhw(*src_g, G_U);
		*dst = _m_paddsw(tmp, *dst); // U += G * (-0.367788)

		// B Channel to U Channel
		tmp = _m_pmulhw(*src_b, B_U);
		*dst = _m_paddsw(tmp, *dst); // U += B * (-0.071427)

		// Add offset
		*dst = _m_paddsw(*dst, OFFSET_128); // U += 128

		// increase iterators
		dst++;
		src_r++;
		src_g++;
		src_b++;
	}
	// End of RGB to U Channel

	// RGB to V Channel
	dst = (__m64*) dst_yuv->v16;
	src_r = (__m64*) tmp_r;
	src_g = (__m64*) tmp_g;
	src_b = (__m64*) tmp_b;
	const __m64 R_V = _mm_set_pi16(RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0]);
	const __m64 G_V = _mm_set_pi16(RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1]);
	const __m64 B_V = _mm_set_pi16(RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2]);
	for (i = 0; i < WIDTH * HEIGHT / 16; i++) {
		// R Channel to V Channel
		*dst = _m_pmulhw(*src_r, R_V); // V = R * (-0.148223)

		// G Channel to V Channel
		tmp = _m_pmulhw(*src_g, G_V);
		*dst = _m_paddsw(tmp, *dst); // V += G * (-0.290993)

		 // B Channel to V Channel
		tmp = _m_pmulhw(*src_b, B_V);
		*dst = _m_paddsw(tmp, *dst); // V += B * (0.439216)

		// Add offset
		*dst = _m_paddsw(*dst, OFFSET_128); // V += 128

		// increase iterators
		dst++;
		src_r++;
		src_g++;
		src_b++;
	}
	// End of RGB to V Channel

	_mm_empty();
	dst_yuv->s16_to_u8();
	// End of function
}

namespace AVX {
	int16_t tmp_r[WIDTH * HEIGHT / 4];
	int16_t tmp_g[WIDTH * HEIGHT / 4];
	int16_t tmp_b[WIDTH * HEIGHT / 4];
	static const int16_t RGB_Y[3] = { 0.256788 * (1 << 16),  0.004129 * (1 << 16),  0.097906 * (1 << 16) }; // offset: 0 -0.5 0
	static const int16_t RGB_U[3] = { 0.439216 * (1 << 16), -0.367788 * (1 << 16), -0.071427 * (1 << 16) }; // offset: 0 0 0
	static const int16_t RGB_V[3] = { -0.148223 * (1 << 16), -0.290993 * (1 << 16),  0.439216 * (1 << 16) }; // offset: 0 0 0
	static const __m256i OFFSET_128 = _mm256_set_epi16(128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128);
	static const __m256i OFFSET_16 = _mm256_set_epi16(16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16);
}

void AVX::rgb2yuv(const YUV* dst_yuv, const RGB* src_rgb) {
	int i, j, k;
	__m256i tmp;


	_mm_empty();

	// RGB to Y Channel
	__m256i* dst = (__m256i*) dst_yuv->y16;
	__m256i* src_r = (__m256i*)src_rgb->r16;
	__m256i* src_g = (__m256i*)src_rgb->g16;
	__m256i* src_b = (__m256i*)src_rgb->b16;
	const __m256i R_Y = _mm256_set_epi16(RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0]);
	const __m256i G_Y = _mm256_set_epi16(RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1]);
	const __m256i B_Y = _mm256_set_epi16(RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2]);
	for (i = 0; i < dst_yuv->height * dst_yuv->width / 16; i++) {
		// R Channel to Y Channel
		*dst = _mm256_mulhi_epi16(*src_r, R_Y); // Y = R * 0.256788

		// G Channel to Y Channel
		tmp = _mm256_mulhi_epi16(*src_g, G_Y);
		*dst = _mm256_adds_epi16(tmp, *dst);	// Y += G * 0.004129
		tmp = _mm256_srli_epi16(*src_g, 1);
		*dst = _mm256_adds_epi16(tmp, *dst);	// Y += G >> 1;

		// B Channel to Y Channel
		tmp = _mm256_mulhi_epi16(*src_b, B_Y);
		*dst = _mm256_adds_epi16(tmp, *dst);	// Y += B * 0.097906

		// Add offset
		*dst = _mm256_adds_epi16(*dst, OFFSET_16); // Y += 16

		// increase iterators
		dst++;
		src_r++;
		src_g++;
		src_b++;
	}
	// End of RGB to Y Channel



	for (i = 0, k = 0; i < dst_yuv->height; i += 2) {
		for (j = 0; j < dst_yuv->width; j += 2, ++k) {
			tmp_r[k] = src_rgb->r16[i * dst_yuv->width + j];
			tmp_g[k] = src_rgb->g16[i * dst_yuv->width + j];
			tmp_b[k] = src_rgb->b16[i * dst_yuv->width + j];
		}
	}

	// RGB to U Channel
	dst = (__m256i*) dst_yuv->u16;
	src_r = (__m256i*) tmp_r;
	src_g = (__m256i*) tmp_g;
	src_b = (__m256i*) tmp_b;
	const __m256i R_U = _mm256_set_epi16(RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0]);
	const __m256i G_U = _mm256_set_epi16(RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1]);
	const __m256i B_U = _mm256_set_epi16(RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2]);
	for (i = 0; i < WIDTH * HEIGHT / 64; i++) {
		// R Channel to U Channel
		*dst = _mm256_mulhi_epi16(*src_r, R_U);		// U = R * 0.439216

		// G Channel to U Channel
		tmp = _mm256_mulhi_epi16(*src_g, G_U);
		*dst = _mm256_adds_epi16(tmp, *dst);		// U += G * (-0.367788)

		// B Channel to U Channel
		tmp = _mm256_mulhi_epi16(*src_b, B_U);
		*dst = _mm256_adds_epi16(tmp, *dst);		// U += B * (-0.071427)

		// Add offset
		*dst = _mm256_adds_epi16(*dst, OFFSET_128); // U += 128

		// increase iterators
		dst++;
		src_r++;
		src_g++;
		src_b++;
	}
	// End of RGB to U Channel

	// RGB to V Channel
	dst = (__m256i*) dst_yuv->v16;
	src_r = (__m256i*) tmp_r;
	src_g = (__m256i*) tmp_g;
	src_b = (__m256i*) tmp_b;
	const __m256i R_V = _mm256_set_epi16(RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0]);
	const __m256i G_V = _mm256_set_epi16(RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1]);
	const __m256i B_V = _mm256_set_epi16(RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2]);
	for (i = 0; i < WIDTH * HEIGHT / 64; i++) {
		// R Channel to V Channel
		*dst = _mm256_mulhi_epi16(*src_r, R_V); // V = R * (-0.148223)

		// G Channel to V Channel
		tmp = _mm256_mulhi_epi16(*src_g, G_V);
		*dst = _mm256_adds_epi16(tmp, *dst); // V += G * (-0.290993)

		// B Channel to V Channel
		tmp = _mm256_mulhi_epi16(*src_b, B_V);
		*dst = _mm256_adds_epi16(tmp, *dst); // V += B * (0.439216)

		// Add offset
		*dst = _mm256_adds_epi16(*dst, OFFSET_128); // V += 128

		// increase iterators
		dst++;
		src_r++;
		src_g++;
		src_b++;
	}
	// End of RGB to V Channel

	_mm_empty();
	dst_yuv->s16_to_u8();
	// End of function
}

namespace SSE {
	int16_t tmp_r[WIDTH * HEIGHT / 4];
	int16_t tmp_g[WIDTH * HEIGHT / 4];
	int16_t tmp_b[WIDTH * HEIGHT / 4];
	static const int16_t RGB_Y[3] = { 0.256788 * (1 << 16),  0.004129 * (1 << 16),  0.097906 * (1 << 16) }; // offset: 0 -0.5 0
	static const int16_t RGB_U[3] = { 0.439216 * (1 << 16), -0.367788 * (1 << 16), -0.071427 * (1 << 16) }; // offset: 0 0 0
	static const int16_t RGB_V[3] = { -0.148223 * (1 << 16), -0.290993 * (1 << 16),  0.439216 * (1 << 16) }; // offset: 0 0 0
	static const __m128i OFFSET_128 = _mm_set_epi16(128, 128, 128, 128, 128, 128, 128, 128);
	static const __m128i OFFSET_16 = _mm_set_epi16(16, 16, 16, 16, 16, 16, 16, 16);
}

void SSE::rgb2yuv(const YUV* dst_yuv, const RGB* src_rgb) {

	int i, j, k;
	__m128i tmp;


	_mm_empty();

	// RGB to Y Channel
	__m128i* dst = (__m128i*) dst_yuv->y16;
	__m128i* src_r = (__m128i*)src_rgb->r16;
	__m128i* src_g = (__m128i*)src_rgb->g16;
	__m128i* src_b = (__m128i*)src_rgb->b16;
	const __m128i R_Y = _mm_set_epi16(RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0], RGB_Y[0]);
	const __m128i G_Y = _mm_set_epi16(RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1], RGB_Y[1]);
	const __m128i B_Y = _mm_set_epi16(RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2], RGB_Y[2]);
	for (i = 0; i < dst_yuv->height * dst_yuv->width / 8; i++) {
		// R Channel to Y Channel
		*dst = _mm_mulhi_epi16(*src_r, R_Y); // Y = R * 0.256788

		// G Channel to Y Channel
		tmp = _mm_mulhi_epi16(*src_g, G_Y);
		*dst = _mm_adds_epi16(tmp, *dst); // Y += G * 0.004129
		tmp = _mm_srli_epi16(*src_g, 1);
		*dst = _mm_adds_epi16(tmp, *dst); // Y += G >> 1;

		// B Channel to Y Channel
		tmp = _mm_mulhi_epi16(*src_b, B_Y);
		*dst = _mm_adds_epi16(tmp, *dst); // Y += B * 0.097906

		// Add offset
		*dst = _mm_adds_epi16(*dst, OFFSET_16); // Y += 16

		 // increase iterators
		dst++;
		src_r++;
		src_g++;
		src_b++;
	}
	// End of RGB to Y Channel



	for (i = 0, k = 0; i < dst_yuv->height; i += 2) {
		for (j = 0; j < dst_yuv->width; j += 2, ++k) {
			tmp_r[k] = src_rgb->r16[i * dst_yuv->width + j];
			tmp_g[k] = src_rgb->g16[i * dst_yuv->width + j];
			tmp_b[k] = src_rgb->b16[i * dst_yuv->width + j];
		}
	}

	// RGB to U Channel
	dst = (__m128i*) dst_yuv->u16;
	src_r = (__m128i*) tmp_r;
	src_g = (__m128i*) tmp_g;
	src_b = (__m128i*) tmp_b;
	const __m128i R_U = _mm_set_epi16(RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0], RGB_U[0]);
	const __m128i G_U = _mm_set_epi16(RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1], RGB_U[1]);
	const __m128i B_U = _mm_set_epi16(RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2], RGB_U[2]);
	for (i = 0; i < WIDTH * HEIGHT / 32; i++) {
		// R Channel to U Channel
		*dst = _mm_mulhi_epi16(*src_r, R_U); // U = R * 0.439216

		// G Channel to U Channel
		tmp = _mm_mulhi_epi16(*src_g, G_U);
		*dst = _mm_adds_epi16(tmp, *dst); // U += G * (-0.367788)

		// B Channel to U Channel
		tmp = _mm_mulhi_epi16(*src_b, B_U);
		*dst = _mm_adds_epi16(tmp, *dst); // U += B * (-0.071427)

		// Add offset
		*dst = _mm_adds_epi16(*dst, OFFSET_128); // U += 128

		// increase iterators
		dst++;
		src_r++;
		src_g++;
		src_b++;
	}
	// End of RGB to U Channel

	// RGB to V Channel
	dst = (__m128i*) dst_yuv->v16;
	src_r = (__m128i*) tmp_r;
	src_g = (__m128i*) tmp_g;
	src_b = (__m128i*) tmp_b;
	const __m128i R_V = _mm_set_epi16(RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0], RGB_V[0]);
	const __m128i G_V = _mm_set_epi16(RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1], RGB_V[1]);
	const __m128i B_V = _mm_set_epi16(RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2], RGB_V[2]);
	for (i = 0; i < WIDTH * HEIGHT / 32; i++) {
		// R Channel to V Channel
		*dst = _mm_mulhi_epi16(*src_r, R_V); // V = R * (-0.148223)

		// G Channel to V Channel
		tmp = _mm_mulhi_epi16(*src_g, G_V);
		*dst = _mm_adds_epi16(tmp, *dst); // V += G * (-0.290993)

		// B Channel to V Channel
		tmp = _mm_mulhi_epi16(*src_b, B_V);
		*dst = _mm_adds_epi16(tmp, *dst); // V += B * (0.439216)

		// Add offset
		*dst = _mm_adds_epi16(*dst, OFFSET_128); // V += 128

		// increase iterators
		dst++;
		src_r++;
		src_g++;
		src_b++;
	}
	// End of RGB to V Channel

	_mm_empty();
	dst_yuv->s16_to_u8();
	// End of function
}
// End of rgb2yuv.cpp
