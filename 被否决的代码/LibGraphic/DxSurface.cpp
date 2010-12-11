#include "../common/common.h"
#include "../Script/bitmap.h"
#include "DDrawCommon.h"
#include "Geometry.h"
#include "DxSurface.h"

#include <stdio.h>

//////////////////////////////////////////////////////////////////////
//
//  静态成员初始化
//
//////////////////////////////////////////////////////////////////////

LPDIRECTDRAW7        CDxSurface::s_lpdd           = NULL;
LPDIRECTDRAWSURFACE7 CDxSurface::s_lpdsPrimary    = NULL;
LPDIRECTDRAWCLIPPER  CDxSurface::s_lpddClipperWin = NULL;
HWND                 CDxSurface::s_hWnd           = NULL;
bool                 CDxSurface::s_bWindow        = true;
int                  CDxSurface::s_mainWndWidth   = -1;
int                  CDxSurface::s_mainWndHeight  = -1;    

//////////////////////////////////////////////////////////////////////
//
// 公有成员函数定义
//
//////////////////////////////////////////////////////////////////////

CDxSurface::CDxSurface(const CDxSurface &orig):m_ptr(orig.m_ptr)
{
	++m_ptr->m_use;
        this->Init();
}

//-------------------------------------------------------------------/

CDxSurface &CDxSurface::operator= (const CDxSurface& rhs)
{
	++rhs.m_ptr->m_use;
	
	if(--m_ptr->m_use == 0)
	{
		delete m_ptr;
	}
	m_ptr = rhs.m_ptr;
	
        this->Init();

	return *this;
}

//--------------------------------------------------/

CDxSurface::~CDxSurface()
{
        this->ShutDown();
}

//------------------------------------------------------------------------------/

