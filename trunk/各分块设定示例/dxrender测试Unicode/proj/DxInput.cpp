 #include <tchar.h>

#include "DxInput.h"

CDXInput::CDXInput(void)
{
	m_lpdi		 = NULL;
	m_lpdiKey	 = NULL;
	m_lpdiMouse	 = NULL;
	m_bLBtn		 = false;
	m_bRBtn		 = false;

}

//-------------------------------------------/

CDXInput::~CDXInput(void)
{
	ShutDown();
}

void CDXInput::_WriteError(LPCTSTR error)
{
	::MessageBox(0, error, _T("´íÎó"), 0);
}

//---------------------------------------------------------------------------------/

bool CDXInput::Init(HWND hWnd, HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;
	if (DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION,IID_IDirectInput8, 
		(void **)&m_lpdi,NULL)!=DI_OK)
	{
		_WriteError(_T("DirectInput8Create fail"));
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------/

void CDXInput::ShutDown()
{
	ReleaseKeyBoard();
	ReleaseMouse();
	if(m_lpdi)
	{
		m_lpdi->Release();
		m_lpdi = NULL;
	}
}

//------------------------------------------------------------------------------/

bool CDXInput::InitKeyBoard()
{
	if (m_lpdi->CreateDevice(GUID_SysKeyboard, &m_lpdiKey, NULL) != DI_OK)
	{
		_WriteError(_T("CreateDevice fail"));
		return false;
	}

	if (m_lpdiKey->SetCooperativeLevel(m_hWnd,DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK)
	{
		_WriteError(_T("SetCooperativeLevel fail"));
		return false;
	}

	if (m_lpdiKey->SetDataFormat (&c_dfDIKeyboard) != DI_OK)
	{
		_WriteError(_T("SetDataFormat fail"));
		return false;
	}

	if (m_lpdiKey->Acquire() != DI_OK)
	{
		_WriteError(_T("AcquireKey fail"));
		return false;
	}

	memset(m_keyState, 0, sizeof(m_keyState));

	return true;
}

//----------------------------------------------------------------------------/

void CDXInput::ReleaseKeyBoard()
{
	if (m_lpdiKey)
	{
		m_lpdiKey->Unacquire();
		m_lpdiKey->Release();
		m_lpdiKey = NULL;
	}
}

//------------------------------------------------------------------------------------------/

bool CDXInput::InitMouse(int xMouse, int yMouse)
{
	if (m_lpdi->CreateDevice(GUID_SysMouse, &m_lpdiMouse, NULL) != DI_OK)
	{
		_WriteError(_T("CreateDevice fail"));
		return false;
	}

	if (m_lpdiMouse->SetCooperativeLevel(m_hWnd,DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK)
	{
		_WriteError(_T("SetCooperativeLevel fail"));
		return false;
	}

	if (m_lpdiMouse->SetDataFormat (&c_dfDIMouse) != DI_OK)
	{
		_WriteError(_T("SetDataFormat fail"));
		return false;
	}

	if (m_lpdiMouse->Acquire() != DI_OK)
	{
		_WriteError(_T("AcquireKey fail"));
		return false;
	}

	m_xMouse = xMouse;
	m_yMouse = yMouse;

	memset(&m_mouseState, 0, sizeof(m_mouseState));

	return true;
}

//--------------------------------------------------------------------------------------/

void CDXInput::ReleaseMouse()
{
	if (m_lpdiMouse)
	{
		m_lpdiMouse->Unacquire();
		m_lpdiMouse->Release();
		m_lpdiMouse = NULL;
	}
}

//-------------------------------------------------------------------------------------------/

bool CDXInput::GetKeyState()
{
	if(m_lpdiKey)
	{
		if ((m_lpdiKey->GetDeviceState(256, (LPVOID)m_keyState)) != DI_OK)
		{
			return false;
		}
	}
	else
	{
		memset(m_keyState, 0, sizeof(m_keyState));
	}
	return true;
}

//---------------------------------------------------------------------------------------------/

int CDXInput::KeyDown(BYTE Key)
{
	return (m_keyState[Key] & 0x80);
}

//---------------------------------------------------------------------------------------------/

bool CDXInput::GetMouseState()
{
	if (m_lpdiMouse)
	{
		if ((m_lpdiMouse->GetDeviceState(sizeof(m_mouseState), (LPVOID)&m_mouseState)) != DI_OK)
		{
			return false;
		}

		m_lx = m_mouseState.lX;
		m_ly = m_mouseState.lY;

		m_xMouse += m_mouseState.lX;
		m_yMouse += m_mouseState.lY;

		if (m_mouseState.rgbButtons[0])
		{
			m_bLBtn = true;
		}
		else
		{
			m_bLBtn = false;
		}

		if (m_mouseState.rgbButtons[0])
		{
			m_bRBtn = true;
		}
		else
		{
			m_bRBtn = false;
		}
	}
	else
	{
		memset(&m_mouseState, 0, sizeof(m_mouseState));
	}


	return true;
}

//----------------------------------------------------------------/

long CDXInput::GetMouseX()
{
	return m_xMouse;
}

//----------------------------------------------------/

long CDXInput::GetMouseY()
{
	return m_yMouse;
}

long CDXInput::GetMouseDeltaX()
{
	return m_lx;
}

long CDXInput::GetMouseDeltaY()
{
	return m_ly;
}

//------------------------------------------/

bool CDXInput::LBtnDown()
{
	return m_bLBtn;
}

//--------------------------------------------

bool CDXInput::RBtnDown()
{
	return m_bRBtn;
}