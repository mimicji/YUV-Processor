//==============================================
//	Name: yuv.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/23/2016 22:48
//	All rights reserved.
//==============================================


#include "yuv.h"

void YUV::u8_to_s16() const {
	for (int i = 0; i < width * height; i++) {
		this->y16[i] = (int16_t)this->y8[i];
	}
	for (int i = 0; i < width * height / 4; i++) {
		this->u16[i] = (int16_t)this->u8[i];
		this->v16[i] = (int16_t)this->v8[i];
	}
}

_forceinline static inline uint8_t format(int16_t input) {
	if (input > 255) {
		return (uint8_t)255;
	}
	if (input < 0) {
		return (uint8_t)0;
	}
	return (uint8_t)input;
}

void YUV::s16_to_u8() const {
	for (int i = 0; i < width * height; i++) {
		this->y8[i] = format(this->y16[i]);
	}
	for (int i = 0; i < width * height / 4; i++) {
		this->u8[i] = format(this->u16[i]);
		this->v8[i] = format(this->v16[i]);
	}
}

int YUV::read_file(const char* file_name) const {
	FILE * fp;
	if ((fp = fopen(file_name, "rb")) == NULL) return -1;
	char * buf = new char[width * height * 1.5];
	fread(buf, sizeof(char), width * height * 1.5, fp);
	fclose(fp);
	memcpy(y8, buf, width * height * sizeof(char));
	memcpy(u8, buf + width * height, width * height * sizeof(char) / 4);
	memcpy(v8, buf + (width * height * 5)/4, width * height * sizeof(char) / 4);
	delete[] buf;
	return 0;
}

void YUV::write(FILE * fp) const{
	char * buf = new char[width * height * 1.5];
	memcpy(buf, y8,  width * height * sizeof(char));
	memcpy(buf + width * height, u8,  width * height * sizeof(char) / 4);
	memcpy(buf + (width * height * 5) / 4, v8,  width * height * sizeof(char) / 4);
	fwrite(buf, sizeof(char), width * height * 1.5, fp);
	delete[] buf;
}

void YUV::clean() const {
	memset(y8, 0, width * height);
	memset(u8, 0, width * height / 4);
	memset(v8, 0, width * height / 4);
}

int32_t YUV::getWidth() const {
	return width;
};

int32_t YUV::getHeight() const {
	return height;
};
// End of yuv.cpp