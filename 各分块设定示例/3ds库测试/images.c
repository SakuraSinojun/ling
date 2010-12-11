
#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////
// Define targa header. This is only used locally.
#pragma pack(1)
typedef struct
{
	GLbyte		identsize;              // Size of ID field that follows header (0)
	GLbyte		colorMapType;           // 0 = None, 1 = paletted
	GLbyte		imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;          // First colour map entry
	unsigned short	colorMapLength;         // Number of colors
	unsigned char 	colorMapBits;   // bits per palette entry
	unsigned short	xstart;                 // image x origin
	unsigned short	ystart;                 // image y origin
	unsigned short	width;                  // width in pixels
	unsigned short	height;                 // height in pixels
	GLbyte		bits;                   // bits per pixel (8 16, 24, 32)
	GLbyte		descriptor;             // image descriptor
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


#pragma pack(8)



////////////////////////////////////////////////////////////////////
// Capture the current viewport and save it as a targa file.
// Be sure and call SwapBuffers for double buffered contexts or
// glFinish for single buffered contexts before calling this function.
// Returns 0 if an error occurs, or 1 on success.
GLint images_save_tga(const char *szFileName)
{
	FILE *		pFile;			// File pointer
	TGAHEADER 	tgaHeader;		// TGA file header
	unsigned long 	lImageSize;		// Size in bytes of image
	GLbyte *	pBits = NULL;		// Pointer to bits
	GLint 		iViewport[4];		// Viewport in pixels
	GLenum 		lastBuffer;		// Storage for the current read buffer setting

	// Get the viewport dimensions
	glGetIntegerv(GL_VIEWPORT, iViewport);

	// How big is the image going to be (targas are tightly packed)
	lImageSize = iViewport[2] * 3 * iViewport[3];	

	// Allocate block. If this doesn't work, go home
	pBits = (GLbyte *)malloc(lImageSize);
	if(pBits == NULL)
	{
		return 0;
	}
	
	// Read bits from color buffer
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

	// Get the current read buffer setting and save it. Switch to
	// the front buffer and do the read operation. Finally, restore
	// the read buffer state
	glGetIntegerv(GL_READ_BUFFER, (GLint *)&lastBuffer);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, iViewport[2], iViewport[3], GL_BGR_EXT, GL_UNSIGNED_BYTE, pBits);
	glReadBuffer(lastBuffer);

	// Initialize the Targa header
	tgaHeader.identsize 	= 0;
	tgaHeader.colorMapType 	= 0;
	tgaHeader.imageType 	= 2;
	tgaHeader.colorMapStart = 0;
	tgaHeader.colorMapLength = 0;
	tgaHeader.colorMapBits 	= 0;
	tgaHeader.xstart 	= 0;
	tgaHeader.ystart 	= 0;
	tgaHeader.width 	= iViewport[2];
	tgaHeader.height 	= iViewport[3];
	tgaHeader.bits 		= 24;
	tgaHeader.descriptor 	= 0;

	// Attempt to open the file
	pFile = fopen(szFileName, "wb");
	if(pFile == NULL)
	{
		free(pBits);    // Free buffer and return error
		return 0;
	}

	// Write the header
	fwrite(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

	// Write the image data
	fwrite(pBits, lImageSize, 1, pFile);

	// Free temporary buffer and close the file
	free(pBits);    
	fclose(pFile);

	// Success!
	return 1;

}


////////////////////////////////////////////////////////////////////
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
GLbyte * images_load_tga(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
	
	FILE *			pFile;			// File pointer
	TGAHEADER 		tgaHeader;		// TGA file header
	unsigned long 		lImageSize;		// Size in bytes of image
	short 			sDepth;			// Pixel depth;
	GLbyte *		pBits = NULL;		// Pointer to bits

	// Default/Failed values
	*iWidth 		= 0;
	*iHeight 		= 0;
	*eFormat 		= GL_BGR_EXT;
	*iComponents 		= GL_RGB8;

	// Attempt to open the fil
	pFile = fopen(szFileName, "rb");
	if(pFile == NULL)
	{
		return NULL;
	}
	
	// Read in header (binary)
	fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);

	// Get width, height, and depth of texture
	*iWidth		= tgaHeader.width;
	*iHeight	= tgaHeader.height;
	sDepth		= tgaHeader.bits / 8;

	// Put some validity checks here. Very simply, I only understand
	// or care about 8, 24, or 32 bit targa's.
	if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
	{
		return NULL;
	}
	
	// Calculate size of image buffer
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	// Allocate memory and check for success
	pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if(pBits == NULL)
	{
		return NULL;
	}
	
	// Read in the bits
	// Check for read error. This should catch RLE or other 
	// weird formats that I don't want to recognize
	if(fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return NULL;
	}

	// Set OpenGL format expected
	switch(sDepth)
	{
		case 3:     // Most likely case
			*eFormat = GL_BGR_EXT;
			*iComponents = GL_RGB8;
			break;
		case 4:
			*eFormat = GL_BGRA_EXT;
			*iComponents = GL_RGBA8;
			break;
		case 1:
			*eFormat = GL_LUMINANCE;
			*iComponents = GL_LUMINANCE8;
			break;
	};


	// Done with File
	fclose(pFile);

	// Return pointer to image data
	return pBits;

}







