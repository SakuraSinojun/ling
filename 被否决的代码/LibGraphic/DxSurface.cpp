#include "../common/common.h"
#include "../Script/bitmap.h"
#include "DDrawCommon.h"
#include "Geometry.h"
#include "DxSurface.h"

#include <stdio.h>

//////////////////////////////////////////////////////////////////////
//
//  ��̬��Ա��ʼ��
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
// ���г�Ա��������
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
                WriteError("����ģʽ���봫�ݴ��ھ��");
                return false;
        }

        if(s_lpdd)
        {
                 WriteError("�������Ѿ�����");
                return false;
        }
	
	s_mainWndWidth  = m_width  = width;
	s_mainWndHeight = m_height = height;

	s_hWnd = hWnd;
	s_bWindow = bWindow;

        // ��ʶ��ʵ���Ƿ�Ϊ�����ڣ�����ѡ���ͷ���Դ

        m_bMainWnd = true;

	DDSURFACEDESC2 ddsd;
	InitStruct(ddsd);

	// ��ʼ��DxDraw

	if (FAILED (DirectDrawCreateEx (NULL, (void**)&s_lpdd, IID_IDirectDraw7, NULL)))
	{
		WriteError("DirectDrawCreateEx fail!");
		return false;
	}

	// ����Эͬ�ȼ�

	if (bWindow)
	{
		// ����ģʽ

		if (FAILED (s_lpdd->SetCooperativeLevel (hWnd, DDSCL_NORMAL)))
		{
			WriteError("SetCooperativeLevel fail!");
			return false;
		}
	}
	else
	{
		// �Ǵ���ģʽ Ĭ����16λ��ʽ

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
		WriteError("����DX������ʧ��");
		return false;
	}


	m_pixelFormat = com_getSurPixelFormat(s_lpdsPrimary);

        if(m_pixelFormat != 32)
        {
                WriteError("�뽫��ʾ�����ó�32λ");
                return false;
        }

	if (bWindow)
	{
		if(!com_createSurface(this->m_ptr->m_lpddSur, s_lpdd, width, height, 0))
		{	
			WriteError("����DX���廭��ʧ��");
			return false;
		}
	}
	else
	{
		if(!com_getAttachedSurface(s_lpdsPrimary, DDSCAPS_BACKBUFFER,this->GetPtr()))
		{
			WriteError("����DX���廭��ʧ��");
			return false;
		}
	}


	//���û����ʼ��ɫ

	if (bWindow)
	{
		com_colorRect(this->GetPtr(), 0);
	}
	else
	{
		com_colorRect(s_lpdsPrimary, 0);
		com_colorRect(GetPtr(), 0);
	}

	//���ü�������
	
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
                WriteError("�������ȡʧ��");
                return false;
        }

        CDxSurface *dxsur = (CDxSurface *)mainSur;
        LPDIRECTDRAW7 lpdd = dxsur->GetDDraw();
	if(!lpdd)
	{
		WriteError("������û�д���");
		return false;
	}

	if(!com_createSurface(this->m_ptr->m_lpddSur,lpdd ,width, height,0))
	{
		WriteError("��������ʧ��");
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
                // ����������棬���ܴ���

                return false;
        }

        if(NULL == fileName)
        {
                return false;
        }

        // λͼ����
        BMPINFOHEADER info;
        char *bmpBuf = (char*)res::loadimg(fileName, &info);
        if(NULL == bmpBuf)
        {
                WriteError("����λͼ %s ʧ��", fileName);
                return false;
        }

        // �ߴ���
        if(info.biWidth > s_mainWndWidth || info.biHeight > s_mainWndHeight)
        {
                WriteError("λͼ %s �ߴ���ڴ��ڳߴ�", fileName);
                pl::unload(fileName);
                return false;
        }

        this->Create(mainSur, info.biWidth, info.biHeight);

        // ��������
        int lPitch;
        BYTE *surBuf = com_lockSurface(this->GetPtr(),lPitch);
        if(NULL == surBuf)
        {
                WriteError("����dx����ʧ��");
                pl::unload(fileName);
                return false;
        }
        
        // �����ڴ�
        geo_copyBuf((DWORD *)surBuf, (DWORD *)bmpBuf,
		    ((int)lPitch)>>2, info.biWidth, info.biWidth, info.biHeight);

        // ����
        if(!com_unlockSurface(GetPtr()))
        {
                WriteError("�������dx����ʧ��");
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
			// ����ֵ��ָ�����ü�����һ
			// ����һ�����Ϊ�㣬��ɾ��

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
                WriteError("SetColorKey ʧ��!");
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
		
		// ��ȡ��������ԭ�����Ļ����

		POINT pt = {0, 0};
		ClientToScreen(s_hWnd, &pt);

		// ���о��ε���Ļλ��

		rcClient.left   = pt.x;
		rcClient.top    = pt.y;
		rcClient.right  = pt.x + m_width;;
		rcClient.bottom = pt.y + m_height;

		if (FAILED (s_lpdsPrimary->Blt(&rcClient, this->GetPtr(), NULL, DDBLT_WAIT, NULL)))
		{
			WriteError("���廭���л�ʧ��");
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
                WriteError("����dx����ʧ��");
                return false;
        }
        geo_drawLine(x1, y1, x2, y2, color, (DWORD*)buf, lPitch>>2);
        if(!com_unlockSurface(GetPtr()))
        {
                WriteError("�������dx����ʧ��");
                return false;
        }
        return true;
}

