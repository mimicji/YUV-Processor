//==============================================
//	Name: yuv.h
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/19/2016 19:01
//	All rights reserved. Only for Windows. 
//==============================================

#include <cstdint>
#include <iostream>
class RGB;
using std::cin;
using std::cout;
class YUV {
public:
	int32_t width;
	int32_t height;
	uint8_t* y_ptr;
	uint8_t* u_ptr;
	uint8_t* v_ptr;
	int16_t* y16;
	int16_t* u16;
	int16_t* v16;

	YUV(int32_t _width, int32_t _height) {
		width = _width;
		height = _height;
		y_ptr = new uint8_t[width * height];
		u_ptr = new uint8_t[width * height / 4];
		v_ptr = new uint8_t[width * height / 4];
		y16 = new int16_t[width * height];
		u16 = new int16_t[width * height / 4];
		v16 = new int16_t[width * height / 4];
		clean();
	}

	~YUV() {
		delete[] y_ptr;
		delete[] u_ptr;
		delete[] v_ptr;
		delete[] y16;
		delete[] u16;
		delete[] v16;
	}

	void u8_to_s16() const;
	void s16_to_u8() const;
	int read_file(const char* file_name) const;
	void write(FILE* fp) const;
	void clean() const;
	int32_t getWidth() const;
	int32_t getHeight() const;

};