////////////////////////////////////////////////////////////////////
// Capture the current viewport and save it as a bitmap file.
// Be sure and call SwapBuffers for double buffered contexts or
// glFinish for single buffered contexts before calling this function.
// Returns 0 if an error occurs, or 1 on success.
GLint images_save_bmp(const char *szFileName)
{
	
	FILE *			pFile;			// File pointer
	BMPHEADER 		bmpHeader;		// BMP file header
	BMPINFOHEADER		biHeader;		// bmp info header
	unsigned long 		lImageSize;		// Size in bytes of image
	GLbyte *		pBits = NULL;		// Pointer to bits
	GLint 			iViewport[4];		// Viewport in pixels
	GLenum 			lastBuffer;		// Storage for the current read buffer setting
	GLenum			fmt;
	GLint			depth = 24;
	
	
	if(depth == 8)
	{
		fmt = GL_LUMINANCE;
	}else if(depth == 24)
	{
		fmt = GL_BGR_EXT;
	}else if(depth == 32)
	{
		fmt = GL_BGRA_EXT;
	}else{
		return 0;
	}
	
	// Get the viewport dimensions
	glGetIntegerv(GL_VIEWPORT, iViewport);

	// How big is the image going to be (bitmaps are tightly packed)
	lImageSize = iViewport[2] * depth / 8 * iViewport[3];	

	// Allocate block. If this doesn't work, go home
	pBits = (GLbyte *)malloc(lImageSize);
	if(pBits == NULL)
	{
		return 0;
	}
	
	// Read bits from color buffer
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

	// Get the current read buffer setting and save it. Switch to
	// the front buffer and do the read operation. Finally, restore
	// the read buffer state
	glGetIntegerv(GL_READ_BUFFER, (GLint *)&lastBuffer);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, iViewport[2], iViewport[3], fmt, GL_UNSIGNED_BYTE, pBits);
	glReadBuffer(lastBuffer);

	// Initialize the Targa header
	memset(&bmpHeader, 0, sizeof(BMPHEADER));
	memset(&biHeader, 0, sizeof(BMPINFOHEADER));
	bmpHeader.bfType 	= 0x4d42;
	bmpHeader.bfReserved1 	= 0;
	bmpHeader.bfReserved2	= 0;
	bmpHeader.bfSize 	= sizeof(BMPHEADER) + sizeof(BMPINFOHEADER) + lImageSize;
	bmpHeader.bfOffBits	= sizeof(BMPHEADER) + sizeof(BMPINFOHEADER);
	biHeader.biSize		= sizeof(BMPINFOHEADER);
	biHeader.biWidth	= iViewport[2];
	biHeader.biHeight	= iViewport[3];
	biHeader.biPlanes	= 1;
	biHeader.biBitCount	= depth;
	biHeader.biCompression	= 0;
	biHeader.biSizeImage	= lImageSize;
	
	// Attempt to open the file
	pFile = fopen(szFileName, "wb");
	if(pFile == NULL)
	{
		free(pBits);    // Free buffer and return error
		return 0;
	}

	// Write the header
	fwrite(&bmpHeader, sizeof(BMPHEADER), 1, pFile);
	fwrite(&biHeader, sizeof(BMPINFOHEADER), 1, pFile);

	// Write the image data
	fwrite(pBits, lImageSize, 1, pFile);

	// Free temporary buffer and close the file
	free(pBits);    
	fclose(pFile);

	// Success!
	return 1;

}


