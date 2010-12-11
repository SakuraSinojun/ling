
/*******************************************************************************

        Author: Ó£Áè×¼
        Tab:    8
        
*******************************************************************************/


#pragma once

#ifndef __COMDEF_H__
#define __COMDEF_H__

#ifndef WIN32
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



#pragma pack(pop)



#endif

