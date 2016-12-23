//==============================================
//	Name: rgb.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/23/2016 22:48
//	All rights reserved.
//==============================================


#include "rgb.h"
#include "bmp.h"

_forceinline static inline uint8_t format(int16_t input) {
	if (input > 255) {
		return (uint8_t)255;
	}
	if (input < 0) {
		return (uint8_t)0;
	}
	return (uint8_t)input;
}

void RGB::u8_to_s16() const {
	for (int i = 0; i<width * height; i++) {
		this->r16[i] = (int16_t)this->r8[i];
		this->g16[i] = (int16_t)this->g8[i];
		this->b16[i] = (int16_t)this->b8[i];
	}
}

void RGB::s16_to_u8() const {
	for (int i = 0; i<width * height; i++) {
		this->r8[i] = format(this->r16[i]);
		this->g8[i] = format(this->g16[i]);
		this->b8[i] = format(this->b16[i]);
	}
}

void RGB::boundCheck() const {
	for (int i = 0; i<width * height; i++) {
		this->r16[i] = format(this->r16[i]);
		this->g16[i] = format(this->g16[i]);
		this->b16[i] = format(this->b16[i]);
	}
}

void RGB::clean() const {
	memset(r8, 0, width * height);
	memset(g8, 0, width * height);
	memset(b8, 0, width * height);
}

int32_t RGB::getWidth() const {
	return width;
};

int32_t RGB::getHeight() const {
	return height;
};

void RGB::write_bmp(FILE* fout) const {
	t_BMPfileheader file_header;
	t_BMPinfoheader info_header;
	uint8_t buf[3];
	int i, j;
	fwrite(&file_header, sizeof(t_BMPfileheader), 1, fout);
	fwrite(&info_header, sizeof(t_BMPinfoheader), 1, fout);
	for (i = height - 1; i >= 0; i--) {
		for (j = 0; j<width; j++) {
			buf[0] = r8[i*width + j];
			buf[1] = g8[i*width + j];
			buf[2] = b8[i*width + j];
			fwrite(buf, sizeof(uint8_t), 3, fout);
		}
	}
}
// End of rgb.cpp