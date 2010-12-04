#include "DxSurface.h"



CDxSurface::CDxSurface(const CDxSurface &orig):m_ptr(orig.m_ptr)
{
	++m_ptr->m_use;
}

CDxSurface &CDxSurface::operator= (const CDxSurface& rhs)
{
	++rhs.m_ptr->m_use;
	
	if(--m_ptr->m_use == 0)
	{
		delete m_ptr;
	}
	m_ptr = rhs.m_ptr;
	
	return *this;
}
CDxSurface::~CDxSurface()
{
	if(NULL != m_ptr)
	{
		if(--m_ptr->m_use == 0)
		{
			delete m_ptr;
			m_ptr = NULL;
		}
	}
}

LPDIRECTDRAWSURFACE7 &CDxSurface::GetPtr()
{
	return m_ptr->m_lpddSur;
}
const LPDIRECTDRAWSURFACE7 &CDxSurface::GetPtr() const 
{
	return m_ptr->m_lpddSur;
}

void CDxSurface::SetPtr(LPDIRECTDRAWSURFACE7 lpSur)
{
	m_ptr->m_lpddSur = lpSur;
}

bool CDxSurface::CreateGameWnd(int width,int height, bool bWindow, HWND hWnd)
{
	
	m_width = width;
	m_height = height;

	s_hWnd = hWnd;
	//s_bWindow = bWindow;

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

	if (bWindow)
	{
		if(!com_createSurface(GetPtr(), s_lpdd, width, height, 0))
		{	
			WriteError("创建DX缓冲画面失败");
			return false;
		}
	}
	else
	{
		if(!com_getAttachedSurface(s_lpdsPrimary, DDSCAPS_BACKBUFFER,GetPtr()))
		{
			WriteError("创建DX缓冲画面失败");
			return false;
		}
	}


	//设置画面初始颜色

	if (bWindow)
	{
		com_colorRect(s_lpdsPrimary, 0);
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

bool CDxSurface::Create(int width, int height)
{
	if(!s_lpdd)
	{
		WriteError("主画面没有创建");
		return false;
	}

	if(!com_createSurface(GetPtr(),s_lpdd ,width, height,0))
	{
		WriteError("创建画面失败");
		return false;
	}
	return true;
}
void CDxSurface::WriteError(const char *error)
{
	
}