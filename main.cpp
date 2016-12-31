//==============================================
//	Name: main.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/24/2016 01:18
//	All rights reserved.
//==============================================

#include "process.h"

/**
 * Select the instruction set. 
 * Choose a namespace from Non_Simd/MMX/AVX/SSE.
 *	Non_Simd : 
 *		Run the program with no special instruction set.
 *	MMX :
 *		Run the program with MMX. Certainly, it will be more
 *		efficient than Non_Simd. This part requires mmintrin.h.
 *	AVX :
 *		Run the program with AVX. Technically, it will be most
 *		efficient. This part requires immintrin.h.
 *      #Warning : it may cause core dumped in Linux. See readme
 *      for more information.
 *	SSE :
 *		Run the program with SSE2. Technically, it will be more
 *		efficient than MMX but less than AVX. This part requires
 *		emmintrin.h.
 */
using namespace Non_Simd;

/**
 * Choose the program function. If you want to alpha_blending,
 * then define ALPHA_BLENDING. Otherwise, undefine it.
 */
#define ALPHA_BLENDING

/**
 * Select your file path. 
 * INPUT_YUV_1 :
 *		When image_overlay, it is the path of the first input 
 *		YUV file. When alpha_blending, it is the path of the 
 *		input YUV file. 
 * INPUT_YUV_2 :
 *		When image_overlay, it is the path of the second input
 *		YUV file. When alpha_blending, it is will never be used.
 * OUTPUT_YUV :
 *		It is the path of the output YUV file.
 * OUTPUT_BMP :
 *		It is the path of the output BMP file. See BMP_OUT for
 *		in main.cpp and RGB::write_bmp in rgb.cpp for more detials.
 */
#define INPUT_YUV_1 "dem1.yuv"
#define INPUT_YUV_2 "dem2.yuv"
#define OUTPUT_YUV "out.yuv"
#define OUTPUT_BMP "out.bmp"

// Do not modify this part.
#ifndef ALPHA_BLENDING
	#define IMAGE_OVERLAY
#else
	#undef IMAGE_OVERLAY
#endif

// Output a BMP from RGB(u8) for debugging.
#define BMP_OUT(path, rgb)	do{FILE* bmpout = fopen(path, "wb"); \
							rgb->write_bmp(bmpout); \
							fclose(bmpout);} while(false)


int main() {
	FILE* fout;
	YUV* input_yuv = new YUV(WIDTH, HEIGHT);
	YUV* dst_yuv = new YUV(WIDTH, HEIGHT);
	RGB* tmp_rgb = new RGB(WIDTH,HEIGHT);
	RGB* dst_rgb = new RGB(WIDTH, HEIGHT);
#ifdef	IMAGE_OVERLAY
	YUV* input_yuv2 = new YUV(WIDTH, HEIGHT);
	RGB* tmp_rgb2 = new RGB(WIDTH, HEIGHT);
#endif

	clock_t begin_time = clock();
	clock_t total_time = 0;
	// Open INPUT_YUV_1 and convert it into rgb.
	if (input_yuv->read_file(INPUT_YUV_1) == -1) {
		cerr << "INPUT FILE ERROR" << endl;
        #ifdef WIN32
		system("pause");
        #endif
		return 0;
	}
	else {
		// Record the begin time.
		clock_t recuit_time = clock();

		// Call YUV to RGB
		yuv2rgb(tmp_rgb, input_yuv);

		// Calculate the running time.
		total_time += clock() - recuit_time;
	}

	// Open INPUT_YUV_2 and convert it into rgb.
	#ifdef	IMAGE_OVERLAY
	if (input_yuv2->read_file(INPUT_YUV_2) == -1) {
		cerr << "INPUT FILE ERROR." << endl;
		#ifdef WIN32
		system("pause");
        	#endif
		return 0;
	}
	else {
		// Record the begin time.
		clock_t recuit_time = clock();

		// Call YUV to RGB
		yuv2rgb(tmp_rgb2, input_yuv2);

		// Calculate the running time.
		total_time += clock() - recuit_time;
	}
	#endif

	// Prepare for output YUV.
	if ((fout = fopen(OUTPUT_YUV, "wb")) == NULL) {
		cerr << "FAIL TO WRITE THE OUTPUT FILE." << endl;
		system("pause");
		return 0;
	}

	// Output a BMP file from INPUT_YUV_1.toRGB() to see if YUV::yuv2rgb() works correctly.
	BMP_OUT(OUTPUT_BMP, tmp_rgb);

	// Main part.
	int alpha;
	for (alpha = 1; alpha < 256; alpha += 3) {
		// Record the begin time.
		clock_t recuit_time = clock();

		// Alpha blending.
		#ifdef	ALPHA_BLENDING
			alpha_blend(dst_rgb, tmp_rgb, alpha);
		#endif

		// Image overlay.
		#ifdef  IMAGE_OVERLAY
			#ifdef ALPHA_BLENDING
				#error Both of IMAGE_OVERLAY and ALPHA_BLENDING have been defined.
			#endif
			image_overlay(dst_rgb, tmp_rgb, tmp_rgb2, alpha);
		#endif
		
		// Convert it into yuv for output.
		rgb2yuv(dst_yuv, dst_rgb);

		// Calculate the running time.
		total_time += clock() - recuit_time;

		// Output one frame.
		dst_yuv->write(fout);

		// Reset dst_rgb and dst_yuv.
		dst_rgb->clean();
		dst_yuv->clean();
	}

	// Close output file.
	fclose(fout);

	// Print run time info on the screen.
	#ifdef WIN32
	cerr << "Core function time: " << total_time << "ms" << endl;
	cerr << "Total run time: " << clock() - begin_time << "ms" << endl;
	system("pause");
    	#else
	// In Linux, clock() return in us instead of ms.
	cerr << "Core function time: " << total_time/1000 << "ms" << endl;
	cerr << "Total run time: " << (clock() - begin_time)/1000 << "ms" << endl;
    	#endif

	// Nomarlly exit.
	return 0;
}
// End of main.cpp
