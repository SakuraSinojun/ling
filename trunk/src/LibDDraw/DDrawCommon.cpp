#include "DDrawCommon.h"


LPDIRECTDRAWCLIPPER com_attachClipper(LPDIRECTDRAWSURFACE7 lpds, LPDIRECTDRAW7 lpdd, LPRECT rectList, int rectNum)
{
	LPDIRECTDRAWCLIPPER lpddClipper = NULL;

	if (FAILED (lpdd->CreateClipper(0, &lpddClipper, NULL)))
	{
		return NULL;
	}

	LPRGNDATA lpRgnData;
	lpRgnData = new RGNDATA[sizeof(RGNDATAHEADER) + rectNum * sizeof(RECT)];
	memcpy(lpRgnData->Buffer, rectList, sizeof(RECT)*rectNum);

	lpRgnData->rdh.dwSize          = sizeof(RGNDATAHEADER);
	lpRgnData->rdh.iType           = RDH_RECTANGLES;
	lpRgnData->rdh.nCount          = rectNum;
	lpRgnData->rdh.nRgnSize        = rectNum * sizeof(RECT);

	lpRgnData->rdh.rcBound.left    =  64000;
	lpRgnData->rdh.rcBound.top     =  64000;
	lpRgnData->rdh.rcBound.right   = -64000;
	lpRgnData->rdh.rcBound.bottom  = -64000;

	for (int index=0; index < rectNum; index++)
	{
		// test if the next rectangle unioned with the current bound is larger
		if (rectList[index].left < lpRgnData->rdh.rcBound.left)
			lpRgnData->rdh.rcBound.left = rectList[index].left;

		if (rectList[index].right > lpRgnData->rdh.rcBound.right)
			lpRgnData->rdh.rcBound.right = rectList[index].right;

		if (rectList[index].top < lpRgnData->rdh.rcBound.top)
			lpRgnData->rdh.rcBound.top = rectList[index].top;

		if (rectList[index].bottom > lpRgnData->rdh.rcBound.bottom)
			lpRgnData->rdh.rcBound.bottom = rectList[index].bottom;
	} // end for index

	if (FAILED (lpddClipper->SetClipList(lpRgnData, 0)))
	{
		delete lpRgnData;
		return NULL;
	}

	if (FAILED (lpds->SetClipper (lpddClipper)))
	{
		delete lpRgnData;
		return NULL;
	}

	delete lpRgnData;

	return lpddClipper;
}


bool com_createPrimarySurface(LPDIRECTDRAWSURFACE7 &lpds, LPDIRECTDRAW7 lpdd, bool isWindow)
{
	DDSURFACEDESC2 ddsd;
	InitStruct(ddsd);
	
	if(isWindow)
	{
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		ddsd.dwBackBufferCount = 0;
	}
	else
	{
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
		ddsd.dwBackBufferCount = 1;
	}

	if(FAILED(lpdd->CreateSurface(&ddsd, &lpds, NULL)))
	{
		return false;
	}

	return true;
}

bool com_createSurface(LPDIRECTDRAWSURFACE7 &lpds, LPDIRECTDRAW7 lpdd, int width, int height, int menFlags, int keyColor)
{
	DDSURFACEDESC2 ddsd;
	InitStruct(ddsd);

	ddsd.dwFlags  = DDSD_CAPS |DDSD_WIDTH |DDSD_HEIGHT |DDSD_CKSRCBLT ;

	ddsd.dwWidth  = width;
	ddsd.dwHeight = height;
	ddsd.ddsCaps.dwCaps =DDSCAPS_OFFSCREENPLAIN | menFlags;

	if (FAILED (lpdd->CreateSurface (&ddsd, &lpds,NULL)))
	{
		return false;
	}

	if (keyColor >= 0)
	{
		DDCOLORKEY ColorKey;
		ColorKey.dwColorSpaceLowValue  = keyColor;
		ColorKey.dwColorSpaceHighValue = keyColor;
		if (FAILED (lpds->SetColorKey (DDCKEY_SRCBLT, &ColorKey)))
		{
			return false;
		}
	}

	return true;
}

bool com_colorRect(LPDIRECTDRAWSURFACE7 lpds, int color, LPRECT pRectDest)
{
	DDBLTFX ddbltfx; // the blitter fx structure

	InitStruct(ddbltfx);

	ddbltfx.dwFillColor = color;

	if (FAILED(lpds->Blt(pRectDest,
		NULL, 
		NULL,
		DDBLT_COLORFILL |DDBLT_WAIT, 
		&ddbltfx))) 
	{
		return false;
	}

	return true;
}

int com_getSurPixelFormat(LPDIRECTDRAWSURFACE7 lpds)
{
	DDPIXELFORMAT ddPixelFormat;
	InitStruct(ddPixelFormat);

	lpds->GetPixelFormat (&ddPixelFormat);

	return ddPixelFormat.dwRGBBitCount;
}

bool com_getAttachedSurface(LPDIRECTDRAWSURFACE7 &lpddSurAttatched ,DWORD dwCaps, LPDIRECTDRAWSURFACE7 &lpddSur)
{
	DDSURFACEDESC2 ddsd;
	InitStruct(ddsd);

	ddsd.ddsCaps.dwCaps = dwCaps;

	if(FAILED(lpddSurAttatched->GetAttachedSurface(&ddsd.ddsCaps, &lpddSur)))
	{
		return false;
	}

	return true;

}

bool com_setClipper(LPDIRECTDRAWSURFACE7 lpds, LPDIRECTDRAWCLIPPER lpddClipper)
{
	if (FAILED (lpds->SetClipper(lpddClipper)))
	{
		return false;
	}

	return true;
}