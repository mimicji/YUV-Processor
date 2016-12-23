//==============================================
//	Name: yuv2rgb.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/23/2016 22:48
//	All rights reserved.
//==============================================

#include "process.h"

void Non_Simd::yuv2rgb(const RGB* dst_rgb, const YUV* src_yuv){
	int i, j, k;

	for (i = 0, k = 0; i < dst_rgb->getHeight(); ++i) {
		for (j = 0; j < dst_rgb->getWidth(); ++j, ++k) {
			int loc = (i / 2)*dst_rgb->getWidth()/2 + (j / 2);
			dst_rgb->r16[k] = (int16_t)(src_yuv->y8[k] - 16)*1.164383 + 2.017232 * (src_yuv->u8[loc] - 128);
			dst_rgb->g16[k] = (int16_t)(src_yuv->y8[k] - 16)*1.164383 - 0.391762 * (src_yuv->u8[loc] - 128) - 0.812968 * (src_yuv->v8[loc] - 128);
			dst_rgb->b16[k] = (int16_t)(src_yuv->y8[k] - 16)*1.164383 + 1.596027 * (src_yuv->v8[loc] - 128);
		}
	}
	
	dst_rgb->boundCheck();

	// To match the yuv2rgb() under namespace Non_Simd, we need to make a copy of data in uint8_t.
	dst_rgb->s16_to_u8();
}

namespace MMX {
	int16_t tmp_u[WIDTH*HEIGHT];
	int16_t tmp_v[WIDTH*HEIGHT];
	static const int16_t YUV_R[3] = { 0.164383 * (1 << 16),  0.017232 * (1 << 16),	       0 * (1 << 16) }; // offset: -1 -2 0
	static const int16_t YUV_G[3] = { 0.164383 * (1 << 16), -0.391762 * (1 << 16), -0.312968 * (1 << 16) }; // offset: -1 0 +0.5
	static const int16_t YUV_B[3] = { 0.164383 * (1 << 16),         0 * (1 << 16),  0.096027 * (1 << 16) }; // offset: -1 0 -1.5
	static const __m64 OFFSET_128 = _mm_set_pi16(128, 128, 128, 128);
	static const __m64 OFFSET_16 = _mm_set_pi16(16, 16, 16, 16);

}

