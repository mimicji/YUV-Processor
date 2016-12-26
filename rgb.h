//==============================================
//	Name: rgb.h
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/19/2016 19:01
//	All rights reserved. 
//==============================================

#include <cstdint>
#include <iostream>
#include <memory.h>
#ifdef RGB
#undef RGB
#endif
class RGB {
	// rgb888
public:
	int32_t width;
	int32_t height;
	uint8_t* r8;
	uint8_t* g8;
	uint8_t* b8;
	int16_t* r16;
	int16_t* g16;
	int16_t* b16;

	RGB(int32_t _width, int32_t _height) {
		width = _width;
		height = _height;
		r8 = new uint8_t[width * height];
		g8 = new uint8_t[width * height];
		b8 = new uint8_t[width * height];
		r16 = new int16_t[width * height];
		g16 = new int16_t[width * height];
		b16 = new int16_t[width * height];
		clean();
	}

	~RGB() {
		delete[] r8;
		delete[] g8;
		delete[] b8;
		delete[] r16;
		delete[] g16;
		delete[] b16;
	}

	void u8_to_s16() const;
	void s16_to_u8() const;
	void boundCheck() const;
	void clean() const;
	int32_t getWidth() const;
	int32_t getHeight() const;
	void write_bmp(FILE* fout) const;
};
// End of rgb.h
