#ifndef DDRAWCOMMON_H_011204
#define DDRAWCOMMON_H_011204

#include <ddraw.h>
// �������ٳ�ʼ��ddraw�ĺ�
#define InitStruct(st) {memset (&st,0,sizeof (st)); st.dwSize = sizeof (st);}

// ���ƶ�����������ɫ
extern bool com_colorRect(LPDIRECTDRAWSURFACE7 lpds, unsigned long color, LPRECT pRectDest = NULL);

// Ϊ���渽��һ����ɫ��
extern LPDIRECTDRAWCLIPPER com_attachClipper(LPDIRECTDRAWSURFACE7 lpds, LPDIRECTDRAW7 lpdd, LPRECT rectList, int rectNum);

// ����������
extern bool com_createPrimarySurface(LPDIRECTDRAWSURFACE7 &lpds, LPDIRECTDRAW7 lpdd, bool isWindow);

// ������ͨ����
extern bool com_createSurface(LPDIRECTDRAWSURFACE7 &lpds, LPDIRECTDRAW7 lpdd, int width, int height, int menFlags, int keyColor = 0);

// �õ���������ظ�ʽ
extern int com_getSurPixelFormat(LPDIRECTDRAWSURFACE7 lpds);

// �õ�ĳһ������Ĺ�������
extern bool com_getAttachedSurface(LPDIRECTDRAWSURFACE7 &lpddSurAttatched ,DWORD dwCaps, LPDIRECTDRAWSURFACE7 &lpddSur);

extern bool com_attachSurface(LPDIRECTDRAWSURFACE7 lpddSurDest, LPDIRECTDRAWSURFACE7 lpddSurSrc, LPRECT lprcDest, LPRECT lprcSrc, bool bTransparent);

// ΪDX����ָ��һ�����а�
extern bool com_setClipper(LPDIRECTDRAWSURFACE7 lpds, LPDIRECTDRAWCLIPPER lpddClipper);

// ����DX����
extern unsigned char *com_lockSurface(LPDIRECTDRAWSURFACE7 lpds, int &lPitch);

// ����DX����
extern bool com_unlockSurface(LPDIRECTDRAWSURFACE7 lpds);

#endif