////////////////////////////////////////////////////////////////////
// Allocate memory and load bmp bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla bitmaps... 8, 24, or 32 bit color
// only.
GLbyte * images_load_bmp(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
	
	FILE *			pFile;			// File pointer
	BMPHEADER 		bmpHeader;		// BMP file header
	BMPINFOHEADER		biHeader;		// bmp info header
	unsigned long 		lImageSize;		// Size in bytes of image
	short 			sDepth;			// Pixel depth;
	GLbyte *		pBits = NULL;		// Pointer to bits

	// Default/Failed values
	*iWidth 		= 0;
	*iHeight 		= 0;
	*eFormat 		= GL_BGR_EXT;
	*iComponents 		= GL_RGB8;

	// Attempt to open the fil
	pFile = fopen(szFileName, "rb");
	if(pFile == NULL)
	{
		return NULL;
	}
	
	// Read in header (binary)
	fread(&bmpHeader, sizeof(BMPHEADER), 1, pFile);
	fread(&biHeader, sizeof(BMPINFOHEADER), 1, pFile);


	// Get width, height, and depth of texture
	*iWidth		= biHeader.biWidth;
	*iHeight	= biHeader.biHeight;
	sDepth		= biHeader.biBitCount / 8;

	// Put some validity checks here. Very simply, I only understand
	// or care about 8, 24, or 32 bit targa's.
	if(biHeader.biBitCount != 8 && biHeader.biBitCount!= 24 && biHeader.biBitCount != 32)
	{
		return NULL;
	}
	
	// Calculate size of image buffer
	lImageSize = biHeader.biWidth * biHeader.biHeight * sDepth;

	// Allocate memory and check for success
	pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if(pBits == NULL)
	{
		return NULL;
	}
	
	// Read in the bits
	// Check for read error. This should catch RLE or other 
	// weird formats that I don't want to recognize
	fseek(pFile, sizeof(BMPHEADER) + biHeader.biSize, SEEK_SET);
	if(fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return NULL;
	}

	// Set OpenGL format expected
	switch(sDepth)
	{
		case 3:     // Most likely case
			*eFormat = GL_BGR_EXT;
			*iComponents = GL_RGB8;
			break;
		case 4:
			*eFormat = GL_BGRA_EXT;
			*iComponents = GL_RGBA8;
			break;
		case 1:
			*eFormat = GL_LUMINANCE;
			*iComponents = GL_LUMINANCE8;
			break;
	};


	// Done with File
	fclose(pFile);

	// Return pointer to image data
	return pBits;

}


GLbyte * images_load_bmp_rgba(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
	GLbyte	*	pBit = images_load_bmp(szFileName, iWidth, iHeight, iComponents, eFormat);
	int		i;
	int		j;
	GLbyte	*	rgba_ptr;
	int		length;
	int		r, g, b, a;
	
	if(*eFormat == GL_BGRA_EXT)
	{
		return pBit;
	}
	
	rgba_ptr = (GLbyte *)malloc((*iWidth) * (*iHeight) * 4);
	
	
	if(*eFormat == GL_LUMINANCE)
	{
		length = (*iWidth) * (*iHeight) * 1;
		for(i=0,j=0; i<=length-1; i+=1, j+=4)
		{
			r = pBit[i];
			rgba_ptr[j] = r;
			rgba_ptr[j+1] = r;
			rgba_ptr[j+2] = r;
			rgba_ptr[j+3] = r;
		}
	}
	
	if(*eFormat == GL_BGR_EXT)
	{
		length = (*iWidth) * (*iHeight) * 3;
		for(i=0,j=0; i<=length-3; i+=3,j+=4)
		{
			b = pBit[i];
			g = pBit[i+1];
			r = pBit[i+2];
			a = (r + b + g)/3;
			rgba_ptr[j] = b;
			rgba_ptr[j+1] = g;
			rgba_ptr[j+2] = r;
			rgba_ptr[j+3] = a;
		}
	}
	*eFormat = GL_BGRA_EXT;
	*iComponents = GL_RGBA8;
	
	return rgba_ptr;
	
}


GLbyte * images_load_tga_rgba(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
	GLbyte	*	pBit = images_load_tga(szFileName, iWidth, iHeight, iComponents, eFormat);
	int		i;
	int		j;
	GLbyte	*	rgba_ptr;
	int		length;
	int		r, g, b, a;
	
	if(*eFormat == GL_BGRA_EXT)
	{
		return pBit;
	}
	
	rgba_ptr = (GLbyte *)malloc((*iWidth) * (*iHeight) * 4);
	
	
	if(*eFormat == GL_LUMINANCE)
	{
		length = (*iWidth) * (*iHeight) * 1;
		for(i=0,j=0; i<=length-1; i+=1, j+=4)
		{
			r = pBit[i];
			rgba_ptr[j] = r;
			rgba_ptr[j+1] = r;
			rgba_ptr[j+2] = r;
			rgba_ptr[j+3] = r;
		}
	}
	
	if(*eFormat == GL_BGR_EXT)
	{
		length = (*iWidth) * (*iHeight) * 3;
		for(i=0,j=0; i<=length-3; i+=3,j+=4)
		{
			b = pBit[i];
			g = pBit[i+1];
			r = pBit[i+2];
			a = (r + b + g)/3;
			rgba_ptr[j] = b;
			rgba_ptr[j+1] = g;
			rgba_ptr[j+2] = r;
			rgba_ptr[j+3] = a;
		}
	}
	*eFormat = GL_BGRA_EXT;
	*iComponents = GL_RGBA8;
			
	return rgba_ptr;
	
}









