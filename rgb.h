//==============================================
//	Name: rgb.h
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/19/2016 19:01
//	All rights reserved. Only for Windows. 
//==============================================

#include <cstdint>
#include <iostream>
#ifdef RGB
#undef RGB
#endif
class RGB {
	// rgb888
public:
	int32_t width;
	int32_t height;
	uint8_t* r_ptr;
	uint8_t* g_ptr;
	uint8_t* b_ptr;
	int16_t* r16;
	int16_t* g16;
	int16_t* b16;

	RGB(int32_t _width, int32_t _height) {
		width = _width;
		height = _height;
		r_ptr = new uint8_t[width * height];
		g_ptr = new uint8_t[width * height];
		b_ptr = new uint8_t[width * height];
		r16 = new int16_t[width * height];
		g16 = new int16_t[width * height];
		b16 = new int16_t[width * height];
		clean();
	}

	~RGB() {
		delete[] r_ptr;
		delete[] g_ptr;
		delete[] b_ptr;
		delete[] r16;
		delete[] g16;
		delete[] b16;
	}

	void extend() const;
	void clean() const;
	int32_t getWidth() const;
	int32_t getHeight() const;
	void write_bmp(FILE* fout) const;
};
