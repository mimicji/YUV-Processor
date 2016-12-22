//==============================================
//	Name: rgb.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/19/2016 19:01
//	All rights reserved. Only for Windows. 
//==============================================


#include "rgb.h"
#include "bmp.h"

void RGB::extend() const {
	for (int i = 0; i<width * height; i++) {
		this->r16[i] = (uint16_t)this->r_ptr[i];
		this->g16[i] = (uint16_t)this->g_ptr[i];
		this->b16[i] = (uint16_t)this->b_ptr[i];
	}
}

void RGB::clean() const {
	memset(r_ptr, 0, width * height);
	memset(g_ptr, 0, width * height);
	memset(b_ptr, 0, width * height);
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
			buf[0] = r_ptr[i*width + j];
			buf[1] = g_ptr[i*width + j];
			buf[2] = b_ptr[i*width + j];
			fwrite(buf, sizeof(uint8_t), 3, fout);
		}
	}
}