void MMX::yuv2rgb(const RGB* dst_rgb, const YUV* src_yuv) {
	src_yuv->u8_to_s16();
	int i, j, k;
	__m64 tmp, tmp_data;
	for (i = 0, k = 0; i < dst_rgb->getHeight(); ++i) {
		for (j = 0; j < dst_rgb->getWidth(); ++j, ++k) {
			int loc = (i / 2)*dst_rgb->getWidth() / 2 + (j / 2);
			tmp_u[k] = src_yuv->u16[loc];
			tmp_v[k] = src_yuv->v16[loc];
		}
	}

	_mm_empty();

	// YUV to R Channel
	__m64* dst = (__m64*) dst_rgb->r16;
	__m64* src_y = (__m64*)src_yuv->y16;
	__m64* src_u = (__m64*)tmp_u;
	__m64* src_v = (__m64*)tmp_v;
	const __m64 Y_R = _mm_set_pi16(YUV_R[0], YUV_R[0], YUV_R[0], YUV_R[0]);
	const __m64 U_R = _mm_set_pi16(YUV_R[1], YUV_R[1], YUV_R[1], YUV_R[1]);
	const __m64 V_R = _mm_set_pi16(YUV_R[2], YUV_R[2], YUV_R[2], YUV_R[2]);
	for (i = 0; i < dst_rgb->height * dst_rgb->width / 4; i++) {
		// Y Channel to R Channel
		tmp_data = _m_psubw(*src_y, OFFSET_16); // (Y - 16)
		tmp = _m_pmulhw(tmp_data, Y_R);			// R = (Y - 16) * 0.164383
		*dst = _m_paddsw(tmp, tmp_data);		// R += Y - 16
		
		// U Channel to R Channel
		tmp_data = _m_psubw(*src_u, OFFSET_128);// (U - 128)
		tmp = _m_pmulhw(tmp_data, U_R);			// (U - 128) * 0.017232
		*dst = _m_paddsw(*dst, tmp);			// R += (U - 128) * 0.017232
		tmp = _m_psllwi(tmp_data, 1);
		*dst = _m_paddsw(*dst, tmp);			// R += (U - 128) << 1;

		// V Channel to R Channel


		// increase iterators
		dst++;
		src_y++;
		src_u++;
		src_v++;
	}
	// End of YUV to R Channel

	// YUV to G Channel
	dst = (__m64*) dst_rgb->g16;
	src_y = (__m64*)src_yuv->y16;
	src_u = (__m64*)tmp_u;
	src_v = (__m64*)tmp_v;
	const __m64 Y_G = _mm_set_pi16(YUV_G[0], YUV_G[0], YUV_G[0], YUV_G[0]);
	const __m64 U_G = _mm_set_pi16(YUV_G[1], YUV_G[1], YUV_G[1], YUV_G[1]);
	const __m64 V_G = _mm_set_pi16(YUV_G[2], YUV_G[2], YUV_G[2], YUV_G[2]);
	for (i = 0; i < dst_rgb->height * dst_rgb->width / 4; i++) {
		// Y Channel to G Channel
		tmp_data = _m_psubw(*src_y, OFFSET_16); // (Y - 16)
		tmp = _m_pmulhw(tmp_data, Y_G);			// G = (Y - 16) * 0.164383
		*dst = _m_paddsw(tmp, tmp_data);		// G += Y - 16

		// U Channel to G Channel
		tmp_data = _m_psubw(*src_u, OFFSET_128);// (U - 128)
		tmp = _m_pmulhw(tmp_data, U_G);			// (U - 128) * (-0.391762)
		*dst = _m_paddsw(*dst, tmp);			// G += (U - 128) * (-0.391762)

		// V Channel to R Channel
		tmp_data = _m_psubw(*src_v, OFFSET_128);// (V - 128)
		tmp = _m_pmulhw(tmp_data, V_G);			// (V - 128) * (-0.312968)
		*dst = _m_paddsw(*dst, tmp);			// G += (V - 128) * (-0.312968)
		tmp = _m_psrawi(tmp_data, 1);
		*dst = _m_psubsw(*dst, tmp);			// G -= (V - 128) >> 1;

		// increase iterators
		dst++;
		src_y++;
		src_u++;
		src_v++;
	}
	// End of YUV to G Channel

	// YUV to B Channel
	dst = (__m64*) dst_rgb->b16;
	src_y = (__m64*)src_yuv->y16;
	src_u = (__m64*)tmp_u;
	src_v = (__m64*)tmp_v;
	const __m64 Y_B = _mm_set_pi16(YUV_B[0], YUV_B[0], YUV_B[0], YUV_B[0]);
	const __m64 U_B = _mm_set_pi16(YUV_B[1], YUV_B[1], YUV_B[1], YUV_B[1]);
	const __m64 V_B = _mm_set_pi16(YUV_B[2], YUV_B[2], YUV_B[2], YUV_B[2]);
	for (i = 0; i < dst_rgb->height * dst_rgb->width / 4; i++) {
		// Y Channel to B Channel
		tmp_data = _m_psubw(*src_y, OFFSET_16); // (Y - 16)
		tmp = _m_pmulhw(tmp_data, Y_B);			// B = (Y - 16) * 0.164383
		*dst = _m_paddsw(tmp, tmp_data);		// B += Y - 16

		// U Channel to B Channel


		// V Channel to B Channel
		tmp_data = _m_psubw(*src_v, OFFSET_128);// (V - 128)
		tmp = _m_pmulhw(tmp_data, V_B);			// (V - 128) * 0.096027
		*dst = _m_paddsw(*dst, tmp);			// B += (V - 128) * 0.096027
		tmp = _m_psrawi(tmp_data, 1);
		*dst = _m_paddsw(*dst, tmp);			// G += (V - 128) >> 1;
		tmp = _m_psllwi(tmp_data, 1);
		*dst = _m_paddsw(*dst, tmp);			// G += (V - 128) << 1;
		
		// increase iterators
		dst++;
		src_y++;
		src_u++;
		src_v++;
	}
	// End of YUV to B Channel

	_mm_empty();

	dst_rgb->boundCheck();
	dst_rgb->s16_to_u8();
}

