#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <DXFile.h>
#include <rmxftmpl.h>		// 注册X文件模板宏
#include <RmxfGuid.h>		// GUID
#include <windows.h>
#include <tchar.h>
#include "Graphics.h"


void _WriteError(LPCTSTR error)
{
	::MessageBox(0, error, 0, 0);
}

CGraphics::CGraphics()
{
	m_hWnd      = NULL;
	m_pD3D      = NULL;
	m_pDevice   = NULL;
	m_pSprite   = NULL;

	m_width     = 0;
	m_height    = 0;
	m_bpp       = 0;

	m_bWindow   = TRUE;
	m_bHAL      = FALSE;
	m_bZBuffer  = FALSE;
}

CGraphics::~CGraphics()
{
	this->ShutDown();
}

BOOL CGraphics::ShutDown()
{
	ReleaseCOM(m_pDevice);
	ReleaseCOM(m_pD3D);
	ReleaseCOM(m_pSprite);
	return TRUE;
}

BOOL CGraphics::Init(HWND hWnd, BOOL bWindow, DWORD width, DWORD height, BYTE bpp)
{
	if(NULL == hWnd)
	{
		return FALSE;
	}

	if((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		_WriteError(_T("初始化D3D失败"));
		return FALSE;
	}

	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_d3ddm)))
	{
		return FALSE;
	}

	m_bWindow = bWindow;
	m_width   = width;
	m_height  = height;
	m_bpp     = bpp;

	RECT wndRect, clientRect;

	if(m_bWindow)
	{
		GetClientRect(hWnd, &clientRect);
		GetWindowRect(hWnd, &wndRect);
	}

	if(0 == m_width)
	{
		m_width = m_bWindow ? clientRect.right : m_d3ddm.Width;
		
	}

	if(0 == m_height)
	{
		m_height = m_bWindow ? clientRect.bottom : m_d3ddm.Height;
	}


	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	if(m_bWindow)
	{
		// 调整窗口大小

		int wndWidth  = (wndRect.right  - (clientRect.right  - m_width))  - wndRect.left;
		int wndHeight = (wndRect.bottom - (clientRect.bottom - m_height)) - wndRect.top;

		MoveWindow(m_hWnd, wndRect.left, wndRect.top, wndWidth, wndHeight, TRUE);

		// 初始化DX窗口

		d3dpp.Windowed         = TRUE;
		d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = m_d3ddm.Format;
		d3dpp.BackBufferCount  = 1;

		/*
		d3dpp.BackBufferWidth            = m_width;
		d3dpp.BackBufferHeight           = m_height;
		d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount            = 1;
		d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality         = 0;
		d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
		d3dpp.hDeviceWindow              = hWnd;
		d3dpp.Windowed                   = bWindow;
		d3dpp.EnableAutoDepthStencil     = true; 
		d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
		d3dpp.Flags                      = 0;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
		*/

		if(_CheckFormat(m_d3ddm.Format, m_bWindow, TRUE))
		{
			// 查询是否支持HAL
			m_bHAL = TRUE;
		}
		else
		{
			if(!_CheckFormat(m_d3ddm.Format, TRUE, FALSE))
			{
				_WriteError(_T("没有支持的格式"));
				return FALSE;
			}
		}
	}
	else
	{
		d3dpp.Windowed                        = FALSE;
		d3dpp.SwapEffect                      = D3DSWAPEFFECT_FLIP;
		d3dpp.BackBufferWidth                 = m_width;
		d3dpp.BackBufferHeight                = m_height;
		d3dpp.FullScreen_RefreshRateInHz      =	D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval            = D3DPRESENT_INTERVAL_IMMEDIATE;

		D3DFORMAT format, altFormat;

		if(m_bpp == 32) 
		{
			format    = D3DFMT_X8R8G8B8;
			altFormat = D3DFMT_X8R8G8B8;
		}
		if(m_bpp == 24) 
		{
			format    = D3DFMT_R8G8B8;
			altFormat = D3DFMT_R8G8B8;
		}
		if(m_bpp == 16) 
		{
			format    = D3DFMT_R5G6B5;
			altFormat = D3DFMT_X1R5G5B5;
		}
		if(m_bpp == 8) 
		{
			format    = D3DFMT_P8;
			altFormat = D3DFMT_P8;
		}
		if(_CheckFormat(format, FALSE, TRUE))
		{
			m_bHAL = TRUE;
		}
		else
		{
			if(_CheckFormat(altFormat, FALSE, TRUE))
			{
				m_bHAL = TRUE;
				format = altFormat;
			}
			else
			{
				if(!_CheckFormat(format, FALSE, FALSE))
				{
					if(!_CheckFormat(altFormat, FALSE, FALSE))
					{
						_WriteError(_T("没有支持的格式"));
						return FALSE;
					}
					else
					{
						format = altFormat;
					}
				}
			}
		}
		d3dpp.BackBufferFormat = format;
	}

	d3dpp.EnableAutoDepthStencil     = TRUE; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D16;  // 或者32，不过32更慢

	D3DDEVTYPE deviceType = m_bHAL ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF;
	D3DCAPS9 caps;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	if(FAILED(m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		deviceType,
		hWnd,
		vp,
		&d3dpp,
		&m_pDevice)))
	{
		_WriteError(_T("创建设备失败"));
		return FALSE;
	}


	EnableZBuffer(FALSE);
	EnableLighting(FALSE);
	EnableAlphaBlending(FALSE);
	//EnableAlphaTesting(FALSE);

	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE );

	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	SetAmbientLight(255, 255, 255);

	SetPerspective(D3DX_PI * 0.25f, (float)m_width/(float)m_height, 1.0f, 10000.0f);

	if(FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		_WriteError(_T("CreateSprite eorror"));
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::ClearZBuffer(float zBuffer)
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, zBuffer, 0)))
	{
		_WriteError(_T("清除画面失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::ClearDisplay(DWORD color)
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->Clear(0, NULL,  D3DCLEAR_TARGET, 0, 1.0f, 0)))
	{
		_WriteError(_T("清除画面失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::Clear(DWORD color, FLOAT zBuffer)
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(!m_bZBuffer)
	{
		return this->ClearDisplay(color);
	}

	if(FAILED(m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, zBuffer, 0)))
	{
		_WriteError(_T("清除画面失败"));
		return FALSE;
	}

	return TRUE;
}



BOOL CGraphics::BeginScene()
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->BeginScene()))
	{
		_WriteError(_T("绘制缓冲画面失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::EndScene()
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->EndScene()))
	{
		_WriteError(_T("关闭缓冲画面失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::Display()
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->Present(NULL, NULL, NULL, NULL)))
	{
		//_WriteError(_T("显示主画面失败"));
		return FALSE;
	}

	return TRUE;
}

BYTE CGraphics::_GetFormatBPP(const D3DFORMAT &format)
{
	switch(format) 
	{
		// 32 位模式
	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:

		return 32;
		break;

		// 24 位模式
	case D3DFMT_R8G8B8:

		return 24;
		break;

		// 16 位模式
	case D3DFMT_R5G6B5:
	case D3DFMT_X1R5G5B5:
	case D3DFMT_A1R5G5B5:
	case D3DFMT_A4R4G4B4:

		return 16;
		break;

		// 8 位模式
	case D3DFMT_A8P8:
	case D3DFMT_P8:

		return 8;
		break;

	default:
		return 0;
	}
}

BOOL CGraphics::_CheckFormat(const D3DFORMAT &format, BOOL bWindowed, BOOL bHAL)
{
	if(FAILED(m_pD3D->CheckDeviceType(
		D3DADAPTER_DEFAULT, 
		(bHAL == TRUE) ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF,    
		format, 
		format, 
		bWindowed)))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::SetPerspective(FLOAT fov, FLOAT aspect, FLOAT nz, FLOAT fz)
{
	D3DXMATRIX matProjection;

	if(m_pDevice == NULL)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	D3DXMatrixPerspectiveFovLH(&matProjection, fov, aspect, nz, fz);

	if(FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &matProjection)))
	{
		_WriteError(_T("变换失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::SetAmbientLight(BYTE red, BYTE green, BYTE blue)
{
	D3DCOLOR Color;

	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	Color = D3DCOLOR_XRGB((m_ambientRed = red), (m_ambientGreen = green), (m_ambientBlue = blue));

	if(FAILED(m_pDevice->SetRenderState(D3DRS_AMBIENT, Color)))
	{
		_WriteError(_T("设置环境光失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::SetShadeMode(DWORD shadeMode)
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->SetRenderState(D3DRS_SHADEMODE, shadeMode)))
	{
		_WriteError(_T("设置环境光失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::SetFillMode(DWORD fillMode)
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->SetRenderState(D3DRS_FILLMODE, fillMode)))
	{
		_WriteError(_T("设置环境光失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::SetLignt(DWORD num, const CLight &light)
{
	
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->SetLight(num, light.GetLight())))
	{
		_WriteError(_T("设置光照失败"));
		return FALSE;
	}
	return TRUE;
}

BOOL CGraphics::SetWorldPos(const D3DXMATRIX &worldMatrix)
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &worldMatrix)))
	{
		_WriteError(_T("转换世界坐标失败"));
		return FALSE;
	}
	return TRUE;
}

BOOL CGraphics::SetWorldPos(CPosition &pos)
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &pos.GetMatrix())))
	{
		_WriteError(_T("转换世界坐标失败"));
		return FALSE;
	}
	return TRUE;
}

BOOL CGraphics::SetCamera(CCamera &camera)
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	//camera.

	if(FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &camera.GetMatrix())))
	{
		_WriteError(_T("相机坐标转换失败"));
		return FALSE;
	}
	return TRUE;
}

