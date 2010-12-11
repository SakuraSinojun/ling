
/*******************************************************************************

        Ahthor: 准
        Tab:    8

*******************************************************************************/


#include "common.h"
#include <stdio.h>
#include <malloc.h>

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
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
}RGBQ;

#pragma pack(pop)




typedef enum 
{
	PIC_BMP,
	PIC_TGA,
	PIC_PNG,
	PIC_JPG,
	PIC_GIF,
}PIC_TYPE;

static void _parse_file(const char * filename, BYTE ** buffer, unsigned int len);
static void _parse_to_bmp(const char * filename, PIC_TYPE type, BYTE ** buffer, unsigned int len);



BYTE * _read_data_file(const char * filename, unsigned int & len)
{
	FILE	*	fp;
	BYTE	*	buffer;
	
	if(filename == NULL)
	{	
		len = -1;
		return NULL;
	}
	
	fp = fopen(filename, "rb");
	if( fp == NULL )
	{
		len = -1;
		return NULL;
	}
	
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	if(len == 0)
	{
		fclose(fp);
		return NULL;
	}
	
	buffer = (BYTE *)malloc(len);
	fseek(fp, 0, SEEK_SET);
	if( fread(buffer, len, 1, fp) != 1 )
	{
		common::Error("读取文件失败:[ %s ]", filename);
		fclose(fp);
		return NULL;
	}
	
	fclose(fp);
	
	_parse_file(filename, &buffer, len);

	return buffer; 
	
}

static void _parse_file(const char * filename, BYTE ** buffer, unsigned int len)
{
	int		i;
	unsigned int	l;
	char 		ext[100];
	
	l = strlen(filename);
	
	for(i=l-1; i>=0; i--)
	{
		if( filename[i] == '.' )
		{
			strcpy(ext, filename + i + 1);
			break;
		}
	}
	
	if(i == -1)
	{
		return;
	}
	
	//转化成小写
	for(i=0; i<(int)strlen(ext); i++)
	{	
		ext[i] = ext[i] | 0x20;
	}
	
	if(strcmp(ext, "bmp") == 0)
	{
		_parse_to_bmp(filename, PIC_BMP, buffer, len);
	}
	else if(strcmp(ext, "tga") == 0)
	{
		_parse_to_bmp(filename, PIC_BMP, buffer, len);
	}else if(strcmp(ext, "png") == 0)
	{
		_parse_to_bmp(filename, PIC_PNG, buffer, len);
	}
	
}

static void _parse_to_bmp(const char * filename, PIC_TYPE type, BYTE ** buffer, unsigned int len)
{
	BMPHEADER		bmpHeader;
	BMPINFOHEADER		biHeader;
	int			i;
	int			j;
	int			l;
	unsigned char *		newBuffer;
	int			offset;
	int			r, g, b, a;
	int			depth;
	int			width;
	int			height;
	//int			bytes_per_line;
	

	switch(type)
	{
	case PIC_BMP:
		memcpy(&bmpHeader, *buffer, sizeof(BMPHEADER));
		memcpy(&biHeader, *buffer+sizeof(BMPHEADER), sizeof(BMPINFOHEADER));
		switch(biHeader.biBitCount)
		{
		case 32:
		case 24:
			depth = biHeader.biBitCount / 8;
			width = biHeader.biWidth;
			height = (int)biHeader.biHeight;
			offset = sizeof(BMPHEADER) + biHeader.biSize; 
			newBuffer = (unsigned char *)malloc(offset + width * abs(height) * 4);
			
			if(height < 0)
			{
				biHeader.biHeight = (DWORD)(-height);
				for(j=0; j>height; j--)
				{
					for(i=0, l=0; i<width*depth; i+=depth, l+=4)
					{
						b = (*buffer)[offset - j*width*depth + i + 0];
						g = (*buffer)[offset - j*width*depth + i + 1];
						r = (*buffer)[offset - j*width*depth + i + 2];
						a = (r + g + b)/3;

						newBuffer[offset - j*width*4 + l + 0] = b;
						newBuffer[offset - j*width*4 + l + 1] = g;
						newBuffer[offset - j*width*4 + l + 2] = r;
						newBuffer[offset - j*width*4 + l + 3] = a;
					}
				}
			}else{
			
				for(j=0; j<height; j++)
				{
					for(i=0, l=0; i<width*depth; i+=depth, l+=4)
					{
						b = (*buffer)[offset + j*width*depth + i + 0];
						g = (*buffer)[offset + j*width*depth + i + 1];
						r = (*buffer)[offset + j*width*depth + i + 2];
						a = (r + g + b)/3;

						newBuffer[offset + (height-j-1)*width*4 + l + 0] = b;
						newBuffer[offset + (height-j-1)*width*4 + l + 1] = g;
						newBuffer[offset + (height-j-1)*width*4 + l + 2] = r;
						newBuffer[offset + (height-j-1)*width*4 + l + 3] = a;
					}
				}

			}

			l = width * height * 4;
			bmpHeader.bfSize = offset + l;
			biHeader.biBitCount = 32;
			biHeader.biSizeImage = l;
			memcpy(newBuffer, &bmpHeader, sizeof(BMPHEADER));
			memcpy(newBuffer+sizeof(BMPHEADER), &biHeader, sizeof(BMPINFOHEADER));
			
			if(biHeader.biSize-sizeof(BMPINFOHEADER) != 0)
			{
				memcpy(	newBuffer, 
					buffer+sizeof(BMPHEADER)+sizeof(BMPINFOHEADER), 
					biHeader.biSize-sizeof(BMPINFOHEADER)
				);
			}
			
			free(*buffer);
			(*buffer) = newBuffer;
			
			break;
		default:
			common::Error("未处理的位图位数: [ %d ] [ %s ]", biHeader.biBitCount, filename);
		}
		
		break;
	case PIC_PNG:
	case PIC_TGA:
	case PIC_GIF:
	case PIC_JPG:
	default:
		common::Error("无法识别的图象类型:[ %s ].", filename);
	}
	
	return;
	
}






namespace res
{
	
	

	void * loadimg(const char * filename, BMPINFOHEADER * info)
	{
		unsigned char *		buffer;
		unsigned int		len;
		int			l;
		//short int		depth;
		//int			i;
		BMPINFOHEADER		biHeader;
		
		buffer = (unsigned char *)pl::load(filename, len);
		
		if(buffer == NULL)
		{
			return NULL;
		}
		
		l = sizeof(BMPHEADER);
		memcpy(&biHeader, buffer+l, sizeof(BMPINFOHEADER));
		l = sizeof(BMPHEADER) + biHeader.biSize;
		
		if(biHeader.biBitCount != 32)
		{
			return NULL;
		}
		
		if(info != NULL)
		{
			memcpy(info, &biHeader, sizeof(BMPINFOHEADER));
		}

		return buffer + l;
	}
}






















