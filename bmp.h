//===============================================================
//	Name: bmp.h
//	Description: File header for BMP24-1920*1080(Lab4.1)
//	Author: Kaihang JI
//	Last Edit: 12/19/2016 19:01
//	All rights reserved. 
//================================================================

/*********** *********** *********** *********** *********** *********** ***********
* definition :struct
* Description :位图文件头
*********** *********** *********** *********** *********** *********** ***********/
typedef unsigned short int WORD;
typedef unsigned int DWORD;
typedef int LONG;
typedef unsigned char BYTE;

#pragma pack(1)/////////////////将结构体中成员按n字节对齐

typedef struct tagBITMAPFILEHEADER
{
	WORD bfType = 19778;////////////////文件类型，必须为BM
	DWORD bfSize = 6220854;///////////////指定文件大小，以字节为单位（3-6字节，低位在前）
	WORD bfReserved1 = 0;///////////文件保留字，必须为0
	WORD bfReserved2 = 0;///////////文件保留字，必须为0
	DWORD bfOffBits = 54;////////////从文件头到实际位图数据的偏移字节数（11-14字节，低位在前）
}t_BMPfileheader;
/*********** *********** *********** *********** *********** *********** ***********
* definition :struct
* Description :位图信息头
*********** *********** *********** *********** *********** *********** ***********/
typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize = 40;///////////////本结构所占用字节数，为40。注意：实际操作中则有44，这是字节补齐的原因
	LONG biWidth = 1920;///////////////位图的宽度，以像素为单位
	LONG biHeight = 1080;//////////////位图的高度，以像素为单位
	WORD biPlanes = 1;//////////////目标设备的级别，必须为1
	WORD biBitCount = 24;////////////每个像素所需的位数，1（双色），4(16色），8(256色）16(高彩色)，24（真彩色）或32之一
	DWORD biCompression = 0;////////位图压缩类型，0（不压缩），1(BI_RLE8压缩类型）或2(BI_RLE4压缩类型）之一
	DWORD biSizeImage = 6220800;//////////位图的大小(其中包含了为了补齐行数是4的倍数而添加的空字节)，以字节为单位
	LONG biXPelsPerMeter = 2835;///////位图水平分辨率，每米像素数
	LONG biYPelsPerMeter = 2835;///////位图垂直分辨率，每米像素数
	DWORD biClrUsed = 0;////////////位图实际使用的颜色表中的颜色数，若该值为0,则使用颜色数为2的biBitCount次方
	DWORD biClrImportant = 0;///////位图显示过程中重要的颜色数，若该值为0,则所有的颜色都重要
}t_BMPinfoheader;
#pragma pack()//////////////////取消自定义字节方式