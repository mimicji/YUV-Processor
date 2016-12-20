//==============================================
//	Name: main.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/19/2016 19:01
//	All rights reserved. Only for Windows. 
//==============================================

#include <windows.h>
#include <time.h>
#include <fstream>
#include "process.h"

#define INPUT_YUV_1 "D:\\dem1.yuv"
#define INPUT_YUV_2 "D:\\dem2.yuv"
#define OUTPUT_YUV "D:\\out.yuv"
#define WIDTH 1920
#define HEIGHT 1080

using std::cerr;
using std::endl;


uint8_t* dst_ptr = NULL;
int32_t width = 0;
int32_t height = 0;

using namespace Non_Simd;
// To choose an Instruction Set, choose a namespace from Non_Simd/MMX/AVX/SSE.

int main() {
	YUV* input_yuv = new YUV(WIDTH, HEIGHT);
	YUV* input_yuv2 = new YUV(WIDTH, HEIGHT);
	YUV* dst_yuv = new YUV(WIDTH, HEIGHT);
	RGB* tmp_rgb = new RGB(WIDTH,HEIGHT);
	RGB* tmp_rgb2 = new RGB(WIDTH, HEIGHT);
	RGB* dst_rgb = new RGB(WIDTH, HEIGHT);
	clock_t begin_time = clock();
	clock_t total_time = 0;


	if (input_yuv->read_file(INPUT_YUV_1) == -1) {
		cerr << "INPUT FILE ERROR" << endl;
		system("pause");
		return 0;
	}
	if (input_yuv2->read_file(INPUT_YUV_2) == -1) {
		cerr << "INPUT FILE ERROR" << endl;
		system("pause");
		return 0;
	}
	FILE* fout = fopen(OUTPUT_YUV, "wb");

	yuv2rgb(tmp_rgb, input_yuv);
	yuv2rgb(tmp_rgb2, input_yuv2);
	FILE* bmpout = fopen("D:\\out.bmp", "wb");
	tmp_rgb->write_bmp(bmpout);
	fclose(bmpout);

	//rgb2yuv(dst_yuv, tmp_rgb);
	//dst_yuv->write(fout);

	//freopen("D:\\test.txt", "w", stdout);
	//for(int i=0;i<dst_yuv->width*dst_yuv->height/4;i++) {
	//	cout << (int16_t)dst_yuv->u_ptr[i] - input_yuv->u_ptr[i] << ' ';
	//	if (i % 16 == 15)
	//		cout << endl;
	//}
	//fclose(stdout);

	int alpha;
	for (alpha = 1; alpha < 256; alpha += 3) {
		clock_t recuit_time = clock();
		//alpha_blend(dst_rgb, tmp_rgb, alpha);
		image_overlay(dst_rgb, tmp_rgb, tmp_rgb2, alpha);
		rgb2yuv(dst_yuv, dst_rgb);
		total_time += clock() - recuit_time;
		dst_yuv->write(fout);
		dst_rgb->clean();
		dst_yuv->clean();
		//cerr << "Alpha: " << alpha << endl;
	}
	fclose(fout);
	cerr << "Calcular time: " << total_time << "ms" << endl;
	cerr << "Total run time: " << clock() - begin_time << "ms" << endl;
	system("pause");
	return 0;
}