bool CDxSurface::CreateMainWnd(int width,int height, bool bWindow, HWND hWnd)
{
        if(NULL == hWnd && bWindow)
        {
                WriteError("窗口模式必须传递窗口句柄");
                return false;
        }

        if(s_lpdd)
        {
                 WriteError("主窗口已经存在");
                return false;
        }
	
	s_mainWndWidth  = m_width  = width;
	s_mainWndHeight = m_height = height;

	s_hWnd = hWnd;
	s_bWindow = bWindow;

        // 标识此实例是否为主窗口，用于选择释放资源

        m_bMainWnd = true;

	DDSURFACEDESC2 ddsd;
	InitStruct(ddsd);

	// 初始化DxDraw

	if (FAILED (DirectDrawCreateEx (NULL, (void**)&s_lpdd, IID_IDirectDraw7, NULL)))
	{
		WriteError("DirectDrawCreateEx fail!");
		return false;
	}

	// 设置协同等级

	if (bWindow)
	{
		// 窗口模式

		if (FAILED (s_lpdd->SetCooperativeLevel (hWnd, DDSCL_NORMAL)))
		{
			WriteError("SetCooperativeLevel fail!");
			return false;
		}
	}
	else
	{
		// 非窗口模式 默认是16位格式

		if (FAILED (s_lpdd->SetCooperativeLevel (hWnd, DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX | 
			DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))
		{
			WriteError("SetCooperativeLevel fail!");
			return false;
		}

		HRESULT hr;
		if (FAILED (hr = s_lpdd->SetDisplayMode (width, height, DEF_BPP, 0, 0)))
		{

			WriteError("SetDisplayMode fail!");
			return false;
		}
	}

	
	if(!com_createPrimarySurface(s_lpdsPrimary, s_lpdd, bWindow))
	{
		WriteError("创建DX主画面失败");
		return false;
	}


	m_pixelFormat = com_getSurPixelFormat(s_lpdsPrimary);

        if(m_pixelFormat != 32)
        {
                WriteError("请将显示器设置成32位");
                return false;
        }

	if (bWindow)
	{
		if(!com_createSurface(this->m_ptr->m_lpddSur, s_lpdd, width, height, 0))
		{	
			WriteError("创建DX缓冲画面失败");
			return false;
		}
	}
	else
	{
		if(!com_getAttachedSurface(s_lpdsPrimary, DDSCAPS_BACKBUFFER,this->GetPtr()))
		{
			WriteError("创建DX缓冲画面失败");
			return false;
		}
	}


	//设置画面初始颜色

	if (bWindow)
	{
		com_colorRect(this->GetPtr(), 0);
	}
	else
	{
		com_colorRect(s_lpdsPrimary, 0);
		com_colorRect(GetPtr(), 0);
	}

	//设置剪切区域
	
	RECT windowRect = {0, 0, width, height};

	if(!com_attachClipper(GetPtr(), s_lpdd, &windowRect, 1))
	{
		WriteError("AttachClipper fail");
		return false;
	}
	if (bWindow)
	{
		if (FAILED (s_lpdd->CreateClipper(0, &s_lpddClipperWin, NULL)))
		{
			WriteError("CreateClipper fail!");
			return false;
		}

		if (FAILED (s_lpddClipperWin->SetHWnd(0, hWnd)))
		{
			WriteError("SetHWnd fail!");
			return false;
		}

		if(!com_setClipper(s_lpdsPrimary, s_lpddClipperWin))
		{
			WriteError("SetWinClipper fail!");
			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------/
bool CDxSurface::Create(const CSurface *mainSur, int width, int height)
{
        if(mainSur == NULL)
        {
                WriteError("主画面获取失败");
                return false;
        }

        CDxSurface *dxsur = (CDxSurface *)mainSur;
        LPDIRECTDRAW7 lpdd = dxsur->GetDDraw();
	if(!lpdd)
	{
		WriteError("主画面没有创建");
		return false;
	}

	if(!com_createSurface(this->m_ptr->m_lpddSur,lpdd ,width, height,0))
	{
		WriteError("创建画面失败");
		return false;
	}

        m_width = width;
        m_height = height;
	return true;
}

void CDxSurface::SetPtr(LPDIRECTDRAWSURFACE7 lpSur)
{
	m_ptr->m_lpddSur = lpSur;
}

bool CDxSurface::CreateBitmapSur(const CSurface *mainSur, const char *fileName)
{
        if(m_bMainWnd)
        {
                // 如果是主画面，则不能创建

                return false;
        }

        if(NULL == fileName)
        {
                return false;
        }

        // 位图加载
        BMPINFOHEADER info;
        char *bmpBuf = (char*)res::loadimg(fileName, &info);
        if(NULL == bmpBuf)
        {
                WriteError("加载位图 %s 失败", fileName);
                return false;
        }

        // 尺寸检测
        if(info.biWidth > s_mainWndWidth || info.biHeight > s_mainWndHeight)
        {
                WriteError("位图 %s 尺寸大于窗口尺寸", fileName);
                pl::unload(fileName);
                return false;
        }

        this->Create(mainSur, info.biWidth, info.biHeight);

        // 锁定画面
        int lPitch;
        BYTE *surBuf = com_lockSurface(this->GetPtr(),lPitch);
        if(NULL == surBuf)
        {
                WriteError("锁定dx画面失败");
                pl::unload(fileName);
                return false;
        }
        
        // 拷贝内存
        geo_copyBuf((DWORD *)surBuf, (DWORD *)bmpBuf,
		    ((int)lPitch)>>2, info.biWidth, info.biWidth, info.biHeight);

        // 解锁
        if(!com_unlockSurface(GetPtr()))
        {
                WriteError("解除锁定dx画面失败");
                pl::unload(fileName);
                return false;
        }

        pl::unload(fileName);

        return true;
}

//----------------------------------------------------------------------/

void CDxSurface::ShutDown()
{
	if(m_bMainWnd)
	{
		if (s_lpddClipperWin)
		{
			s_lpddClipperWin->Release();
			s_lpddClipperWin = NULL;
		}
		
                if(s_lpdsPrimary)
                {
                        s_lpdsPrimary->Release();
                        s_lpdsPrimary = NULL;
                }
		
		if (s_lpdd)
		{
			s_lpdd->Release();
			s_lpdd = NULL;
		}

                s_mainWndWidth   = -1;
                s_mainWndHeight  = -1;  

                m_bMainWnd = false;
	}

        if(NULL != m_ptr)
	{
		if(--m_ptr->m_use == 0)
		{
			// 被赋值的指针引用计数减一
			// 若减一后计数为零，则删除

			delete m_ptr;
			m_ptr = NULL;
		}
        }
}

bool CDxSurface::SetColorKey(DWORD color)
{
        DDCOLORKEY ColorKey;
        ColorKey.dwColorSpaceLowValue  = color;
        ColorKey.dwColorSpaceHighValue = color;
        if (FAILED (this->m_ptr->m_lpddSur->SetColorKey (DDCKEY_SRCBLT, &ColorKey)))
        //if (FAILED (s_lpdsPrimary->SetColorKey (DDCKEY_SRCBLT, &ColorKey)))
        {
                WriteError("SetColorKey 失败!");
                return false;
        }
        
        return true;
}
//------------------------------------------------------------------/

bool CDxSurface::Flip()
{
        if(!m_bMainWnd)
        {
                return false;
        }
        if (!s_bWindow)
	{
		while(FAILED(s_lpdsPrimary->Flip(NULL, DDFLIP_WAIT)));
	}
	else
	{
		RECT rcClient;
		
		// 获取剪切区域原点的屏幕坐标

		POINT pt = {0, 0};
		ClientToScreen(s_hWnd, &pt);

		// 剪切矩形的屏幕位置

		rcClient.left   = pt.x;
		rcClient.top    = pt.y;
		rcClient.right  = pt.x + m_width;;
		rcClient.bottom = pt.y + m_height;

		if (FAILED (s_lpdsPrimary->Blt(&rcClient, this->GetPtr(), NULL, DDBLT_WAIT, NULL)))
		{
			WriteError("缓冲画面切换失败");
			return false;
		}
	}

	return true;
}

 bool CDxSurface::AttachSurface(const CSurface *sur, int x, int y)
 {
        CDxSurface *dxsur = (CDxSurface*)sur;
        RECT rcDest = {x, y , x + dxsur->GetWidth(), y + dxsur->GetHeight()};
	RECT rcSrc  = {0, 0, dxsur->GetWidth(), dxsur->GetHeight()};

        return com_attachSurface(this->GetPtr(), dxsur->GetPtr(), &rcDest, &rcSrc, true);
 }
//-------------------------------------------------------------------------------------/

bool CDxSurface::DrawLine(int x1, int y1, int x2, int y2, DWORD color)
{
        int lPitch;
        BYTE *buf = com_lockSurface(GetPtr(),lPitch);
        if(NULL == buf)
        {
                WriteError("锁定dx画面失败");
                return false;
        }
        geo_drawLine(x1, y1, x2, y2, color, (DWORD*)buf, lPitch>>2);
        if(!com_unlockSurface(GetPtr()))
        {
                WriteError("解除锁定dx画面失败");
                return false;
        }
        return true;
}

//------------------------------------------------------------------------/

bool CDxSurface::DrawSolidRect(DWORD color, LPRECT lpRect)
 {
        if(!com_colorRect(this->GetPtr(), color, lpRect))
        {
                WriteError("绘制矩形失败");
                return false;
        }
        return true;
 }

///////////////////////////////////////////////////////////////////
//
// 保护成员函数
//
//////////////////////////////////////////////////////////////////

void CDxSurface::Init()
{
	m_bMainWnd    = false;
	m_width	      = -1;
	m_height      = -1;
	m_pixelFormat = 0;
}

//------------------------------------------------------/

void CDxSurface::WriteError(const char *error, ...)
{
        char buffer[1024];
        va_list arglist;
        
        va_start(arglist, error);
        vsprintf(buffer, error, arglist);
        va_end(arglist);

        common::Error(error, buffer);
}

//----------------------------------------------------------/

LPDIRECTDRAWSURFACE7 &CDxSurface::GetPtr()
{
	return m_ptr->m_lpddSur;
}

//------------------------------------------------------------/

const LPDIRECTDRAWSURFACE7 &CDxSurface::GetPtr() const 
{
	return m_ptr->m_lpddSur;
}

//-----------------------------------------------------------/

bool CDxSurface::LoadBitmapFile(const char *fileName, int surX, int surY,  
                int bmpX, int bmpY, int width, int height)
{
        if(NULL == fileName)
        {
                return false;
        }

        // 加载位图
        BMPINFOHEADER info;
        char *bmpBuf = (char*)res::loadimg(fileName, &info);
        if(NULL == bmpBuf)
        {
                WriteError("加载 %s 位图失败", fileName);
                return false;
        }

        // 尺寸检测
        if(width > m_width-surX || height > m_height-surY)
        {
                WriteError("位图 %s 加载超出画面尺寸范围", fileName);
                pl::unload(fileName);
                return false;
        }

        // 锁定画面
        int lPitch;
        BYTE *surBuf = com_lockSurface(this->m_ptr->m_lpddSur,lPitch);
        if(NULL == surBuf)
        {
                WriteError("锁定dx画面失败");
                pl::unload(fileName);
                return false;
        }
        
        // 重新调整起始位置
        surBuf = surBuf + surX + surY * lPitch;
        bmpBuf = bmpBuf + bmpX + bmpY * info.biWidth;

        // 拷贝内存
        geo_copyBuf((DWORD *)surBuf, (DWORD *)bmpBuf,
		    ((int)lPitch)>>2, info.biWidth, width, height);

        // 解锁画面
        if(!com_unlockSurface(GetPtr()))
        {
                WriteError("解除锁定dx画面失败");
                pl::unload(fileName);
                return false;
        }

        pl::unload(fileName);

        return true;
}

bool CDxSurface::LoadBitmap(const CBitmap &bmp, int surX, int surY,  
                int bmpX, int bmpY, int width, int height)
{
        if(!bmp.IsLoaded())
        {
                return false;
        }

        // 尺寸检测
        if(bmp.GetWidth() > m_width-surX || bmp.GetHeight() > m_height-surY)
        {
                WriteError("位图 %s 加载超出画面尺寸范围");
                return false;
        }

        // 锁定画面
        int lPitch;
        BYTE *surBuf = com_lockSurface(this->m_ptr->m_lpddSur,lPitch);
        if(NULL == surBuf)
        {
                WriteError("锁定dx画面失败");
                return false;
        }

        char *bmpBuf = bmp.GetBuffer();
        
        // 重新调整起始位置
        surBuf = surBuf + surX + surY * lPitch;
        bmpBuf = bmpBuf + bmpX + bmpY * bmp.GetWidth();

        // 拷贝内存
        geo_copyBuf((DWORD *)surBuf, (DWORD *)bmpBuf,
		    ((int)lPitch)>>2, bmp.GetWidth(), width, height);

        // 解锁画面
        if(!com_unlockSurface(GetPtr()))
        {
                WriteError("解除锁定dx画面失败");              
                return false;
        }

        return true;
}