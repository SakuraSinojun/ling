#include "d3d9.h"
#include <d3dx9.h>
#include <DXFile.h>
#include <rmxftmpl.h>		// 注册X文件模板宏
#include <RmxfGuid.h>		// GUID
#include "Graphics.h"

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

void CGraphics::_WriteError(const char *error)
{
	MessageBox(NULL, error, "错误", 0);
}

BOOL CGraphics::Init(HWND hWnd, BOOL bWindow, DWORD width, DWORD height, char bpp)
{
	if(NULL == hWnd)
	{
		return FALSE;
	}

	if((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		_WriteError("初始化D3D失败");
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
				_WriteError("没有支持的格式");
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
						_WriteError("没有支持的格式");
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
		_WriteError("创建设备失败");
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

	SetPerspective(D3DX_PI * 0.5f, (float)m_width/(float)m_height, 1.0f, 1000.0f);

	if(FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		_WriteError("CreateSprite eorror");
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::ClearZBuffer(float zBuffer)
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, zBuffer, 0)))
	{
		_WriteError("清除画面失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::ClearDisplay(DWORD color)
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->Clear(0, NULL,  D3DCLEAR_TARGET, 0, 1.0f, 0)))
	{
		_WriteError("清除画面失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::Clear(DWORD color, FLOAT zBuffer)
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(!m_bZBuffer)
	{
		return this->ClearDisplay(color);
	}

	if(FAILED(m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, zBuffer, 0)))
	{
		_WriteError("清除画面失败");
		return FALSE;
	}

	return TRUE;
}



BOOL CGraphics::BeginScene()
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->BeginScene()))
	{
		_WriteError("绘制缓冲画面失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::EndScene()
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->EndScene()))
	{
		_WriteError("关闭缓冲画面失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::Display()
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->Present(NULL, NULL, NULL, NULL)))
	{
		//_WriteError("显示主画面失败");
		return FALSE;
	}

	return TRUE;
}

char CGraphics::_GetFormatBPP(const D3DFORMAT &format)
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
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	D3DXMatrixPerspectiveFovLH(&matProjection, fov, aspect, nz, fz);

	if(FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &matProjection)))
	{
		_WriteError("变换失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::SetAmbientLight(BYTE red, BYTE green, BYTE blue)
{
	D3DCOLOR Color;

	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	Color = D3DCOLOR_XRGB((m_ambientRed = red), (m_ambientGreen = green), (m_ambientBlue = blue));

	if(FAILED(m_pDevice->SetRenderState(D3DRS_AMBIENT, Color)))
	{
		_WriteError("设置环境光失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::SetShadeMode(DWORD shadeMode)
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->SetRenderState(D3DRS_SHADEMODE, shadeMode)))
	{
		_WriteError("设置环境光失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::SetFillMode(DWORD fillMode)
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->SetRenderState(D3DRS_FILLMODE, fillMode)))
	{
		_WriteError("设置环境光失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::SetLignt(DWORD num, const CLight &light)
{
	
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->SetLight(num, light.GetLight())))
	{
		_WriteError("设置光照失败");
		return FALSE;
	}
	return TRUE;
}

BOOL CGraphics::SetWorldPos(const D3DXMATRIX &worldMatrix)
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->SetTransform(D3DTS_WORLD, &worldMatrix)))
	{
		_WriteError("转换世界坐标失败");
		return FALSE;
	}
	return TRUE;
}

BOOL CGraphics::SetWorldPos(CPosition &pos)
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->SetTransform(D3DTS_WORLD, pos.GetMatrix())))
	{
		_WriteError("转换世界坐标失败");
		return FALSE;
	}
	return TRUE;
}

BOOL CGraphics::SetCamera(CCamera &camera)
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->SetTransform(D3DTS_VIEW, camera.GetMatrix())))
	{
		_WriteError("相机坐标转换失败");
		return FALSE;
	}
	return TRUE;
}

BOOL CGraphics::EnableLight(DWORD num, BOOL bEnable)
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->LightEnable(num, bEnable)))
	{
		_WriteError("设置光源失败");
		return FALSE;
	}
	return TRUE;
}

