#ifndef DDRAWCOMMON_H
#define DDRAWCOMMON_H

#include <ddraw.h>

#define InitStruct(st) {memset (&st,0,sizeof (st)); st.dwSize = sizeof (st);}

extern bool com_colorRect(LPDIRECTDRAWSURFACE7 lpds, int color, LPRECT pRectDest = NULL);

extern LPDIRECTDRAWCLIPPER com_attachClipper(LPDIRECTDRAWSURFACE7 lpds, LPDIRECTDRAW7 lpdd, 
					 LPRECT rectList, int rectNum);

extern bool com_createPrimarySurface(LPDIRECTDRAWSURFACE7 &lpds, LPDIRECTDRAW7 lpdd, bool isWindow);

extern bool com_createSurface(LPDIRECTDRAWSURFACE7 &lpds, LPDIRECTDRAW7 lpdd, int width, int height, 
			  int menFlags, int keyColor = 0);

extern int com_getSurPixelFormat(LPDIRECTDRAWSURFACE7 lpds);

extern bool com_getAttachedSurface(LPDIRECTDRAWSURFACE7 &lpddSurAttatched ,
				   DWORD dwCaps, LPDIRECTDRAWSURFACE7 &lpddSur);

bool com_setClipper(LPDIRECTDRAWSURFACE7 lpds, LPDIRECTDRAWCLIPPER lpddClipper);

#endif