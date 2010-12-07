#ifndef DXSURFACE_H_WD101204
#define DXSURFACE_H_WD101204

#define DEF_BPP 32	// 默认色彩格式

#include "Surface.h"
#include <ddraw.h>

class CBitmap;

class CDxSurface :public CSurface
{
        // 智能指针类

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
public:
        // 各种构造函数和赋值函数
        //CDxSurface():m_ptr(new AutoSurPtr(NULL)){this->Init();}
	CDxSurface(LPDIRECTDRAWSURFACE7 lpSur = NULL):m_ptr(new AutoSurPtr(lpSur)){this->Init();}
         CDxSurface(const CDxSurface &orig);
	CDxSurface& operator= (const CDxSurface& rhs);
	~CDxSurface();

        // 返回surface信息
        int GetWidth() const {return m_width;}
        int GetHeight() const {return m_height;}
        LPDIRECTDRAW7 GetDDraw(){return s_lpdd;}

        // surface的设定
	bool CreateMainWnd(int width,int height, bool bWindow, HWND hWnd);
	bool Create(const CSurface *mainSur, int width, int height);
	void ShutDown();
        bool SetColorKey(DWORD color);

        // 简单的绘图
        bool DrawLine(int x1, int y1, int x2, int y2, DWORD color);
        bool DrawSolidRect(DWORD color, LPRECT lpRect);

        // 画面复制
        bool AttachSurface(const CSurface *sur, int x, int y);
        bool Flip();

        // 位图相关
        bool CreateBitmapSur(const CSurface *mainSur, const char *fileName);
        bool LoadBitmapFile(const char *fileName, int surX, int surY,  
                int bmpX, int bmpY, int width, int height);
        bool LoadBitmap(const CBitmap &bmp, int surX, int surY,  
                int bmpX, int bmpY, int width, int height);

protected:


        // 初始化成员变量
	void Init();

        // 错误处理
        void WriteError(const char *error, ...);

        // 返回指针信息
	inline LPDIRECTDRAWSURFACE7& GetPtr();
	inline const LPDIRECTDRAWSURFACE7& GetPtr() const;
	inline void SetPtr(LPDIRECTDRAWSURFACE7 lpSur);

protected:

	bool m_bMainWnd;        // 储存当前画面是否为主画面
	AutoSurPtr *m_ptr;      // 指向当前画面
	int m_width;            // 画面尺寸
	int m_height;             
	int m_pixelFormat;      // 画面像素格式

protected:
                                  
	static LPDIRECTDRAW7 s_lpdd;                    // DDRAW主要信息
	static LPDIRECTDRAWSURFACE7 s_lpdsPrimary;      // 双缓冲的显示画面
	static LPDIRECTDRAWCLIPPER  s_lpddClipperWin;   // WINDOWS窗口的剪切区域
	static HWND s_hWnd;                             // 关联的窗口句柄
        static bool s_bWindow;                          // SURFACE是否在窗口模式下运行
        static int s_mainWndWidth;
        static int s_mainWndHeight;
	

};


#endif
