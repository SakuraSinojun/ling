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
		//С��45��
		//��ʼ��������ʼΪ0���߲���ȷ
		//��С��45��ʱ��ÿ�ε���2*dy
		//����������2*dxʱ��֤��Y�õ�����

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
		//����45��

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