BOOL CGraphics::EnableLight(DWORD num, BOOL bEnable)
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->LightEnable(num, bEnable)))
	{
		_WriteError(_T("设置光源失败"));
		return FALSE;
	}
	return TRUE;
}

BOOL CGraphics::EnableLighting(BOOL bEnable)
{
	if(m_pDevice == NULL)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, bEnable)))
	{
		_WriteError(_T("设置光照失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::EnableZBuffer(BOOL bEnable)
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->SetRenderState(D3DRS_ZENABLE, bEnable  ? D3DZB_TRUE : D3DZB_FALSE)))
	{
		_WriteError(_T("无法使用Z缓存"));
		return FALSE;
	}

	m_bZBuffer = bEnable;

	return TRUE;
}

BOOL CGraphics::EnableAlphaBlending(BOOL bEnable, DWORD srcValue, DWORD destValue)
{
	if(NULL == m_pDevice)
	{
		_WriteError(_T("未初始化DX设备"));
		return FALSE;
	}

	if(FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, bEnable)))
	{
		_WriteError(_T("设置alpha混合失败"));
		return FALSE;
	}

	if(bEnable)
	{
		m_pDevice->SetRenderState(D3DRS_SRCBLEND,  srcValue);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, destValue);
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//
// CVectorBuffer
//
///////////////////////////////////////////////////////////////////////////////////////////////

const DWORD WireVertex::FVF   = D3DFVF_XYZ;
const DWORD ColorVertex::FVF  = D3DFVF_XYZ | D3DFVF_DIFFUSE;
const DWORD NormalVertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
const DWORD Vertex::FVF       = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
const DWORD TexVertex2D::FVF  = D3DFVF_XYZRHW | D3DFVF_TEX1;
const DWORD TexVertex3D::FVF  = D3DFVF_XYZ | D3DFVF_TEX1; 

CVertex::CVertex()
{
	m_pGraphics = NULL;
	m_pVB = NULL;
	m_pIB = NULL;
	m_pBuffer = NULL;
	m_vertexNum = 0;
	m_vertexSize = 0;
	m_FVF = 0;
}

CVertex::~CVertex()
{
	this->Destroy();
}