BOOL CGraphics::EnableLighting(BOOL bEnable)
{
	if(m_pDevice == NULL)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, bEnable)))
	{
		_WriteError("设置光照失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CGraphics::EnableZBuffer(BOOL bEnable)
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->SetRenderState(D3DRS_ZENABLE, bEnable  ? D3DZB_TRUE : D3DZB_FALSE)))
	{
		_WriteError("无法使用Z缓存");
		return FALSE;
	}

	m_bZBuffer = bEnable;

	return TRUE;
}

BOOL CGraphics::EnableAlphaBlending(BOOL bEnable, DWORD srcValue, DWORD destValue)
{
	if(NULL == m_pDevice)
	{
		_WriteError("未初始化DX设备");
		return FALSE;
	}

	if(FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, bEnable)))
	{
		_WriteError("设置alpha混合失败");
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

void CVertex::_WriteError(const char *error)
{
	::MessageBox(0, error, "错误", 0);
}

BOOL CVertex::CreateBuffer(CGraphics *pGraphics, DWORD vertexNum, DWORD faceNum, DWORD vertexSize, DWORD FVF)
{
	if(NULL == (m_pGraphics = pGraphics))
	{
		_WriteError("图形指针为正确传入");
		return FALSE;
	}

	if(NULL == m_pGraphics->GetDevice())
	{
		_WriteError("设备未初始化");
		return FALSE;
	}

	if(0 == (m_vertexNum = vertexNum) || 
	   0 == (m_vertexSize = vertexSize) || 
	   0 == (m_FVF = FVF) ||
	   0 == (m_faceNum = faceNum))
	{
		_WriteError("参数不正确");
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
		_WriteError("图形指针为正确传入");
		return FALSE;
	}

	if(NULL == m_pGraphics->GetDevice())
	{
		_WriteError("设备未初始化");
		return FALSE;
	}

	if(0 == (m_indexNum = indexNum) || 
	   0 == (m_indexSize = indexSize))
	{
		_WriteError("参数不正确");
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
		_WriteError("顶点数组没有创建");
		return NULL;
	}

	if(FAILED(m_pVB->Lock(
		startVertex * m_vertexNum, 
		vertexNum * m_vertexSize,
		&m_pBuffer,
		D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD)))
	{
		_WriteError("顶点锁定失败");
		return NULL;
	}

	return m_pBuffer;
}

void *CVertex::LockIndex(DWORD startIndex, DWORD indexNum)
{
	if(NULL == m_pIB)
	{
		_WriteError("顶点数组没有创建");
		return NULL;
	}

	if(FAILED(m_pIB->Lock(
		startIndex * m_indexNum, 
		indexNum * m_indexSize,
		&m_pIndex,
		D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD)))
	{
		_WriteError("顶点锁定失败");
		return NULL;
	}

	return m_pIndex;
}

BOOL CVertex::UnlockBuffer()
{
	if(NULL == m_pVB)
	{
		//_WriteError("顶点数组没有创建");
		return FALSE;
	}

	if(FAILED(m_pVB->Unlock()))
	{
		_WriteError("顶点解除锁定失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CVertex::UnlockIndex()
{
	if(NULL == m_pIB)
	{
		//_WriteError("顶点数组没有创建");
		return FALSE;
	}

	if(FAILED(m_pIB->Unlock()))
	{
		_WriteError("顶点解除锁定失败");
		return FALSE;
	}

	return TRUE;
}

BOOL CVertex::SetBuffer(DWORD startVertex, DWORD vertexNum, void* vertexList)
{
	if(NULL == m_pGraphics || NULL == m_pVB)
	{
		_WriteError("顶点缓存没有被正确创建");
		return FALSE;
	}

	if(NULL == m_pGraphics)
	{
		_WriteError("设备未初始化");
		return FALSE;
	}

	if(0 == vertexNum || NULL == vertexList)
	{
		_WriteError("参数不正确");
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
		_WriteError("顶点缓存没有被正确创建");
		return FALSE;
	}

	if(NULL == m_pGraphics)
	{
		_WriteError("设备未初始化");
		return FALSE;
	}

	if(0 == indexNum || NULL == indexList)
	{
		_WriteError("参数不正确");
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
		_WriteError("顶点缓存没有被正确创建");
		return FALSE;
	}

	if(NULL == m_pGraphics->GetDevice())
	{
		_WriteError("设备未初始化");
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
			_WriteError("纹理未初始化");
			return FALSE;
		}
		if(FAILED(m_pGraphics->GetDevice()->SetTexture(0, pTexture->GetTexture())))
		{
			_WriteError("纹理绘制失败");
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
			_WriteError("纹理清空失败");
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

void CTexture::_WriteError(const char *error)
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

BOOL CTexture::LoadFromFile(CGraphics *pGraphics, const char *fileName, DWORD colorKey, D3DFORMAT format)
{
	if(NULL == (m_pGraphics = pGraphics) || NULL == fileName)
	{
		_WriteError("参数指针为空");
		return FALSE;
	}

	if(NULL == m_pGraphics->GetDevice())
	{
		_WriteError("图形设备未初始化");
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
		_WriteError("加载纹理失败");
		return FALSE;
	}

	if(FAILED(m_pTexture->GetLevelDesc(0, &m_d3dsd)))
	{
		_WriteError("获取纹理信息失败");
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
		_WriteError("纹理没有被初始化");
		return FALSE;
	}

	if(NULL == m_pGraphics)
	{
		_WriteError("图形设备初始化");
		return FALSE;
	}

	ID3DXSprite *pSprite = m_pGraphics->GetSprite();

	if(NULL == pSprite)
	{
		_WriteError("DX精灵没有被初始化");
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
		_WriteError("绘制精灵画面失败");
		return FALSE;
	}

	pSprite->End();

	return TRUE;
}

BOOL  CTexture::Rotate(float centerX, float centerY, FLOAT angle)
{
	if(NULL == m_pTexture)
	{
		_WriteError("纹理没有被初始化");
		return FALSE;
	}

	if(NULL == m_pGraphics)
	{
		_WriteError("图形设备初始化");
		return FALSE;
	}

	ID3DXSprite *pSprite = m_pGraphics->GetSprite();

	if(NULL == pSprite)
	{
		_WriteError("DX精灵没有被初始化");
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
		_WriteError("纹理没有被初始化");
		return FALSE;
	}

	if(NULL == m_pGraphics)
	{
		_WriteError("图形设备初始化");
		return FALSE;
	}

	ID3DXSprite *pSprite = m_pGraphics->GetSprite();

	if(NULL == pSprite)
	{
		_WriteError("DX精灵没有被初始化");
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
		_WriteError("纹理没有被初始化");
		return FALSE;
	}

	if(NULL == m_pGraphics)
	{
		_WriteError("图形设备初始化");
		return FALSE;
	}

	ID3DXSprite *pSprite = m_pGraphics->GetSprite();

	if(NULL == pSprite)
	{
		_WriteError("DX精灵没有被初始化");
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

void CFont::_WriteError(const char *error)
{
	::MessageBox(0, error, "错误", 0);
}

BOOL CFont::Create(CGraphics *pGraphics, INT height, const char *fontName, BOOL bItalic)
{
	if(NULL == pGraphics)
	{
		_WriteError("图形设备指针为空");
		return FALSE;
	}

	D3DXFONT_DESC lf;
	ZeroMemory(&lf, sizeof(lf));

	if(NULL == fontName)
	{
		strcpy(lf.FaceName, "Times New Roman");
	}
	else
	{
		strcpy(lf.FaceName, fontName);
	}

	lf.Height = height;

	lf.Italic = bItalic;

	if(FAILED(D3DXCreateFontIndirect(pGraphics->GetDevice(), &lf, &m_pFont)))
	{
		_WriteError("D3DXCreateFontIndirect failed");
		return FALSE;
	}

	return TRUE;
}

BOOL CFont::Print(const char *text, long xPos, long yPos, long width, long height, DWORD format, D3DCOLOR color)
{
	if(NULL == m_pFont)
	{
		_WriteError("字体没有被创建");
		return FALSE;
	}

	RECT rc = {xPos, yPos, xPos + width, yPos + height};

	if(FAILED(m_pFont->DrawText(NULL, text, -1, &rc, format, color)))
	{
		_WriteError("绘制文字失败");
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

	D3DXMatrixTranslation(&m_matTranslation, -m_xPos, -m_yPos, -m_zPos);
}

void CPosition::Move(float xPosAdd, float yPosAdd, float zPosAdd)
{
	this->MoveTo(m_xPos + xPosAdd, m_yPos + yPosAdd, m_zPos + zPosAdd);
}

void CPosition::RotateTo(float xRot, float yRot, float zRot)
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
	m_matWorld = m_matScale * m_matTranslation * m_matRotation;
}

const D3DXMATRIX *CPosition::GetMatrix()
{
	Update();
	return &m_matWorld;
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

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CFrame
//
////////////////////////////////////////////////////////////////////////////////////////////////

CFrame::CFrame()
{
	m_pFrame = NULL;
}

CFrame::~CFrame()
{
	delete m_pFrame;
}

void CFrame::_WriteError(const char *error)
{
	::MessageBox(0, error, "错误", 0);
}
BOOL CFrame::LoadXFile(CGraphics *pGraphics, const char *fileName)
{
	if(NULL == pGraphics || NULL  == pGraphics->GetDevice() || NULL == fileName)
	{
		_WriteError("参数传入错误或设备未初始化");
		return FALSE;
	}

	m_pGraphics = pGraphics;
	return ParseXFile(fileName);
}

BOOL CFrame::ParseXFile(const char *fileName)
{
	ID3DXFile           *pDXFile = NULL;
	ID3DXFileEnumObject *pDXEnum = NULL;
	ID3DXFileData       *pDXData = NULL;

	Frame *pFrame = NULL;

	// 创建一个DX文件
	if(FAILED(D3DXFileCreate(&pDXFile)))
	{
		_WriteError("创建DX文件失败");
		return FALSE;
	}

	// 注册.X文件模板
	if(FAILED(pDXFile->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES)))
	{
		_WriteError("注册.X文件模板失败");
		ReleaseCOM(pDXFile);
		return FALSE;
	}

	// 建立一个枚举对象 浏览.x文件中的顶级对象
	if(FAILED(pDXFile->CreateEnumObject((LPVOID)fileName, DXFILELOAD_FROMFILE, &pDXEnum)))
	{
		_WriteError("加载.X文件失败");
		ReleaseCOM(pDXFile);
		return FALSE;
	}

	pFrame = new Frame;

	// 9.0c 与 9.0b 不同

	SIZE_T childNum;

	if(FAILED(pDXEnum->GetChildren(&childNum)))
	{
		_WriteError("pDXEnum::GetChildren failed");
		ReleaseCOM(pDXFile);
		ReleaseCOM(pDXEnum);
		return FALSE;
	}

	for(SIZE_T i = 0; i < childNum; i++)
	{
		if(FAILED(pDXEnum->GetChild(i, &pDXData)))
		{
			_WriteError("pDXEnum::GetChild failed");
			ReleaseCOM(pDXFile);
			ReleaseCOM(pDXEnum);
			return FALSE;
		}

		ParseXFileData(pDXData, pFrame);
	}

	ReleaseCOM(pDXEnum);
	ReleaseCOM(pDXFile);

	m_pFrame = pFrame;

	return TRUE;
}

void CFrame::ParseXFileData(ID3DXFileData *pDXData, Frame *pParentFrame)
{
	ID3DXFileData             *pSubData        = NULL;
	
	ID3DXBuffer               *pMaterialBuffer = NULL;
	D3DXMATERIAL              *pMaterials      = NULL;
	ID3DXBuffer               *pAdjacency      = NULL;

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
			_WriteError("内存申请失败");
			return;
		}
		
		strcpy(pName, "Template");
	}

	pSubFrame = pParentFrame;

	if(type == TID_D3DRMFrame)
	{
		pFrame = new Frame;

		pFrame->_pName = pName;
		pName = NULL;

		pFrame->_pSibling = pParentFrame->_pChild;
		pParentFrame->_pChild = pFrame;

		pSubFrame = pFrame;
	}

	if(type == TID_D3DRMMesh)
	{
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
			_WriteError("D3DXLoadSkinMeshFromXof failed");

			delete[] pName;
			delete pMesh;

			return;
		}

		ReleaseCOM(pAdjacency);

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
		}

		if(0 == pMesh->_materialNum)
		{
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
			pMaterials = (D3DXMATERIAL*) pMaterialBuffer->GetBufferPointer();

			pMesh->_pMaterials = new D3DMATERIAL9[pMesh->_materialNum];
			pMesh->_apTextures = new LPDIRECT3DTEXTURE9[pMesh->_materialNum];

			for(DWORD i=0; i<pMesh->_materialNum; i++)
			{
				pMesh->_pMaterials[i] = pMaterials[i].MatD3D;
				pMesh->_pMaterials[i].Ambient = pMesh->_pMaterials[i].Diffuse;

				if(FAILED(D3DXCreateTextureFromFile(
					m_pGraphics->GetDevice(), 
					pMaterials[i].pTextureFilename, 
					&pMesh->_apTextures[i])))
				{
					pMesh->_apTextures[i] = NULL;
				}
			}
		}

		ReleaseCOM(pMaterialBuffer);

		pParentFrame->_pMesh = pMesh;

	} // TID_D3DRMMesh
	
	if(type == TID_D3DRMAnimationSet || type == TID_D3DRMAnimation || type == TID_D3DRMAnimationKey)
	{
		// 跳过动画

		delete pName;

		return;
	}

	SIZE_T childNum;

    pDXData->GetChildren(&childNum);

    // scan for embedded templates
    for(SIZE_T i=0; i<childNum; i++)
    {
        pDXData->GetChild(i, &pSubData);

        // process embedded pDXFile data, recursive call.
        ParseXFileData(pSubData, pSubFrame);

        ReleaseCOM(pSubData);
    }
}

BOOL CFrame::Render()
{
	return _DrawFrame(m_pFrame);
}

BOOL CFrame::_DrawFrame(Frame *pFrame)
{
	Mesh *pMesh;
    D3DXMATRIX *pMatrices = NULL;
    ID3DXMesh *pMeshToDraw;

    // return if no Frame
    if(pFrame == NULL)
        return TRUE;

    // draw meshes if any in Frame
    if((pMesh = pFrame->_pMesh) != NULL)
    {
        // setup pointer to Mesh to draw
        pMeshToDraw = pMesh->_pDXMesh;

        // generate Mesh from skinned Mesh to draw with
        if(pMesh->_pDXSkinMesh != NULL && pMesh->_pSkinInfo != NULL)
        {
            DWORD num_bones = pMesh->_pSkinInfo->GetNumBones();

            // allocate an array of Matrices to orient bones
            pMatrices = new D3DXMATRIX[num_bones];

            // set all bones orientation to identity
            for(DWORD i = 0; i < num_bones; i++)
                D3DXMatrixIdentity(&pMatrices[i]);

            // lock source and destination vertex buffers

            void* source = NULL;
            void* dest = NULL;

            // locks a vertex buffer and obtains a pointer to the vertex buffer memory
            pMesh->_pDXMesh->LockVertexBuffer(0, &source);
            pMesh->_pDXSkinMesh->LockVertexBuffer(0, &dest);

            // update skinned Mesh, applies software skinning to the target vertices based on the current Matrices.
            pMesh->_pSkinInfo->UpdateSkinnedMesh(pMatrices, NULL, source, dest);

            // unlock buffers
            pMesh->_pDXSkinMesh->UnlockVertexBuffer();
            pMesh->_pDXMesh->UnlockVertexBuffer();

            // point to skin Mesh to draw
            pMeshToDraw = pMesh->_pDXSkinMesh;
        }

        // render the Mesh
        for(DWORD i = 0; i < pMesh->_materialNum; i++)
        {
            // set the Materials properties for the device
            m_pGraphics->GetDevice()->SetMaterial(&pMesh->_pMaterials[i]);

            // assigns a texture to a stage for a device
            m_pGraphics->GetDevice()->SetTexture(0, pMesh->_apTextures[i]);

            // draw a subset of a Mesh
            pMeshToDraw->DrawSubset(i);
        }

        // free array of Matrices
        delete[] pMatrices;
        pMatrices = NULL;
    }

    // draw child frames, recursively call.
    return _DrawFrame(pFrame->_pChild);
}