void AVX::yuv2rgb(const RGB* dst_rgb, const YUV* src_yuv) {
	// TODO
}

namespace SSE {
	int16_t tmp_u[WIDTH*HEIGHT];
	int16_t tmp_v[WIDTH*HEIGHT];
	static const int16_t YUV_R[3] = { 0.164383 * (1 << 16),  0.017232 * (1 << 16),	       0 * (1 << 16) }; // offset: -1 -2 0
	static const int16_t YUV_G[3] = { 0.164383 * (1 << 16), -0.391762 * (1 << 16), -0.312968 * (1 << 16) }; // offset: -1 0 +0.5
	static const int16_t YUV_B[3] = { 0.164383 * (1 << 16),         0 * (1 << 16),  0.096027 * (1 << 16) }; // offset: -1 0 -1.5
	static const __m128i OFFSET_128 = _mm_set_epi16(128, 128, 128, 128, 128, 128, 128, 128);
	static const __m128i OFFSET_16 = _mm_set_epi16(16, 16, 16, 16, 16, 16, 16, 16);

}

void SSE::yuv2rgb(const RGB* dst_rgb, const YUV* src_yuv) {
	src_yuv->u8_to_s16();
	int i, j, k;
	__m128i tmp, tmp_data;
	for (i = 0, k = 0; i < dst_rgb->getHeight(); ++i) {
		for (j = 0; j < dst_rgb->getWidth(); ++j, ++k) {
			int loc = (i / 2)*dst_rgb->getWidth() / 2 + (j / 2);
			tmp_u[k] = src_yuv->u16[loc];
			tmp_v[k] = src_yuv->v16[loc];
		}
	}

	_mm_empty();

	// YUV to R Channel
	__m128i* dst = (__m128i*) dst_rgb->r16;
	__m128i* src_y = (__m128i*)src_yuv->y16;
	__m128i* src_u = (__m128i*)tmp_u;
	__m128i* src_v = (__m128i*)tmp_v;
	const __m128i Y_R = _mm_set_epi16(YUV_R[0], YUV_R[0], YUV_R[0], YUV_R[0], YUV_R[0], YUV_R[0], YUV_R[0], YUV_R[0]);
	const __m128i U_R = _mm_set_epi16(YUV_R[1], YUV_R[1], YUV_R[1], YUV_R[1], YUV_R[1], YUV_R[1], YUV_R[1], YUV_R[1]);
	const __m128i V_R = _mm_set_epi16(YUV_R[2], YUV_R[2], YUV_R[2], YUV_R[2], YUV_R[2], YUV_R[2], YUV_R[2], YUV_R[2]);
	for (i = 0; i < dst_rgb->height * dst_rgb->width / 8; i++) {
		// Y Channel to R Channel
		tmp_data = _mm_subs_epi16(*src_y, OFFSET_16);	// (Y - 16)
		tmp = _mm_mulhi_epi16(tmp_data, Y_R);			// R = (Y - 16) * 0.164383
		*dst = _mm_adds_epi16(tmp, tmp_data);			// R += Y - 16

		// U Channel to R Channel
		tmp_data = _mm_subs_epi16(*src_u, OFFSET_128);	// (U - 128)
		tmp = _mm_mulhi_epi16(tmp_data, U_R);			// (U - 128) * 0.017232
		*dst = _mm_adds_epi16(*dst, tmp);				// R += (U - 128) * 0.017232
		tmp = _mm_slli_epi16(tmp_data, 1);
		*dst = _mm_adds_epi16(*dst, tmp);				// R += (U - 128) << 1;

		// V Channel to R Channel


		// increase iterators
		dst++;
		src_y++;
		src_u++;
		src_v++;
	}
	// End of YUV to R Channel

	// YUV to G Channel
	dst = (__m128i*) dst_rgb->g16;
	src_y = (__m128i*)src_yuv->y16;
	src_u = (__m128i*)tmp_u;
	src_v = (__m128i*)tmp_v;
	const __m128i Y_G = _mm_set_epi16(YUV_G[0], YUV_G[0], YUV_G[0], YUV_G[0], YUV_G[0], YUV_G[0], YUV_G[0], YUV_G[0]);
	const __m128i U_G = _mm_set_epi16(YUV_G[1], YUV_G[1], YUV_G[1], YUV_G[1], YUV_G[1], YUV_G[1], YUV_G[1], YUV_G[1]);
	const __m128i V_G = _mm_set_epi16(YUV_G[2], YUV_G[2], YUV_G[2], YUV_G[2], YUV_G[2], YUV_G[2], YUV_G[2], YUV_G[2]);
	for (i = 0; i < dst_rgb->height * dst_rgb->width / 8; i++) {
		// Y Channel to G Channel
		tmp_data = _mm_subs_epi16(*src_y, OFFSET_16);	// (Y - 16)
		tmp = _mm_mulhi_epi16(tmp_data, Y_G);			// G = (Y - 16) * 0.164383
		*dst = _mm_adds_epi16(tmp, tmp_data);			// G += Y - 16

		// U Channel to G Channel
		tmp_data = _mm_subs_epi16(*src_u, OFFSET_128);	// (U - 128)
		tmp = _mm_mulhi_epi16(tmp_data, U_G);			// (U - 128) * (-0.391762)
		*dst = _mm_adds_epi16(*dst, tmp);				// G += (U - 128) * (-0.391762)

		// V Channel to R Channel
		tmp_data = _mm_subs_epi16(*src_v, OFFSET_128);	// (V - 128)
		tmp = _mm_mulhi_epi16(tmp_data, V_G);			// (V - 128) * (-0.312968)
		*dst = _mm_adds_epi16(*dst, tmp);				// G += (V - 128) * (-0.312968)
		tmp = _mm_srai_epi16(tmp_data, 1);
		*dst = _mm_subs_epi16(*dst, tmp);				// G -= (V - 128) >> 1;

		// increase iterators
		dst++;
		src_y++;
		src_u++;
		src_v++;
	}
	// End of YUV to G Channel

	// YUV to B Channel
	dst = (__m128i*) dst_rgb->b16;
	src_y = (__m128i*)src_yuv->y16;
	src_u = (__m128i*)tmp_u;
	src_v = (__m128i*)tmp_v;
	const __m128i Y_B = _mm_set_epi16(YUV_B[0], YUV_B[0], YUV_B[0], YUV_B[0], YUV_B[0], YUV_B[0], YUV_B[0], YUV_B[0]);
	const __m128i U_B = _mm_set_epi16(YUV_B[1], YUV_B[1], YUV_B[1], YUV_B[1], YUV_B[1], YUV_B[1], YUV_B[1], YUV_B[1]);
	const __m128i V_B = _mm_set_epi16(YUV_B[2], YUV_B[2], YUV_B[2], YUV_B[2], YUV_B[2], YUV_B[2], YUV_B[2], YUV_B[2]);
	for (i = 0; i < dst_rgb->height * dst_rgb->width / 8; i++) {
		// Y Channel to B Channel
		tmp_data = _mm_subs_epi16(*src_y, OFFSET_16); // (Y - 16)
		tmp = _mm_mulhi_epi16(tmp_data, Y_B);			// B = (Y - 16) * 0.164383
		*dst = _mm_adds_epi16(tmp, tmp_data);		// B += Y - 16

		// U Channel to B Channel


		// V Channel to B Channel
		tmp_data = _mm_subs_epi16(*src_v, OFFSET_128);// (V - 128)
		tmp = _mm_mulhi_epi16(tmp_data, V_B);			// (V - 128) * 0.096027
		*dst = _mm_adds_epi16(*dst, tmp);			// B += (V - 128) * 0.096027
		tmp = _mm_srai_epi16(tmp_data, 1);
		*dst = _mm_adds_epi16(*dst, tmp);			// G += (V - 128) >> 1;
		tmp = _mm_slli_epi16(tmp_data, 1);
		*dst = _mm_adds_epi16(*dst, tmp);			// G += (V - 128) << 1;

												// increase iterators
		dst++;
		src_y++;
		src_u++;
		src_v++;
	}
	// End of YUV to B Channel

	_mm_empty();

	dst_rgb->boundCheck();
	dst_rgb->s16_to_u8();
}
// End of yuv2rgb.cpp