BOOL CVertex::CreateBuffer(CGraphics *pGraphics, DWORD vertexNum, DWORD faceNum, DWORD vertexSize, DWORD FVF)
{
	if(NULL == (m_pGraphics = pGraphics))
	{
		_WriteError(_T("图形指针为正确传入"));
		return FALSE;
	}

	if(NULL == m_pGraphics->GetDevice())
	{
		_WriteError(_T("设备未初始化"));
		return FALSE;
	}

	if(0 == (m_vertexNum = vertexNum) || 
	   0 == (m_vertexSize = vertexSize) || 
	   0 == (m_FVF = FVF) ||
	   0 == (m_faceNum = faceNum))
	{
		_WriteError(_T("参数不正确"));
		return FALSE;
	}

	if(FAILED(m_pGraphics->GetDevice()->CreateVertexBuffer(
		m_vertexNum * m_vertexSize,
		D3DUSAGE_WRITEONLY,
		m_FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		0)))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CVertex::CreateIndex(CGraphics *pGraphics, DWORD indexNum, DWORD indexSize, D3DFORMAT format)
{

	if(NULL == (m_pGraphics = pGraphics))
	{
		_WriteError(_T("图形指针为正确传入"));
		return FALSE;
	}

	if(NULL == m_pGraphics->GetDevice())
	{
		_WriteError(_T("设备未初始化"));
		return FALSE;
	}

	if(0 == (m_indexNum = indexNum) || 
	   0 == (m_indexSize = indexSize))
	{
		_WriteError(_T("参数不正确"));
		return FALSE;
	}

	if(FAILED(m_pGraphics->GetDevice()->CreateIndexBuffer(
		m_indexNum * m_indexSize,
		D3DUSAGE_WRITEONLY,
		format,
		D3DPOOL_MANAGED,
		&m_pIB,
		0)))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CVertex::Destroy()
{
	UnlockBuffer();
	ReleaseCOM(m_pVB);
	UnlockIndex();
	ReleaseCOM(m_pIB);
	m_pGraphics = NULL;
	m_pVB = NULL;
	m_vertexNum = 0;
	m_vertexSize = 0;
	m_FVF = 0;
	return TRUE;
}

void *CVertex::LockBuffer(DWORD startVertex, DWORD vertexNum)
{
	if(NULL == m_pVB)
	{
		_WriteError(_T("顶点数组没有创建"));
		return NULL;
	}

	if(FAILED(m_pVB->Lock(
		startVertex * m_vertexNum, 
		vertexNum * m_vertexSize,
		&m_pBuffer,
		D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD)))
	{
		_WriteError(_T("顶点锁定失败"));
		return NULL;
	}

	return m_pBuffer;
}

void *CVertex::LockIndex(DWORD startIndex, DWORD indexNum)
{
	if(NULL == m_pIB)
	{
		_WriteError(_T("顶点数组没有创建"));
		return NULL;
	}

	if(FAILED(m_pIB->Lock(
		startIndex * m_indexNum, 
		indexNum * m_indexSize,
		&m_pIndex,
		D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD)))
	{
		_WriteError(_T("顶点锁定失败"));
		return NULL;
	}

	return m_pIndex;
}

BOOL CVertex::UnlockBuffer()
{
	if(NULL == m_pVB)
	{
		//_WriteError(_T("顶点数组没有创建"));
		return FALSE;
	}

	if(FAILED(m_pVB->Unlock()))
	{
		_WriteError(_T("顶点解除锁定失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CVertex::UnlockIndex()
{
	if(NULL == m_pIB)
	{
		//_WriteError(_T("顶点数组没有创建"));
		return FALSE;
	}

	if(FAILED(m_pIB->Unlock()))
	{
		_WriteError(_T("顶点解除锁定失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CVertex::SetBuffer(DWORD startVertex, DWORD vertexNum, void* vertexList)
{
	if(NULL == m_pGraphics || NULL == m_pVB)
	{
		_WriteError(_T("顶点缓存没有被正确创建"));
		return FALSE;
	}

	if(NULL == m_pGraphics)
	{
		_WriteError(_T("设备未初始化"));
		return FALSE;
	}

	if(0 == vertexNum || NULL == vertexList)
	{
		_WriteError(_T("参数不正确"));
		return FALSE;
	}

	if(NULL == LockBuffer(startVertex, vertexNum))
	{
		return FALSE;
	}

	memcpy(m_pBuffer, vertexList, vertexNum * m_vertexSize);

	if(!UnlockBuffer())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CVertex::SetIndex(DWORD startIndex, DWORD indexNum, void *indexList)
{
	if(NULL == m_pGraphics || NULL == m_pIB)
	{
		_WriteError(_T("顶点缓存没有被正确创建"));
		return FALSE;
	}

	if(NULL == m_pGraphics)
	{
		_WriteError(_T("设备未初始化"));
		return FALSE;
	}

	if(0 == indexNum || NULL == indexList)
	{
		_WriteError(_T("参数不正确"));
		return FALSE;
	}

	if(NULL == LockIndex(startIndex, indexNum))
	{
		return FALSE;
	}

	memcpy(m_pIndex, indexList, indexNum * m_indexSize);

	if(!UnlockIndex())
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CVertex::Render(DWORD startVertex, UINT primitiveCount, D3DPRIMITIVETYPE type, CTexture *pTexture)
{
	if(NULL == m_pGraphics || NULL == m_pVB)
	{
		_WriteError(_T("顶点缓存没有被正确创建"));
		return FALSE;
	}

	if(NULL == m_pGraphics->GetDevice())
	{
		_WriteError(_T("设备未初始化"));
		return FALSE;
	}

	m_pGraphics->GetDevice()->SetFVF(m_FVF);
	m_pGraphics->GetDevice()->SetStreamSource(0, m_pVB, 0, m_vertexSize);

	if(m_pIB)
	{
		m_pGraphics->GetDevice()->SetIndices(m_pIB);
	}

	if(NULL != pTexture)
	{
		if(NULL == pTexture->GetTexture())
		{
			_WriteError(_T("纹理未初始化"));
			return FALSE;
		}
		if(FAILED(m_pGraphics->GetDevice()->SetTexture(0, pTexture->GetTexture())))
		{
			_WriteError(_T("纹理绘制失败"));
			return FALSE;
		}
	}

	if(m_pIB)
	{
		m_pGraphics->GetDevice()->DrawIndexedPrimitive(type, 0, 0, m_vertexNum, startVertex, primitiveCount);
	}
	else
	{
		DWORD faceVerNum = m_vertexNum / m_faceNum;

		for(DWORD i=0; i<m_faceNum; i++)
		{
			// pirmitiveCount指每个面的三角形数
			m_pGraphics->GetDevice()->DrawPrimitive(type, startVertex + i*faceVerNum, primitiveCount);
		}
	}

	if(NULL != pTexture)
	{
		if(FAILED(m_pGraphics->GetDevice()->SetTexture(0, NULL)))
		{
			_WriteError(_T("纹理清空失败"));
			return FALSE;
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// CTexture
//
//////////////////////////////////////////////////////////////////////////////////////////

CTexture::CTexture()
{
	m_pGraphics = NULL;
	m_bLoaded   = FALSE;

	memset((void*)&m_d3dsd, 0, sizeof(m_d3dsd));

}

CTexture::~CTexture()
{

}

BOOL CTexture::Destroy()
{
	m_pGraphics = NULL;
	m_bLoaded   = FALSE;

	ReleaseCOM(m_pTexture);
	memset((void*)&m_d3dsd, 0, sizeof(m_d3dsd));

	return TRUE;
}

BOOL CTexture::LoadFromFile(CGraphics *pGraphics, LPCTSTR fileName, DWORD colorKey, D3DFORMAT format)
{
	if(NULL == (m_pGraphics = pGraphics) || NULL == fileName)
	{
		_WriteError(_T("参数指针为空"));
		return FALSE;
	}

	if(NULL == m_pGraphics->GetDevice())
	{
		_WriteError(_T("图形设备未初始化"));
		return FALSE;
	}

	if(FAILED(D3DXCreateTextureFromFileEx(
		pGraphics->GetDevice(), 
		fileName,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		format,
		D3DPOOL_MANAGED,
		D3DX_FILTER_TRIANGLE,
		D3DX_FILTER_TRIANGLE,
		colorKey,
		NULL, 
		NULL,
		&m_pTexture)))
	{
		_WriteError(_T("加载纹理失败"));
		return FALSE;
	}

	if(FAILED(m_pTexture->GetLevelDesc(0, &m_d3dsd)))
	{
		_WriteError(_T("获取纹理信息失败"));
		return FALSE;
	}

	m_bLoaded = TRUE;


	return TRUE;
}

BOOL CTexture::Blt(long destX, long destY, 
				   long srcX, long srcY, 
				   long width, long height, 
				   FLOAT xScale, FLOAT yScale, 
				   D3DCOLOR color)
{

	if(NULL == m_pTexture)
	{
		_WriteError(_T("纹理没有被初始化"));
		return FALSE;
	}

	if(NULL == m_pGraphics)
	{
		_WriteError(_T("图形设备初始化"));
		return FALSE;
	}

	ID3DXSprite *pSprite = m_pGraphics->GetSprite();

	if(NULL == pSprite)
	{
		_WriteError(_T("DX精灵没有被初始化"));
		return FALSE;
	}

	// 默认为纹理本身的大小

	if(0 == width)
	{
		width = m_d3dsd.Width;
	}
	if(0 == height)
	{
		height = m_d3dsd.Height;
	}

	RECT srcRect;

	srcRect.left   = srcX;
	srcRect.top    = srcY;
	srcRect.right  = srcX + width;
	srcRect.bottom = srcY + height;

	

	/*
	// 这是dx9.0b的方法
	if(FAILED(pSprite->Draw(m_Texture,                         
         &Rect, &D3DXVECTOR2(XScale, YScale),       
         NULL, 0.0f,                                         
         &D3DXVECTOR2((float)DestX, (float)DestY), 
         Color)))
    return FALSE;
	*/

	// 这是9.0c的方法

	D3DXMATRIX mat;

	D3DXMatrixTransformation2D(
		&mat, 
		NULL,
		0.0f, 
		&D3DXVECTOR2(xScale, yScale),
		NULL, 
		0, 
		&D3DXVECTOR2((float)destX, (float)destY));

	pSprite->SetTransform(&mat);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	if(FAILED(pSprite->Draw(
		m_pTexture,
		&srcRect,
		NULL,
		NULL,
		color)))
	{
		_WriteError(_T("绘制精灵画面失败"));
		return FALSE;
	}

	pSprite->End();

	return TRUE;
}

BOOL  CTexture::Rotate(float centerX, float centerY, FLOAT angle)
{
	if(NULL == m_pTexture)
	{
		_WriteError(_T("纹理没有被初始化"));
		return FALSE;
	}

	if(NULL == m_pGraphics)
	{
		_WriteError(_T("图形设备初始化"));
		return FALSE;
	}

	ID3DXSprite *pSprite = m_pGraphics->GetSprite();

	if(NULL == pSprite)
	{
		_WriteError(_T("DX精灵没有被初始化"));
		return FALSE;
	}


	D3DXMATRIX mat;

	D3DXMatrixTransformation2D(
		&mat, 
		NULL,
		0.0f, 
		NULL,
		&D3DXVECTOR2(centerX, centerY), 
		angle, 
		NULL);

	pSprite->SetTransform(&mat);

	return TRUE;
}
BOOL  CTexture::Scale(float centerX, float centerY, FLOAT scaleX, FLOAT scaleY)
{
	if(NULL == m_pTexture)
	{
		_WriteError(_T("纹理没有被初始化"));
		return FALSE;
	}

	if(NULL == m_pGraphics)
	{
		_WriteError(_T("图形设备初始化"));
		return FALSE;
	}

	ID3DXSprite *pSprite = m_pGraphics->GetSprite();

	if(NULL == pSprite)
	{
		_WriteError(_T("DX精灵没有被初始化"));
		return FALSE;
	}


	D3DXMATRIX mat;

	D3DXMatrixTransformation2D(
		&mat, 
		&D3DXVECTOR2(centerX, centerY),
		0.0f, 
		&D3DXVECTOR2(scaleX, scaleY),
		NULL, 
		0.0f, 
		NULL);

	pSprite->SetTransform(&mat);

	return TRUE;
}

BOOL  CTexture::Translation(float x, float y)
{
	if(NULL == m_pTexture)
	{
		_WriteError(_T("纹理没有被初始化"));
		return FALSE;
	}

	if(NULL == m_pGraphics)
	{
		_WriteError(_T("图形设备初始化"));
		return FALSE;
	}

	ID3DXSprite *pSprite = m_pGraphics->GetSprite();

	if(NULL == pSprite)
	{
		_WriteError(_T("DX精灵没有被初始化"));
		return FALSE;
	}


	D3DXMATRIX mat;

	D3DXMatrixTransformation2D(
		&mat, 
		NULL,
		0.0f, 
		NULL,
		NULL, 
		0.0f, 
		&D3DXVECTOR2(x, y));

	pSprite->SetTransform(&mat);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CFont
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

CFont::CFont()
{
	m_pFont = NULL;
}

CFont::~CFont()
{
	this->Destroy();
}

void CFont::Destroy()
{
	ReleaseCOM(m_pFont);
	
}

BOOL CFont::Create(CGraphics *pGraphics, INT height, LPCTSTR  fontName, BOOL bItalic)
{
	if(NULL == pGraphics)
	{
		_WriteError(_T("图形设备指针为空"));
		return FALSE;
	}

	D3DXFONT_DESC lf;
	ZeroMemory(&lf, sizeof(lf));

	if(NULL == fontName)
	{
		_tcscpy (lf.FaceName, _T("Times New Roman"));
	}
	else
	{
		_tcscpy (lf.FaceName, fontName);
	}

	lf.Height = height;

	lf.Italic = bItalic;

	if(FAILED(D3DXCreateFontIndirect(pGraphics->GetDevice(), &lf, &m_pFont)))
	{
		_WriteError(_T("D3DXCreateFontIndirect failed"));
		return FALSE;
	}

	return TRUE;
}

BOOL CFont::Print(LPCTSTR text, long xPos, long yPos, long width, long height, DWORD format, D3DCOLOR color)
{
	if(NULL == m_pFont)
	{
		_WriteError(_T("字体没有被创建"));
		return FALSE;
	}

	RECT rc = {xPos, yPos, xPos + width, yPos + height};

	if(FAILED(m_pFont->DrawText(NULL, text, -1, &rc, format, color)))
	{
		_WriteError(_T("绘制文字失败"));
		return FALSE;
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CLight
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

CLight::CLight()
{
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
	SetType(D3DLIGHT_POINT);
	MoveTo(0.0f, 0.0f, 0.0f);
	SetDiffuseColor(255,255,255);
	SetAmbientColor(255,255,255);
	SetRange(1000.0f);
}

CLight::~CLight()
{

}

void CLight::SetAmbientColor(BYTE red, BYTE green, BYTE blue)
{
	m_light.Ambient.r = 1.0f / 255.0f * (float)red;
	m_light.Ambient.g = 1.0f / 255.0f * (float)green;
	m_light.Ambient.b = 1.0f / 255.0f * (float)blue;
}
void CLight::SetDiffuseColor(BYTE red, BYTE green, BYTE blue)
{
	m_light.Diffuse.r = 1.0f / 255.0f * (float)red;
	m_light.Diffuse.g = 1.0f / 255.0f * (float)green;
	m_light.Diffuse.b = 1.0f / 255.0f * (float)blue;
}
void CLight::SetSpecularColor(BYTE red, BYTE green, BYTE blue)
{
	m_light.Specular.r = 1.0f / 255.0f * (float)red;
	m_light.Specular.g = 1.0f / 255.0f * (float)green;
	m_light.Specular.b = 1.0f / 255.0f * (float)blue;
}

void CLight::MoveTo(float xPos, float yPos, float zPos)
{
	m_light.Position.x = xPos;
	m_light.Position.y = yPos;
	m_light.Position.z = zPos;
}

void CLight::Move(float xPos, float yPos, float zPos)
{
	m_light.Position.x += xPos;
	m_light.Position.y += yPos;
	m_light.Position.z += zPos;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CPositon
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

CPosition::CPosition()
{
	MoveTo(0.0f,0.0f,0.0f);
	RotateTo(0.0f,0.0f,0.0f);
	ScaleTo(1.0f, 1.0f, 1.0f);
	Update();
}
CPosition::~CPosition()
{

}

void CPosition::MoveTo(float xPos, float yPos, float zPos)
{
	m_xPos = xPos;
	m_yPos = yPos;
	m_zPos = zPos;

	D3DXMatrixTranslation(&m_matTranslation, m_xPos, m_yPos, m_zPos);
}

void CPosition::Move(float xPosAdd, float yPosAdd, float zPosAdd)
{
	this->MoveTo(m_xPos + xPosAdd, m_yPos + yPosAdd, m_zPos + zPosAdd);
}

void CPosition::RotateTo(float xRot, float yRot, float zRot)
{
	m_xRot = xRot;
	m_yRot = yRot;
	m_zRot = zRot;

	D3DXMatrixRotationYawPitchRoll(&m_matRotation, m_yRot, m_xRot, m_zRot);
}

void CPosition::Rotate(float xRotAdd, float yRotAdd, float zRotAdd)
{
	this->RotateTo(m_xRot + xRotAdd, m_yRot + yRotAdd, m_zRot + zRotAdd);
}

void CPosition::ScaleTo(float xScale, float yScale, float zScale)
{
	m_xScale = xScale;
	m_yScale = yScale;
	m_zScale = zScale;

	D3DXMatrixScaling(&m_matScale, xScale, yScale, zScale);
}

void CPosition::Scale(float xScaleAdd, float yScaleAdd, float zScaleAdd)
{
	ScaleTo(m_xScale + xScaleAdd, m_yScale + yScaleAdd, m_zScale + zScaleAdd);
}

void CPosition::Update()
{
	m_matWorld = m_matScale  * m_matRotation * m_matTranslation;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CCamera
//
///////////////////////////////////////////////////////////////////////////////////////////////////

CCamera::CCamera()
{
	
}

CCamera::~CCamera()
{

}
void CCamera::MoveTo(float xPos, float yPos, float zPos)
{
	m_xPos = xPos;
	m_yPos = yPos;
	m_zPos = zPos;

	D3DXMatrixTranslation(&m_matTranslation, -m_xPos, -m_yPos, -m_zPos);
}
void CCamera::Move(float x, float y, float z)
{
	MoveTo(m_xPos + x, m_yPos + y, m_zPos + z);
}
void CCamera::RotateTo(float xRot, float yRot, float zRot)
{
	D3DXMATRIX matXRotation, matYRotation, matZRotation;

	m_xRot = xRot;
	m_yRot = yRot;
	m_zRot = zRot;

	D3DXMatrixRotationX(&matXRotation, -m_xRot);
	D3DXMatrixRotationY(&matYRotation, -m_yRot);
	D3DXMatrixRotationZ(&matZRotation, -m_zRot);

	m_matRotation = matZRotation * matYRotation * matXRotation;
}
void CCamera::Rotate(float x, float y, float z)
{
	RotateTo(m_xRot + x, m_yRot + y, m_zRot + z);
}

void CCamera::SetLookAt(float xEye, float yEye, float zEye, float xAt, float yAt, float zAt)
{
	float xDiff = xAt - xEye;
	float yDiff = yAt - yEye;
	float zDiff = zAt - zEye;

	float xRot = (float)atan2(-yDiff, sqrt(xDiff*xDiff + zDiff * zDiff));
	float yRot = (float)atan2(xDiff, zDiff);

	MoveTo(xEye, yEye, zEye);
	RotateTo(xRot, yRot, 0.0f);
}

void CCamera::Update()
{
	m_matView = m_matTranslation * m_matRotation;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CMesh
//
////////////////////////////////////////////////////////////////////////////////////////////////

CMesh::CMesh()
{
	m_pFrames   = NULL;
	m_pMeshes   = NULL;
	m_pGraphics = NULL;

	m_frameNum  = 0;
	m_meshNum   = 0;
	m_radius    = 0.0f;

	m_min.x = m_min.y = m_min.z = m_max.x = m_max.y = m_max.z = 0.0f;
}

CMesh::~CMesh()
{
	Destroy();
}

void CMesh::Destroy()
{
	if(NULL != m_pFrames)
	{
		delete m_pFrames;
		m_pFrames = NULL;
	}

	// 不释放资源，只是个指针
	m_pMeshes   = NULL;
	m_pGraphics = NULL;

	m_frameNum  = 0;
	m_meshNum   = 0;
	m_radius    = 0.0f;

	m_min.x = m_min.y = m_min.z = m_max.x = m_max.y = m_max.z = 0.0f;

}

Frame *CMesh::GetFrame(LPSTR  pFrameName)
{
	if(NULL == m_pFrames)
	{
		return NULL;
	}

	return _FindFrame(m_pFrames, pFrameName);
}

BOOL CMesh::Render(CPosition *pos,  BOOL bTransformed)
{
	// 建立一个单位矩阵，更新根结点

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	// 将变换矩阵更新到帧矩阵中,这个就是遗传。。
	_UpdateFrames(m_pFrames, mat, bTransformed);

	// 将帧矩阵更新到骨骼矩阵，并更新绘制网格
	_UpdateMeshes();

	//绘制网格
	_DrawMeshes(pos);

	return TRUE;
}


BOOL CMesh::LoadXFile(CGraphics *pGraphics, const char *pFileName, const char *pTexturePath)
{
	if(NULL == pGraphics || NULL  == pGraphics->GetDevice() || NULL == pFileName)
	{
		_WriteError(_T("CMesh::LoadXFile 参数传入错误或设备未初始化"));
		return FALSE;
	}

	m_pGraphics = pGraphics;
	return _ParseXFile(pFileName, pTexturePath);
}

//-----------------------------------------------------------------------------------/
//
// CMesh 私有成员函数
//
//------------------------------------------------------------------------------------/

BOOL CMesh::_ParseXFile(const char *pFileName, const char *pTexturePath)
{
	ID3DXFile           *pDXFile = NULL;
	ID3DXFileEnumObject *pDXEnum = NULL;
	ID3DXFileData       *pDXData = NULL;

	//Frame *pFrame = NULL;

	// 创建一个DX文件
	if(FAILED(D3DXFileCreate(&pDXFile)))
	{
		_WriteError(_T("创建DX文件失败"));
		return FALSE;
	}

	// 注册.X文件模板
	if(FAILED(pDXFile->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES)))
	{
		_WriteError(_T("注册.X文件模板失败"));
		ReleaseCOM(pDXFile);
		return FALSE;
	}

	// 建立一个枚举对象 浏览.x文件中的顶级对象
	if(FAILED(pDXFile->CreateEnumObject((LPVOID)pFileName, DXFILELOAD_FROMFILE, &pDXEnum)))
	{
		_WriteError(_T("加载.X文件失败"));
		ReleaseCOM(pDXFile);
		return FALSE;
	}

	Frame *pTempFrame = new Frame;

	// 9.0c 与 9.0b 不同

	SIZE_T childNum;

	if(FAILED(pDXEnum->GetChildren(&childNum)))
	{
		_WriteError(_T("pDXEnum::GetChildren failed"));
		ReleaseCOM(pDXFile);
		ReleaseCOM(pDXEnum);
		return FALSE;
	}

	for(SIZE_T i = 0; i < childNum; i++)
	{
		if(FAILED(pDXEnum->GetChild(i, &pDXData)))
		{
			_WriteError(_T("pDXEnum::GetChild failed"));
			ReleaseCOM(pDXFile);
			ReleaseCOM(pDXEnum);
			return FALSE;
		}

		_ParseXFileData(pDXData, pTempFrame, pTexturePath);
	}

	ReleaseCOM(pDXEnum);
	ReleaseCOM(pDXFile);

	if(NULL != pTempFrame->_pMesh)
	{
		m_pFrames = pTempFrame;
		m_pFrames->_pName = new char[7];
		strcpy(m_pFrames->_pName, "%ROOT%");
	}
	else
	{
		m_pFrames = pTempFrame->_pChild;
		Frame *pFrame = m_pFrames;
		while(pFrame != NULL)
		{
			pFrame->_pParent = NULL;
			pFrame = pFrame->_pSibling;
		}
		pTempFrame->_pChild = NULL;
		delete pTempFrame;
		
	}

	// 将帧与网格联系在一起

	_MapFramesToBones(m_pFrames);

	// 计算边界箱
	Mesh *pList = m_pMeshes;

	while(NULL != pList)
	{
		m_min.x = min(m_min.x, pList->_min.x);
		m_min.y = min(m_min.y, pList->_min.y);
		m_min.z = min(m_min.z, pList->_min.z);

		m_max.x = max(m_max.x, pList->_max.x);
		m_max.y = max(m_max.y, pList->_max.y);
		m_max.z = max(m_max.z, pList->_max.z);

		m_radius = max(m_radius, pList->_radius);

		pList = pList->_pNext;
	}

	return TRUE;
}

void CMesh::_ParseXFileData(ID3DXFileData *pDXData, Frame *pParentFrame, const char *pTexturePath)
{
	GUID                       type;
	DWORD                      size            = 0;
	char                      *pName           = NULL;

	Frame                     *pFrame          = NULL;
	Frame                     *pSubFrame       = NULL;	// 保存pParentFrame指针
	Mesh                      *pMesh           = NULL;


	if(FAILED(pDXData->GetType(&type)))
	{
		return;
	}

	if(FAILED(pDXData->GetName(NULL, &size)))
	{
		return;
	}

	if(size > 0)
	{
		if((pName = new char[size]) != NULL)
		{
			pDXData->GetName(pName, &size);
		}
	}

	if(NULL == pName)
	{
		if(NULL == (pName = new char[9]))
		{
			_WriteError(_T("内存申请失败"));
			return;
		}
		
		strcpy(pName, "$NoName$");
	}

	pSubFrame = pParentFrame;

	if(type == TID_D3DRMFrame)
	{
		// 一个新的帧

		pFrame = new Frame;

		// 名字
		pFrame->_pName = pName;
		pName = NULL;

		// 二叉树,将此Frame加在parent的孩子指针上(第一个孩子)，
		pFrame->_pParent = pParentFrame;
		pFrame->_pSibling = pParentFrame->_pChild;
		pParentFrame->_pChild = pFrame;

		pSubFrame = pFrame;

		m_frameNum++;

	}

	if(type == TID_D3DRMMesh)
	{
		// 网格

		ID3DXBuffer *pMaterialBuffer = NULL;	// 存储材质
		ID3DXBuffer *pAdjacency      = NULL;	// 存储附属物，还没有用到

		pMesh = new Mesh;

		pMesh->_pName = pName;
		pName = NULL;

		if(FAILED(D3DXLoadSkinMeshFromXof(
			pDXData,
			0,
			m_pGraphics->GetDevice(),
			&pAdjacency,
			&pMaterialBuffer,
			NULL,
			&pMesh->_materialNum,
			&pMesh->_pSkinInfo,
			&pMesh->_pDXMesh)))
		{
			_WriteError(_T("D3DXLoadSkinMeshFromXof failed"));
			return;
		}

		// 保存边界球和边界盒

		BYTE *ptr;
		
		if(SUCCEEDED(pMesh->_pDXMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&ptr)))
		{
			
			D3DXComputeBoundingBox(
				(D3DXVECTOR3*)ptr, 
				pMesh->_pDXMesh->GetNumVertices(), 
				D3DXGetFVFVertexSize(pMesh->_pDXMesh->GetFVF()),	// 这里，这里
				&pMesh->_min,
				&pMesh->_max);

			
			D3DXComputeBoundingSphere(
				(D3DXVECTOR3*)ptr,
				pMesh->_pDXMesh->GetNumVertices(), 
				D3DXGetFVFVertexSize(pMesh->_pDXMesh->GetFVF()),
				&D3DXVECTOR3(0.0f,0.0f,0.0f),
				&pMesh->_radius);

				
			pMesh->_pDXMesh->UnlockVertexBuffer();
			
		}
		
		ReleaseCOM(pAdjacency);

		// 加载蒙皮网格模型

		if(pMesh->_pSkinInfo != NULL && pMesh->_pSkinInfo->GetNumBones() != 0)
		{
			if(FAILED(pMesh->_pDXMesh->CloneMeshFVF(
				0, 
				pMesh->_pDXMesh->GetFVF(),
				m_pGraphics->GetDevice(),
				&pMesh->_pDXSkinMesh)))
			{
				ReleaseCOM(pMesh->_pSkinInfo);
			}

			DWORD boneNum = pMesh->_pSkinInfo->GetNumBones();

			pMesh->_apFrameMatrices = new D3DXMATRIX*[boneNum];
			pMesh->_pBoneMatrices   = new D3DXMATRIX[boneNum];

			for(DWORD i=0; i<boneNum; i++)
			{
				// 这个留着全部加载完再复制

				pMesh->_apFrameMatrices[i] = NULL;

				// 网格的骨骼矩阵，默认为单位矩阵

				D3DXMatrixIdentity(&pMesh->_pBoneMatrices[i]);
			}
		}

		if(0 == pMesh->_materialNum)
		{
			// 没有加载到材质信息，则设置一个，默认材质

			pMesh->_pMaterials = new D3DMATERIAL9[1];
			pMesh->_apTextures = new LPDIRECT3DTEXTURE9[1];

			ZeroMemory(pMesh->_pMaterials, sizeof(D3DMATERIAL9));

			pMesh->_pMaterials[0].Diffuse.r = 1.0f;
			pMesh->_pMaterials[0].Diffuse.g = 1.0f;
			pMesh->_pMaterials[0].Diffuse.b = 1.0f;
			pMesh->_pMaterials[0].Diffuse.a = 1.0f;
			pMesh->_pMaterials[0].Ambient   = pMesh->_pMaterials[0].Diffuse;
			pMesh->_pMaterials[0].Specular  = pMesh->_pMaterials[0].Diffuse;

			pMesh->_apTextures[0] = NULL;

			pMesh->_materialNum = 1;
		}
		else
		{
			// 设置材质和纹理

			D3DXMATERIAL *pMaterials = (D3DXMATERIAL*) pMaterialBuffer->GetBufferPointer();

			pMesh->_pMaterials = new D3DMATERIAL9[pMesh->_materialNum];
			pMesh->_apTextures = new LPDIRECT3DTEXTURE9[pMesh->_materialNum];

			for(DWORD i=0; i<pMesh->_materialNum; i++)
			{
				pMesh->_pMaterials[i] = pMaterials[i].MatD3D;
				pMesh->_pMaterials[i].Ambient = pMesh->_pMaterials[i].Diffuse;

				char path[FILE_PATH_LENGTH];

				sprintf(path, "%s%s", pTexturePath, pMaterials[i].pTextureFilename);
#ifdef _UNICODE
				 int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, path, strlen(path), NULL, 0);
				 TCHAR *wpath = new TCHAR[wcsLen+1];
				 ::MultiByteToWideChar(CP_ACP, NULL, path, strlen(path), wpath, wcsLen);
				 wpath[wcsLen] = '\0';

				if(FAILED(D3DXCreateTextureFromFile(
					m_pGraphics->GetDevice(), 
					wpath, 
					&pMesh->_apTextures[i])))
				{
					_WriteError(_T("纹理加载失败"));
					pMesh->_apTextures[i] = NULL;
				}

				delete[] wpath;

				/////////////////////////////////////////////////
#else
				if(FAILED(D3DXCreateTextureFromFile(
					m_pGraphics->GetDevice(), 
					path, 
					&pMesh->_apTextures[i])))
				{
					_WriteError(_T("纹理加载失败"));
					pMesh->_apTextures[i] = NULL;
				}
#endif
			}
		} // else

		ReleaseCOM(pMaterialBuffer);


		m_meshNum++;

		// 将所有的mesh都连接在一起

		pMesh->_pNext = m_pMeshes;
		m_pMeshes = pMesh;

		if(NULL != pMesh)
		{
			_AddMeshToFrame(pParentFrame, pMesh);
		}
	} // TID_D3DRMMesh

	// 获取矩阵，，，原来是这样弄的。。。
	if(type == TID_D3DRMFrameTransformMatrix)
	{
		// 矩阵信息

		DWORD size = 0;
        LPCVOID buffer = NULL;

		if(FAILED(pDXData->Lock(&size, &buffer)))
		{
			return;
		}

		if(size == sizeof(D3DXMATRIX))
		{
			memcpy(pParentFrame->_matTransformed, buffer, size);
			pParentFrame->_matOriginal = pParentFrame->_matTransformed;
		}

		pDXData->Unlock();
	}

	if(NULL != pName)
	{
		delete [] pName;
	}
	
	if(type == TID_D3DRMAnimationSet || type == TID_D3DRMAnimation || type == TID_D3DRMAnimationKey)
	{
		// 跳过动画，直接返回
		return ;
	}

	

	ID3DXFileData             *pSubData        = NULL;

	SIZE_T childNum;

    pDXData->GetChildren(&childNum);

    for(SIZE_T i=0; i<childNum; i++)
    {
        pDXData->GetChild(i, &pSubData);

        _ParseXFileData(pSubData, pSubFrame, pTexturePath);

        ReleaseCOM(pSubData);
    }
}


Frame *CMesh::_FindFrame(Frame *pFrame, const char *pFrameName)
{
	if(NULL == pFrameName)
	{
		return NULL;
	}

	if(NULL == pFrame)
	{
		return NULL;
	}

	// 找到了
	if(NULL != pFrame->_pName && 0 == strcmp(pFrame->_pName, pFrameName))
	{
		return pFrame;
	}

	// 接着找
	Frame *pTempFrame = NULL;

	pTempFrame = _FindFrame(pFrame->_pChild, pFrameName);

	if(NULL != pTempFrame)
	{
		// 找到了
		return pTempFrame;
	}

	pTempFrame = _FindFrame(pFrame->_pSibling, pFrameName);

	if(NULL != pTempFrame)
	{
		// 找到了
		return pTempFrame;
	}

	// 没找到
	return NULL;
	
}

BOOL CMesh::_DrawMeshes(CPosition *pos)
{
	if(NULL == m_pMeshes)
	{
		_WriteError(_T("顶点网格没有初始化"));
		return FALSE;;
	}

	D3DXMATRIX matWorld;
	ID3DXMesh *pMeshToDraw = NULL;
	Mesh      *pCurMesh    = m_pMeshes;

	while(NULL != pCurMesh)
	{
		if(NULL != pCurMesh->_pDXSkinMesh)
		{
			if(NULL != pos)
			{
				m_pGraphics->SetWorldPos(*pos);
			}

			pMeshToDraw =  pCurMesh->_pDXSkinMesh;
		}
		else
		{
			if(NULL != pos)
			{
				matWorld = m_pFrames->_matCombined * pos->GetMatrix();
				m_pGraphics->SetWorldPos(matWorld);
			}	

			pMeshToDraw =  pCurMesh->_pDXMesh;

		}
		
		for(DWORD i = 0; i < pCurMesh->_materialNum; i++)
		{
			m_pGraphics->GetDevice()->SetMaterial(&pCurMesh->_pMaterials[i]);
			m_pGraphics->GetDevice()->SetTexture(0, pCurMesh->_apTextures[i]);

			if(pCurMesh->_pMaterials[i].Diffuse.a != 1.0f)
			{
				// 启动ALPHA混合

				m_pGraphics->EnableAlphaBlending(TRUE);
			}

			pMeshToDraw->DrawSubset(i);

			if(pCurMesh->_pMaterials[i].Diffuse.a != 1.0f)
			{
				m_pGraphics->EnableAlphaBlending(FALSE);
			}
		}

		pCurMesh = pCurMesh->_pNext;
	}

	return TRUE;
}

void CMesh::_AddMeshToFrame(Frame *pFrame, Mesh *pMesh)
{
	// 将mesh加到该frame的列表中

	pMesh->_pNext = pFrame->_pMesh;
	pFrame->_pMesh = pMesh;
}

void CMesh::_ResetFrame(Frame *pFrame)
{
	// 将所有的帧置为开始值

	pFrame->_matTransformed = pFrame->_matOriginal;

	if(NULL != pFrame->_pChild)
	{
		_ResetFrame(pFrame->_pChild);
	}
	if(NULL != pFrame->_pSibling)
	{
		_ResetFrame(pFrame->_pSibling);
	}
}

BOOL CMesh::_UpdateMeshes()
{
	// 此函数用于更新网格，如果mesh关联了animation，
	// 则用animation的帧来更新mesh的骨骼矩阵，以用于显示

	if(NULL == m_pMeshes)
	{
		return FALSE;
	}

	Mesh *pMesh = m_pMeshes;

	while(NULL != pMesh)
	{

		if(NULL == pMesh->_pDXMesh || NULL == pMesh->_pDXSkinMesh || NULL == pMesh->_pSkinInfo)
		{
			return FALSE;
		}

		if(NULL == pMesh->_pBoneMatrices || NULL == pMesh->_apFrameMatrices)
		{
			return FALSE;
		}

		for(DWORD i=0; i<pMesh->_pSkinInfo->GetNumBones(); i++)
		{
			D3DXMATRIX *mat = pMesh->_pSkinInfo->GetBoneOffsetMatrix(i);

			if(NULL != pMesh->_apFrameMatrices[i])
			{
				// 如果加载了animation将骨骼矩阵置为偏移矩阵与变换矩阵的乘积

				pMesh->_pBoneMatrices[i] = (*mat) * (*pMesh->_apFrameMatrices[i]);
			}
			else
			{
				// 否则，将骨骼矩阵置为单位矩阵

				D3DXMatrixIdentity(&pMesh->_pBoneMatrices[i]);
			}
		}

		// 用骨骼矩阵更新绘制网格

		void* source = NULL;
		void* dest = NULL;

		pMesh->_pDXMesh->LockVertexBuffer(0, &source);
		pMesh->_pDXSkinMesh->LockVertexBuffer(0, &dest);

		pMesh->_pSkinInfo->UpdateSkinnedMesh(pMesh->_pBoneMatrices, NULL, source, dest);

		pMesh->_pDXSkinMesh->UnlockVertexBuffer();
		pMesh->_pDXMesh->UnlockVertexBuffer();

		// 下一个

		pMesh = pMesh->_pNext;
	}

	return TRUE;
}

void CMesh::_UpdateFrames(Frame *pFrame,const D3DXMATRIX &mat, BOOL bTransformed)
{
	// 给合成矩阵(matCombined)赋值，在有动画的前提下，此矩阵与mesh中的帧矩阵关联

	if(NULL == pFrame)
	{
		return;
	}

	if(bTransformed)
	{
		// 使用变换矩阵更新

		pFrame->_matCombined = pFrame->_matTransformed * mat;
	}
	else
	{
		// 使用原始矩阵更新

		pFrame->_matCombined = pFrame->_matOriginal * mat;
	}

	// 如果是子帧，则用当前父帧的combine来更新，
	// 如果为兄弟帧，则用形参传入的矩阵来更新

	_UpdateFrames(pFrame->_pChild, pFrame->_matCombined, bTransformed);

	_UpdateFrames(pFrame->_pSibling, mat, bTransformed);
}


void CMesh::_MapFramesToBones(Frame *pFrame)
{
	// 将帧的matCombined关联到mesh的_apFrameMatrices上，这样就做到了同步更新

	if(NULL == pFrame || NULL == pFrame->_pName)
	{
		return;
	}

	Mesh *pMesh = m_pMeshes;
	DWORD boneNum = 0;

	while(NULL != pMesh)
	{
		boneNum = pMesh->_pSkinInfo->GetNumBones();

		if(boneNum != 0 && NULL != pMesh->_pSkinInfo && NULL != pMesh->_apFrameMatrices)
		{

			for(DWORD i=0; i<boneNum; i++)
			{
				const char *name = pMesh->_pSkinInfo->GetBoneName(i);
				if(0 == strcmp(pFrame->_pName,name))
				{
					pMesh->_apFrameMatrices[i] = &pFrame->_matCombined;

					// 这块骨头已经找到，下一块！
					break;
				}
			}
		}

		pMesh = pMesh->_pNext;
	}

	_MapFramesToBones(pFrame->_pChild);
	_MapFramesToBones(pFrame->_pSibling);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CAnimationSet
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

CAnimation::CAnimation()
{
	m_pAnimSet      = NULL;
	m_nAnimSet      = 0;
	m_animStartTime = 0;
	m_pCurAnimSet   = NULL;
}

CAnimation::~CAnimation()
{
	Destroy();
}

void CAnimation::Destroy()
{
	m_nAnimSet      = 0;
	m_pCurAnimSet   = NULL;
	m_animStartTime = 0;

	if(NULL != m_pAnimSet)
	{
		delete m_pAnimSet;
		m_pAnimSet = NULL;
	}
}

BOOL CAnimation::SetLoop(const char *pAnimSetName, BOOL bLoop)
{
	// 设置某个动画集的循环状态

	AnimationSet *pAnimSet = _FindAnimSet(m_pAnimSet, pAnimSetName);

	if(NULL == pAnimSet)
	{
		return FALSE;
	}

	Animation    *pAnim = pAnimSet->_pAnim;

	while(NULL != pAnim)
	{
		pAnim->_bLoop = bLoop;
		pAnim = pAnim->_pNext;
	}

	return TRUE;
}

BOOL CAnimation::SetCurAnimSet(const char *pAnimSetName, DWORD startTime)
{
	// 设置当前用于显示的动画集

	if(NULL == pAnimSetName)
	{
		return FALSE;
	}

	m_pCurAnimSet = _FindAnimSet(m_pAnimSet, pAnimSetName);

	if(NULL == m_pCurAnimSet)
	{
		return FALSE;
	}

	m_animStartTime = startTime;

	return TRUE;
}

BOOL CAnimation::UpdateCurAnim(DWORD time, BOOL bSmooth)
{
	_UpdateAnimationSet(m_pCurAnimSet, time, bSmooth);

	return TRUE;
}

BOOL CAnimation::LoadXFile(const char *fileName, CMesh *pMapMesh)
{
	ID3DXFile           *pDXFile = NULL;
	ID3DXFileEnumObject *pDXEnum = NULL;
	ID3DXFileData       *pDXData = NULL;

	if(NULL == fileName)
	{
		_WriteError(_T("文件名为空"));
		return FALSE;
	}

	// 创建一个DX文件
	if(FAILED(D3DXFileCreate(&pDXFile)))
	{
		_WriteError(_T("创建DX文件失败"));
		return FALSE;
	}

	// 注册.X文件模板
	if(FAILED(pDXFile->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES)))
	{
		_WriteError(_T("注册.X文件模板失败"));
		ReleaseCOM(pDXFile);
		return FALSE;
	}

	// 建立一个枚举对象 浏览.x文件中的顶级对象
	if(FAILED(pDXFile->CreateEnumObject((LPVOID)fileName, DXFILELOAD_FROMFILE, &pDXEnum)))
	{
		_WriteError(_T("加载.X文件失败"));
		ReleaseCOM(pDXFile);
		return FALSE;
	}

	// 9.0c 与 9.0b 不同

	SIZE_T childNum;

	if(FAILED(pDXEnum->GetChildren(&childNum)))
	{
		_WriteError(_T("pDXEnum::GetChildren failed"));
		ReleaseCOM(pDXFile);
		ReleaseCOM(pDXEnum);
		return FALSE;
	}

	for(SIZE_T i = 0; i < childNum; i++)
	{
		if(FAILED(pDXEnum->GetChild(i, &pDXData)))
		{
			_WriteError(_T("pDXEnum::GetChild failed"));
			ReleaseCOM(pDXFile);
			ReleaseCOM(pDXEnum);
			return FALSE;
		}
		
		// NULL 代表这是根

		_ParseXFileData(pDXData, NULL, NULL);
	}

	ReleaseCOM(pDXEnum);
	ReleaseCOM(pDXFile);

	if(NULL != pMapMesh)
	{
		// 将骨骼帧与网格类的帧关联在一起

		_MapToMesh(pMapMesh);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------/
//
// CAnimation私有成员函数
//
//------------------------------------------------------------------------------------/

void CAnimation::_ParseXFileData(ID3DXFileData *pDXData, AnimationSet *pParentSet, Animation *pCurAnim)
{
	GUID   type;
	SIZE_T size;
	LPSTR  pName = NULL;

	// 各种下一次迭代的父指针

	AnimationSet *pSubAnimSet = NULL;
	Animation    *pSubAnim    = NULL;

	AnimationSet *pAnimSet    = NULL;
	Animation    *pAnim       = NULL;

	if(FAILED(pDXData->GetType(&type)))
	{
		return;
	}

	if(FAILED(pDXData->GetName(NULL, &size)))
	{
		return;
	}

	if(size > 0)
	{
		if((pName = new char[size]) != NULL)
		{
			pDXData->GetName(pName, &size);
		}
	}

	if(NULL == pName)
	{
		if(NULL == (pName = new char[9]))
		{
			_WriteError(_T("内存申请失败"));
			return;
		}
		
		strcpy(pName, "$NoName$");
	}

	// 设置父指针

	pSubAnimSet = pParentSet;
	pSubAnim    = pCurAnim;


	if(TID_D3DRMAnimationSet == type)
	{
		// 建立一个动画集
		pAnimSet = new AnimationSet;
		if(NULL == pAnimSet)
		{
			return;
		}

		pAnimSet->_pName = pName;
		pName = NULL;

		// 将新来的放在表头

		pAnimSet->_pNext = m_pAnimSet;
		m_pAnimSet = pAnimSet;

		// 指定下一个循环的对象

		pSubAnimSet = pAnimSet;

		m_nAnimSet++;
	}

	if(TID_D3DRMAnimation == type && NULL != pParentSet)
	{
		// 建立一个动画结构

		pAnim = new Animation;
		if(NULL == pAnim)
		{
			return;
		}

		pAnim->_pName = pName;

		pName = NULL;

		// 将新动画加到当前指定的动画集中

		pAnim->_pNext = pParentSet->_pAnim;
		pParentSet->_pAnim = pAnim;

		// 更新父动画指针

		pSubAnim = pAnim;
	}

	if(TID_D3DRMAnimationKey == type && NULL != pCurAnim)
	{
		DWORD size = 0;
		PBYTE buffer = NULL;
        //LPCVOID buffer = NULL;

		if(FAILED(pDXData->Lock(&size, (LPCVOID*)&buffer)))
		{
			return;
		}

		DWORD keyType = ((DWORD*)buffer)[0];		// 键类型
		DWORD keyNum  = ((DWORD*)buffer)[1];		// 键数量

		switch(keyType)
		{
		case 0:		// 0代表旋转键
			{
				if(NULL != pCurAnim->_pRotateKeys)
				{
					delete[] pCurAnim->_pRotateKeys;
				}

				pCurAnim->_pRotateKeys   = new RotateKey[keyNum];
				pCurAnim->_rotateKeysNum = keyNum;

				XFileRotateKey *pRotKey = (XFileRotateKey*)((char*)buffer + sizeof(DWORD)*2);

				for(DWORD i=0; i<keyNum; i++)
				{
					pCurAnim->_pRotateKeys[i]._time = pRotKey->_time;
					pCurAnim->_pRotateKeys[i]._quaternion.x = -pRotKey->_x;	// 为啥是负的？
					pCurAnim->_pRotateKeys[i]._quaternion.y = -pRotKey->_y;
					pCurAnim->_pRotateKeys[i]._quaternion.z = -pRotKey->_z;
					pCurAnim->_pRotateKeys[i]._quaternion.w = -pRotKey->_w;

					if(pRotKey->_time > pParentSet->_length)
					{
						pParentSet->_length = pRotKey->_time;
					}

					pRotKey += 1;
				}
			}
			break;

		case 1:		// 1代表缩放键
			{
				if(NULL != pCurAnim->_pScaleKeys)
				{
					delete[] pCurAnim->_pScaleKeys;
				}

				pCurAnim->_pScaleKeys   = new ScaleKey[keyNum];
				pCurAnim->_scaleKeysNum = keyNum;

				XFileScaleKey *pScaleKey = (XFileScaleKey*)((char*)buffer + sizeof(DWORD)*2);

				for(DWORD i=0; i<keyNum; i++)
				{
					pCurAnim->_pScaleKeys[i]._time  = pScaleKey->_time;
					pCurAnim->_pScaleKeys[i]._scale = pScaleKey->_scale;

					if(pScaleKey->_time > pParentSet->_length)
					{
						pParentSet->_length = pScaleKey->_time;
					}

					pScaleKey += 1;
				}

				// 计算插值
				if(keyNum > 1)
				{
					for(DWORD i=0; i<keyNum-1; i++)
					{
						pCurAnim->_pScaleKeys[i]._scaleInterpolation = pCurAnim->_pScaleKeys[i+1]._scale - pCurAnim->_pScaleKeys[i]._scale;

						DWORD time = pCurAnim->_pScaleKeys[i+1]._time - pCurAnim->_pScaleKeys[i]._time;

						time = (time < 1) ? 1 : time;

						pCurAnim->_pScaleKeys[i]._scaleInterpolation /= (float)time;
					}
				}
			}
			break;

		case 2:		// 2代表平移键
			{
				if(NULL != pCurAnim->_pPositionKeys)
				{
					delete[] pCurAnim->_pPositionKeys;
				}

				pCurAnim->_pPositionKeys   = new PositionKey[keyNum];
				pCurAnim->_positionKeysNum = keyNum;

				XFilePositionKey *pPositionKey = (XFilePositionKey*)((char*)buffer + sizeof(DWORD)*2);

				for(DWORD i=0; i<keyNum; i++)
				{
					pCurAnim->_pPositionKeys[i]._time = pPositionKey->_time;
					pCurAnim->_pPositionKeys[i]._pos  = pPositionKey->_pos;

					if(pPositionKey->_time > pParentSet->_length)
					{
						pParentSet->_length = pPositionKey->_time;
					}

					pPositionKey += 1;
				}

				// 计算插值
				if(keyNum > 1)
				{
					for(DWORD i=0; i<keyNum-1; i++)
					{
						pCurAnim->_pPositionKeys[i]._posInterpolation = pCurAnim->_pPositionKeys[i+1]._pos - pCurAnim->_pPositionKeys[i]._pos;

						DWORD time = pCurAnim->_pPositionKeys[i+1]._time - pCurAnim->_pPositionKeys[i]._time;

						time = (time < 1) ? 1 : time;

						pCurAnim->_pPositionKeys[i]._posInterpolation /= (float)time;
					}
				}
			}
			break;

		case 4:		// 4代表变换矩阵键
			{
				if(NULL != pCurAnim->_pMatrixKeys)
				{
					delete[] pCurAnim->_pMatrixKeys;
				}

				pCurAnim->_pMatrixKeys  = new MatrixKey[keyNum];
				pCurAnim->_matrixKeyNum = keyNum;

				XFileMatrixKey *pMatrixKey = (XFileMatrixKey*)((char*)buffer + sizeof(DWORD)*2);

				for(DWORD i=0; i<keyNum; i++)
				{
					pCurAnim->_pMatrixKeys[i]._time   = pMatrixKey->_time;
					pCurAnim->_pMatrixKeys[i]._matrix = pMatrixKey->_matrix;

					if(pMatrixKey->_time > pParentSet->_length)
					{
						pParentSet->_length = pMatrixKey->_time;
					}

					pMatrixKey += 1;
				}

				// 计算插值
				if(keyNum > 1)
				{
					for(DWORD i=0; i<keyNum-1; i++)
					{
						pCurAnim->_pMatrixKeys[i]._matInterpolation = pCurAnim->_pMatrixKeys[i+1]._matrix - pCurAnim->_pMatrixKeys[i]._matrix;

						DWORD time = pCurAnim->_pMatrixKeys[i+1]._time - pCurAnim->_pMatrixKeys[i]._time;

						time = (time < 1) ? 1 : time;

						pCurAnim->_pMatrixKeys[i]._matInterpolation /= (float)time;
					}
				}
			}
			break;

		default:
			break;
		}

		pDXData->Unlock();
	}

	if(TID_D3DRMAnimationOptions == type && NULL != pCurAnim)
	{
		// 加载一些其他选项

		DWORD size = 0;
        LPCVOID buffer = NULL;

		if(FAILED(pDXData->Lock(&size, &buffer)))
		{
			return;
		}

		if(0 != ((DWORD*)buffer)[0])
		{
			pCurAnim->_bLoop = TRUE;
		}
		else
		{
			pCurAnim->_bLoop = FALSE;
		}

		if(0 != ((DWORD*)buffer)[1])
		{
			pCurAnim->_bLinear = FALSE;
		}
		else
		{
			pCurAnim->_bLinear = TRUE;
		}
	}

	if(TID_D3DRMFrame == type && NULL != pCurAnim)
	{
		pCurAnim->_pFrameName = pName;
		pName = NULL;

		// 没有动画了，不用继续
		return;
	}

	if(NULL != pName)
	{
		// 可能此部分为mesh的部分，直接回收

		delete [] pName;
	}

	SIZE_T        childNum  = 0;
	ID3DXFileData *pSubData = NULL;

    pDXData->GetChildren(&childNum);

    for(SIZE_T i=0; i<childNum; i++)
    {
        pDXData->GetChild(i, &pSubData);

        _ParseXFileData(pSubData, pSubAnimSet, pSubAnim);

        ReleaseCOM(pSubData);
    }
}

AnimationSet *CAnimation::_FindAnimSet(AnimationSet *pAnimSet, const char *pAnimName)
{

	// 找到动画集链表中的某个集，用名字来识别

	while(NULL != pAnimSet)
	{
		if(NULL != pAnimSet->_pName && 0 == strcmp(pAnimSet->_pName, pAnimName))
		{
			return pAnimSet;
		}
		pAnimSet = pAnimSet->_pNext;
	}

	return NULL;
}

void CAnimation::_UpdateAnimationSet(AnimationSet *pAnimSet, DWORD time, BOOL bSmooth)
{
	// 更新动画集

	if(NULL == pAnimSet)
	{
		return;
	}

	DWORD      length = pAnimSet->_length;
	Animation *pAnim  = pAnimSet->_pAnim;

	while(NULL != pAnim)
	{	
		if(0 == length)
		{
			// length = 0 只绘制第一帧

			_UpdateAnimation(pAnim, 0, FALSE);
		}
		else if(time <= length && !pAnim->_bLoop)
		{
			// 如果提取的位置小于动画的长度，并且没有设置循环，则提取该位置的帧的

			_UpdateAnimation(pAnim, time, FALSE);
		}
		else
		{
			// 如果设置了循环或者长度大于动画的长度，则使用插值

			_UpdateAnimation(pAnim, time % length, bSmooth);
		}
		

		pAnim = pAnim->_pNext;
	}
}

void CAnimation::_UpdateAnimation(Animation *pAnim, DWORD time, BOOL  bSmooth)
{
	// 更新一个动画的帧
	// 动画信息中，可以提供两种方式的信息
	// 一种是直接以矩阵的形式，此时可以直接使用
	// 另一种是以旋转+平移+缩放的组合给出的，
	// 此时应该先计算各自的插值，然后利用矩阵相乘，得到最终的变换矩阵

	Frame *pFrame = pAnim->_pFrame;

	if(NULL == pFrame)
	{	
		return;
	}

	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);

	D3DXMATRIX matTemp;			// 临时借用
	D3DXMatrixIdentity(&matTemp);

	DWORD key      = 0;
	DWORD timeDiff = 0;			// 时间差
	DWORD timeInt  = 0;			// 插值时间

	if(pAnim->_rotateKeysNum > 0 || pAnim->_scaleKeysNum > 0 || pAnim->_positionKeysNum > 0)
	{
		D3DXVECTOR3    vect;	// 向量
		D3DXQUATERNION quat;	// 四元数

		if(NULL != pAnim->_pRotateKeys && pAnim->_rotateKeysNum>0)
		{
			key = 0;

			// 找到在时间范围内的一个键
			for(DWORD i=0; i<pAnim->_rotateKeysNum; i++)
			{
				if(pAnim->_pRotateKeys[i]._time < time)
				{
					key = i;
				}
				else
				{
					break;
				}
			}

			if((pAnim->_rotateKeysNum-1) == key || !bSmooth)
			{
				quat = pAnim->_pRotateKeys[key]._quaternion;
			}
			else
			{
				timeDiff = pAnim->_pRotateKeys[key+1]._time - pAnim->_pRotateKeys[key]._time;
				timeInt  = time - pAnim->_pRotateKeys[key]._time;

				D3DXQuaternionSlerp(
					&quat, 
					&pAnim->_pRotateKeys[key]._quaternion, 
					&pAnim->_pRotateKeys[key]._quaternion, 
					((float)timeInt / (float)timeDiff));
			}

			D3DXMatrixRotationQuaternion(&matTemp, &quat);
			matrix *= matTemp;
		}
		if(NULL != pAnim->_pScaleKeys && pAnim->_scaleKeysNum>0)
		{
			key = 0;

			for(DWORD i=0; i<pAnim->_scaleKeysNum; i++)
			{
				if(pAnim->_pScaleKeys[i]._time < time)
				{
					key = i;
				}
				else
				{
					break;
				}
			}

			if((pAnim->_scaleKeysNum-1) == key || !bSmooth)
			{
				vect = pAnim->_pScaleKeys[key]._scale;
			}	
			else
			{
				timeInt = time - pAnim->_pScaleKeys[key]._time;
				vect    = pAnim->_pScaleKeys[key]._scale + pAnim->_pScaleKeys[key]._scaleInterpolation * (float)timeInt;

			}

			D3DXMatrixScaling(&matTemp, vect.x, vect.y, vect.z);
			matrix *= matTemp;
		}
		if(NULL != pAnim->_pPositionKeys && pAnim->_positionKeysNum>0)
		{
			key = 0;

			for(DWORD i=0; i<pAnim->_positionKeysNum; i++)
			{
				if(pAnim->_pPositionKeys[i]._time < time)
				{
					key = i;
				}
				else
				{
					break;
				}
			}

			if((pAnim->_positionKeysNum-1) == key || !bSmooth)
			{
				vect == pAnim->_pPositionKeys[key]._pos;
			}
			else
			{
				timeInt = time - pAnim->_pPositionKeys[key]._time;

				vect = pAnim->_pPositionKeys[key]._pos + pAnim->_pPositionKeys[key]._posInterpolation*(float)timeInt;
			}

			D3DXMatrixTranslation(&matTemp, vect.x, vect.y, vect.z);
			matrix *= matTemp;
		}
		pFrame->_matTransformed = matrix;
	}
	if(NULL != pAnim->_pMatrixKeys && pAnim->_matrixKeyNum > 0)
	{
		key = 0;

		for(DWORD i=0; i<pAnim->_matrixKeyNum; i++)
		{
			if(pAnim->_pMatrixKeys[i]._time <= time)
			{
				key = i;
			}
			else
			{
				break;
			}
		}

		if((pAnim->_matrixKeyNum-1) == key || !bSmooth)
		{
			pFrame->_matTransformed = pAnim->_pMatrixKeys[key]._matrix;
		}
		else
		{
			timeInt = time - pAnim->_pMatrixKeys[key]._time;
			matrix  = pAnim->_pMatrixKeys[key]._matInterpolation*(float)timeInt;
			pFrame->_matTransformed = matrix + pAnim->_pMatrixKeys[key]._matrix;
		}
	}
}

BOOL CAnimation::_MapToMesh(CMesh *pMesh)
{
	// 将骨骼帧与网格类的帧关联在一起
	// 这样可以做到，当动画更新时，网格可以立即得到更新的信息

	if(NULL == pMesh)
	{
		_WriteError(_T("MapToMesh mesh为空"));
		return FALSE;
	}

	if(NULL == m_pAnimSet)
	{
		_WriteError(_T("MapToMesh animation为空"));
		return FALSE;
	}

	AnimationSet *pAnimSet = m_pAnimSet;
	Animation    *pAnim    = NULL;

	while(NULL != pAnimSet)
	{
		pAnim = pAnimSet->_pAnim;

		while(NULL != pAnim)
		{
			pAnim->_pFrame = pMesh->GetFrame(pAnim->_pFrameName);
			pAnim = pAnim->_pNext;
		}
		pAnimSet = pAnimSet->_pNext;
	}

	return TRUE;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CObject
//
////////////////////////////////////////////////////////////////////////////////////////////////////

CObject::CObject()
{
	m_pAnim   = NULL;
	m_pMesh   = NULL;
	m_bLoaded = FALSE;
}
CObject::~CObject()
{
	Destroy();	
}

void CObject::Destroy()
{

	if(m_bLoaded)
	{
		if(NULL != m_pAnim)
		{
			delete m_pAnim;
			
		}
		if(NULL != m_pMesh)
		{
			delete m_pMesh;
			
		}
		m_bLoaded = FALSE;
	}

	m_pAnim = NULL;
	m_pMesh = NULL;
}

void CObject::MoveTo(float xPos, float yPos, float zPos)
{
	m_pos.MoveTo(xPos, yPos, zPos);
}

void CObject::Move(float x, float y, float z)
{
	m_pos.Move(x, y, z);
}

void CObject::RotateTo(float xRot, float yRot, float zRot)
{
	m_pos.RotateTo(xRot, yRot, zRot);
}
void CObject::Rotate(float x, float y, float z)
{
	m_pos.Rotate(x, y, z);
}

void CObject::ScaleTo(float xRot, float yRot, float zRot)
{
	m_pos.ScaleTo(xRot, yRot, zRot);
}

void CObject::Scale(float x, float y, float z)
{
	m_pos.Scale(x, y, z);
}

BOOL CObject::Create(CMesh *pMesh, CAnimation *pAnim)
{
	Destroy();

	m_pMesh   = pMesh;
	m_pAnim   = pAnim;
	m_bLoaded = FALSE;

	MoveTo(0, 0, 0);
	RotateTo(0.0f, 0.0f, 0.0f);
    ScaleTo(1.0f, 1.0f, 1.0f);

	return TRUE;
}

BOOL CObject::LoadXFile(CGraphics *pGraphics, const char *pFileName, const char *pTexturePath)
{
	Destroy();

	m_pMesh = new CMesh;
	
	if(!m_pMesh->LoadXFile(pGraphics, pFileName, pTexturePath))
	{
		if(NULL != m_pMesh)
		{
			delete m_pMesh;
			m_pMesh = NULL;
		}

		_WriteError(_T(".X文件网格加载失败"));

		// 不能容忍，返回
		return FALSE;
	}

	m_pAnim = new CAnimation;

	if(!m_pAnim->LoadXFile(pFileName, m_pMesh))
	{
		if(NULL != m_pAnim)
		{
			delete m_pAnim;
			m_pAnim = NULL;
		}
	}

	m_bLoaded = TRUE;

	MoveTo(0, 0, 0);
	RotateTo(0.0f, 0.0f, 0.0f);
    ScaleTo(1.0f, 1.0f, 1.0f);
	
	return TRUE;
}

BOOL CObject::SetAnimation(const char *pAnimSetName, DWORD startTime)
{
	if(NULL == m_pAnim)
	{
		_WriteError(_T("SetAnimation 没有加载动画"));
		return FALSE;
	}

	if(!m_pAnim->SetCurAnimSet(pAnimSetName, startTime))
	{
		_WriteError(_T("选择动画集失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CObject::SetLoop(const char *pAnimSetName, BOOL bLoop)
{
	if(NULL == m_pAnim)
	{
		_WriteError(_T("SetLoop 没有加载动画"));
		return FALSE;
	}

	if(!m_pAnim->SetLoop(pAnimSetName, bLoop))
	{
		_WriteError(_T("设置动画循环失败"));
		return FALSE;
	}

	return TRUE;
}

BOOL CObject::Render(DWORD curTime, BOOL bSmooth)
{
	if(NULL == m_pMesh)
	{
		_WriteError(_T("网格没有加载"));
		return FALSE;
	}

	if(NULL != m_pAnim)
	{
		if(!m_pAnim->UpdateCurAnim(curTime, bSmooth))
		{
			_WriteError(_T("CObject::Render 更新动画集失败"));
		}

		if(!m_pMesh->Render(&m_pos, TRUE))
		{
			_WriteError(_T("CObject::Render 网格绘制失败"));
		}
	}
	else
	{
		if(!m_pMesh->Render(&m_pos, FALSE))
		{
			_WriteError(_T("CObject::Render 网格绘制失败"));
		}
	}

	return TRUE;
}




