//==============================================
//	Name: yuv.cpp
//	Description: Computer Architecture Lab4.1
//	Author: Kaihang JI
//	Last Edit: 12/19/2016 19:01
//	All rights reserved. Only for Windows. 
//==============================================


#include "yuv.h"

int YUV::read_file(const char* file_name) const {
	FILE * fp;
	if ((fp = fopen(file_name, "rb")) == NULL) return -1;
	char * buf = new char[width * height * 1.5];
	fread(buf, sizeof(char), width * height * 1.5, fp);
	fclose(fp);
	memcpy(y_ptr, buf, width * height * sizeof(char));
	memcpy(u_ptr, buf + width * height, width * height * sizeof(char) / 4);
	memcpy(v_ptr, buf + (width * height * 5)/4, width * height * sizeof(char) / 4);
	delete[] buf;
	return 0;
}

void YUV::write(FILE * fp) const {
	char * buf = new char[width * height * 1.5];
	memcpy(buf, y_ptr,  width * height * sizeof(char));
	memcpy(buf + width * height, u_ptr,  width * height * sizeof(char) / 4);
	memcpy(buf + (width * height * 5) / 4, v_ptr,  width * height * sizeof(char) / 4);
	fwrite(buf, sizeof(char), width * height * 1.5, fp);
	delete[] buf;
}

void YUV::clean() const {
	memset(y_ptr, 0, width * height);
	memset(u_ptr, 0, width * height / 4);
	memset(v_ptr, 0, width * height / 4);
}

int32_t YUV::getWidth() const {
	return width;
};

int32_t YUV::getHeight() const {
	return height;
};