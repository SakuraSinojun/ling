#ifndef GEOMETRY_H_101205
#define GEOMETRY_H_101205


typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;


inline void MemSetDWORD(void *dest, DWORD data, int count)
{
_asm 
    { 
    mov edi, dest   ; edi points to destination memory
    mov ecx, count  ; number of 32-bit words to move
    mov eax, data   ; 32-bit data
    rep stosd       ; move data
    }
} 

// 拷贝内存
extern void geo_copyBuf(DWORD *destBuf, DWORD *srcBuf, int destDiff, int srcDiff,  
			   int copyWidth, int copyHeight, bool bTransparent = false);

inline DWORD ARGB(int alpha, int red, int green, int blue)
{
	return (blue + (green << 8) + (red << 16) + (alpha << 24)); 
}

inline DWORD RGBA(int red, int green, int blue, int alpha)
{
	return ((red << 24) + (green << 16) + (blue << 8) + alpha); 
}


////////////////////////////////////////////////////////////////////////////////////
//
// 基本图形函数
//
//////////////////////////////////////////////////////////////////////////////////////

extern void geo_drawLine(int x1, int y1, int x2, int y2, DWORD color, DWORD *videoBuf, int lPitch);

// 检测内存中某一个长方形内的是否有startColor至endColor的颜色
// x1,y1为长方形左上角坐标，x2,y2为右下角坐标

extern bool geo_scanColor(int x1, int y1, int x2, int y2, DWORD startColor, DWORD endColor, DWORD *videoBuf, int lPitch); 

// 在2D矩形框内剪切直线

extern bool geo_clipLine(int &x1, int &y1, int &x2, int &y2, int min_clip_x, int min_clip_y, int max_clip_x, int max_clip_y);

// 画三角形，通过bpp的值来进行选择上色模式

// 平顶三角形
extern void geo_drawTopTri(int x1, int y1, int x2, int y2, int x3, int y3, 
                           int minClipX, int minClipY, int maxClipX, int maxClipY, 
                           DWORD color, BYTE *videoBuf, int lPitch, int bpp);
// 平底三角形
extern void geo_drawBottomTri(int x1, int y1, int x2, int y2, int x3, int y3,
                          int minClipX, int minClipY, int maxClipX, int maxClipY,
			  DWORD color, BYTE *videoBuf, int lPitch, int bpp);
// 三角形
extern void geo_drawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3,
			   int minClipX, int minClipY, int maxClipX, int maxClipY,
			    DWORD color, BYTE *videoBuf, int lPitch, int bpp);

#endif