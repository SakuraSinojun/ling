#ifndef DDRAWCOMMON_H_011204
#define DDRAWCOMMON_H_011204

#include <ddraw.h>
// 用来快速初始化ddraw的宏
#define InitStruct(st) {memset (&st,0,sizeof (st)); st.dwSize = sizeof (st);}

// 将制定矩形区域上色
extern bool com_colorRect(LPDIRECTDRAWSURFACE7 lpds, unsigned long color, LPRECT pRectDest = NULL);

// 为画面附上一个调色板
extern LPDIRECTDRAWCLIPPER com_attachClipper(LPDIRECTDRAWSURFACE7 lpds, LPDIRECTDRAW7 lpdd, LPRECT rectList, int rectNum);

// 创建主画面
extern bool com_createPrimarySurface(LPDIRECTDRAWSURFACE7 &lpds, LPDIRECTDRAW7 lpdd, bool isWindow);

// 创建普通画面
extern bool com_createSurface(LPDIRECTDRAWSURFACE7 &lpds, LPDIRECTDRAW7 lpdd, int width, int height, int menFlags, int keyColor = 0);

// 得到画面的像素格式
extern int com_getSurPixelFormat(LPDIRECTDRAWSURFACE7 lpds);

// 得到某一个画面的关联画面
extern bool com_getAttachedSurface(LPDIRECTDRAWSURFACE7 &lpddSurAttatched ,DWORD dwCaps, LPDIRECTDRAWSURFACE7 &lpddSur);

extern bool com_attachSurface(LPDIRECTDRAWSURFACE7 lpddSurDest, LPDIRECTDRAWSURFACE7 lpddSurSrc, LPRECT lprcDest, LPRECT lprcSrc, bool bTransparent);

// 为DX画面指定一个剪切板
extern bool com_setClipper(LPDIRECTDRAWSURFACE7 lpds, LPDIRECTDRAWCLIPPER lpddClipper);

// 锁定DX画面
extern unsigned char *com_lockSurface(LPDIRECTDRAWSURFACE7 lpds, int &lPitch);

// 解锁DX画面
extern bool com_unlockSurface(LPDIRECTDRAWSURFACE7 lpds);

#endif
