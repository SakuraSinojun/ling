
#include "common.h"
#include "render.h"
#include "map.h"
#include <windows.h>

POINT	XY2Pixel(int x, int y);
POINT	Pixel2XY(int x, int y);


static HDC	memDC = NULL;
static BYTE *	bg_buffer = NULL;
static RECT	m_rect;





// 没什么特殊的，，就是把CMap类的格子逐一绘制到了窗口上。
void render(HWND hWnd)
{
	// 窗口不存在
	if( !IsWindow (hWnd) )
	{
		return;
	}

	//窗口最小化，交出CPU时间片
	if( !IsWindowVisible (hWnd) )
	{
		Sleep(5);
		return;
	}



	HDC		hdc = GetDC(hWnd);
	CMap *		map = CMap::GetMap ();
	int		x;
	int		y;
	int		i;
	int		j;
	int		offset;
	int		offset2;
	MAPSQUARE *	sqr;
	POINT		point;
	BYTE *		buffer;
	RECT		rect;
	BMPINFOHEADER	biHeader;
	int		width;
	int		height;
	HBITMAP		hBitmap = NULL;

	// 第一次调用这个函数
	if(memDC == NULL)
	{
		memDC = CreateCompatibleDC(hdc);
	}
	
	// 窗口大小
	GetClientRect(hWnd, &rect);
	width	= rect.right - rect.left ;
	height	= rect.bottom - rect.top ;

	// 窗口大小改变，在窗口第一次建立的时候也会发生窗口大小改变。
	// 重新分配背景位图缓冲区。
	if(rect.left != m_rect.left || rect.top != m_rect.top || rect.right != m_rect.right || rect.bottom != m_rect.bottom )
	{
		m_rect = rect;
		if(bg_buffer != NULL)
		{
			free(bg_buffer);
		}
		bg_buffer = (BYTE *)malloc(width * height * 4);
		memset(bg_buffer, 0, width * height * 4);
	}
	
	memset(&biHeader, 0, sizeof(biHeader));

	// 逐格子画到背景上
	for(y=0; y<map->Height (); y++)
	{
		for(x=0; x<map->Width (); x++)
		{
			sqr = map->GetSqr (x, y);
			point = XY2Pixel(x, y);
			if(point.x+biHeader.biWidth >= width || point.y+biHeader.biHeight >= height)
			{
				break;
			}
			buffer = (BYTE*)res::loadimg(sqr->bg_file, &biHeader);
			if(buffer == NULL)
			{
				common::Error ("文件[ %s ]不存在!", sqr->bg_file);
				return;
			}
			for(j=sqr->y1; j<=sqr->y2; j++)
			{
				for(i=sqr->x1; i<=sqr->x2; i++)
				{
					offset = (i + j * biHeader.biWidth) * 4;
					offset2 = ( point.x + i - sqr->x1 + (point.y + j - sqr->y1 )* width ) * 4;
					bg_buffer[offset2++] = buffer[offset++];
					bg_buffer[offset2++] = buffer[offset++];
					bg_buffer[offset2++] = buffer[offset++];
					bg_buffer[offset2++] = buffer[offset++];
				}
			}
		}
	}
	

	hBitmap = CreateBitmap(width, height, 1, 32, bg_buffer);
	SelectObject(memDC, hBitmap);
	DeleteObject(hBitmap);

	BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

	ReleaseDC(hWnd, hdc);

}


POINT XY2Pixel(int x, int y)
{
	POINT point;
	point.x = x * 64;
	point.y = y * 64;
	return point;
}

POINT	Pixel2XY(int x, int y)
{
	POINT point;
	point.x = x / 64;
	point.y = y / 64;
	return point;
}































