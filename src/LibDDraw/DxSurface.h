#ifndef DXSURFACE_H_WD101204
#define DXSURFACE_H_WD101204

#include "DDrawCommon.h"

#define DEF_BPP 16	// 默认色彩格式


class AutoSurPtr
{
	friend class CDxSurface;
	AutoSurPtr(LPDIRECTDRAWSURFACE7 lpSur):m_use(1),m_lpddSur(lpSur){}
	~AutoSurPtr()
	{
		if(m_lpddSur)
		{
			m_lpddSur->Release();
			m_lpddSur = NULL;
		}
	}
	LPDIRECTDRAWSURFACE7 m_lpddSur;

	// 计数

	int m_use;

};


class CDxSurface
{
public:
	CDxSurface(LPDIRECTDRAWSURFACE7 lpSur = NULL):m_ptr(new AutoSurPtr(lpSur)){}

	CDxSurface(const CDxSurface &orig);

	CDxSurface& operator= (const CDxSurface& rhs);
	~CDxSurface();

	bool CreateGameWnd(int width,int height, bool bWindow = true, HWND = NULL);
	bool Create(int width, int height);

protected:
	LPDIRECTDRAWSURFACE7& GetPtr();
	const LPDIRECTDRAWSURFACE7& GetPtr() const;
	void SetPtr(LPDIRECTDRAWSURFACE7 lpSur);

	void WriteError(const char *error);
protected:

	AutoSurPtr *m_ptr;
	int m_width;
	int m_height;
	int m_pixelFormat;

protected:
	static LPDIRECTDRAW7 s_lpdd;
	static LPDIRECTDRAWSURFACE7 s_lpdsPrimary;
	static LPDIRECTDRAWCLIPPER  s_lpddClipperWin;
	HWND s_hWnd;
	

};


#endif