#include <memory.h>
#include "Geometry.h"

void geo_drawLine(int x1, int y1, int x2, int y2, DWORD color, DWORD *videoBuf, int lPitch)
{
        int dx  = x2 - x1,
		dy  = y2 - y1,
		dx2,
		dy2,
		xInc,
		yInc,
		error,
		index;

	videoBuf = videoBuf + x1 + y1 * lPitch;

	if (dx >= 0)
	{
		xInc = 1;
	}
	else
	{
		xInc = -1;
		dx	 = -dx;
	}

	if (dy >= 0)
	{
		yInc = lPitch;
	}
	else
	{
		yInc = -lPitch;
		dy	 = -dy;
	}

	dx2 = dx<<1;
	dy2 = dy<<1;

	if (dx > dy)
	{
		//小于45度
		//初始化误差，若初始为0画线不精确
		//当小于45度时，每次递增2*dy
		//当递增大于2*dx时，证明Y该递增了

		error = dy2 - dx;
		for (index = 0; index <= dx; index++)
		{
			*videoBuf = color;
			if (error > 0)
			{
				error -= dx2;
				videoBuf += yInc;
			}

			error += dy2;
			videoBuf += xInc;
		}
	}
	else
	{
		//大于45度

		error = dx2 - dy;
		for (index = 0; index <= dy; index++)
		{
			*videoBuf = color;
			if (error > 0)
			{
				error -= dy2;
				videoBuf += xInc;
			}

			error +=dx2;
			videoBuf += yInc;
		}
	}
}

void geo_copyBuf(DWORD *destBuf, DWORD *srcBuf, int destDiff, int srcDiff,  
		 int copyWidth, int copyHeight, bool bTransparent)
{

	if (bTransparent)
	{
		DWORD pixel;
		for (int j = 0; j < copyHeight; j++)
		{
			// copy the memory
			for (int i = 0; i < copyWidth; i++)
			{
				if ((pixel = srcBuf[i])!=0)
				{
					destBuf[i] = pixel;
				}

			}

			destBuf += destDiff;
			srcBuf += srcDiff;
		} 
	}
	else
	{
                // DWORD = 4*char

		copyWidth <<= 2;

		for (int y=0; y < copyHeight; y++)
		{
			memcpy((char *)destBuf, (char *)srcBuf,copyWidth);

			destBuf += destDiff;
			srcBuf  += srcDiff;
		} 
	}
}