//------------------------------------------------------------------------/

bool CDxSurface::DrawSolidRect(DWORD color, LPRECT lpRect)
 {
        if(!com_colorRect(this->GetPtr(), color, lpRect))
        {
                WriteError("���ƾ���ʧ��");
                return false;
        }
        return true;
 }

///////////////////////////////////////////////////////////////////
//
// ������Ա����
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

        // ����λͼ
        BMPINFOHEADER info;
        char *bmpBuf = (char*)res::loadimg(fileName, &info);
        if(NULL == bmpBuf)
        {
                WriteError("���� %s λͼʧ��", fileName);
                return false;
        }

        // �ߴ���
        if(width > m_width-surX || height > m_height-surY)
        {
                WriteError("λͼ %s ���س�������ߴ緶Χ", fileName);
                pl::unload(fileName);
                return false;
        }

        // ��������
        int lPitch;
        BYTE *surBuf = com_lockSurface(this->m_ptr->m_lpddSur,lPitch);
        if(NULL == surBuf)
        {
                WriteError("����dx����ʧ��");
                pl::unload(fileName);
                return false;
        }
        
        // ���µ�����ʼλ��
        surBuf = surBuf + surX + surY * lPitch;
        bmpBuf = bmpBuf + bmpX + bmpY * info.biWidth;

        // �����ڴ�
        geo_copyBuf((DWORD *)surBuf, (DWORD *)bmpBuf,
		    ((int)lPitch)>>2, info.biWidth, width, height);

        // ��������
        if(!com_unlockSurface(GetPtr()))
        {
                WriteError("�������dx����ʧ��");
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

        // �ߴ���
        if(bmp.GetWidth() > m_width-surX || bmp.GetHeight() > m_height-surY)
        {
                WriteError("λͼ %s ���س�������ߴ緶Χ");
                return false;
        }

        // ��������
        int lPitch;
        BYTE *surBuf = com_lockSurface(this->m_ptr->m_lpddSur,lPitch);
        if(NULL == surBuf)
        {
                WriteError("����dx����ʧ��");
                return false;
        }

        char *bmpBuf = bmp.GetBuffer();
        
        // ���µ�����ʼλ��
        surBuf = surBuf + surX + surY * lPitch;
        bmpBuf = bmpBuf + bmpX + bmpY * bmp.GetWidth();

        // �����ڴ�
        geo_copyBuf((DWORD *)surBuf, (DWORD *)bmpBuf,
		    ((int)lPitch)>>2, bmp.GetWidth(), width, height);

        // ��������
        if(!com_unlockSurface(GetPtr()))
        {
                WriteError("�������dx����ʧ��");              
                return false;
        }

        return true;
}