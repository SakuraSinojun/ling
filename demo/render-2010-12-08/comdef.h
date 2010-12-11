


#pragma once

#ifndef __COMDEF_H__
#define __COMDEF_H__

#ifndef _WIN32
typedef unsigned long		DWORD;
typedef int			BOOL;
typedef unsigned char		BYTE;
typedef unsigned short	WORD;
typedef float			FLOAT;
typedef long			LONG;
#else
#include <windows.h>
#endif



#pragma pack(push)
#pragma pack(1)

typedef struct
{
	BYTE		identsize;              // Size of ID field that follows header (0)
	BYTE		colorMapType;           // 0 = None, 1 = paletted
	BYTE		imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;          // First colour map entry
	unsigned short	colorMapLength;         // Number of colors
	unsigned char 	colorMapBits;   	// bits per palette entry
	unsigned short	xstart;                 // image x origin
	unsigned short	ystart;                 // image y origin
	unsigned short	width;                  // width in pixels
	unsigned short	height;                 // height in pixels
	BYTE		bits;                   // bits per pixel (8 16, 24, 32)
	BYTE		descriptor;             // image descriptor
} TGAHEADER;

typedef struct
{
	WORD	bfType;
	DWORD	bfSize;
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits;
} BMPHEADER;

typedef struct 
{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BMPINFOHEADER;

typedef struct 
{
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
}RGBQ;


#pragma pack(pop)



#endif

