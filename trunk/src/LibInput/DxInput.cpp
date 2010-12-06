#include "../common/common.h"
#include "DxInput.h"
#include <stdio.h>

DxInput::DxInput(void)
{
	m_lpdi		 = NULL;
	m_lpdiKey	 = NULL;
	m_lpdiMouse	 = NULL;
	m_bLBtn		 = false;
	m_bRBtn		 = false;
}

//-------------------------------------------/

DxInput::~DxInput(void)
{
}

//---------------------------------------------------------------------------------/

bool DxInput::Init(HWND hWnd, HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;
	if (DirectInput8Create(m_hInstance, DIRECTINPUT_VERSION,IID_IDirectInput8, 
		(void **)&m_lpdi,NULL)!=DI_OK)
	{
		WriteError("DirectInput8Create fail");
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------/

void DxInput::ShutDown()
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

bool DxInput::InitKeyBoard()
{
	if (m_lpdi->CreateDevice(GUID_SysKeyboard, &m_lpdiKey, NULL) != DI_OK)
	{
		WriteError("CreateDevice fail");
		return false;
	}

	if (m_lpdiKey->SetCooperativeLevel(m_hWnd,DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK)
	{
		WriteError("SetCooperativeLevel fail");
		return false;
	}

	if (m_lpdiKey->SetDataFormat (&c_dfDIKeyboard) != DI_OK)
	{
		WriteError("SetDataFormat fail");
		return false;
	}

	if (m_lpdiKey->Acquire() != DI_OK)
	{
		WriteError("AcquireKey fail");
		return false;
	}

	memset(m_keyState, 0, sizeof(m_keyState));

	return true;
}

//----------------------------------------------------------------------------/

void DxInput::ReleaseKeyBoard()
{
	if (m_lpdiKey)
	{
		m_lpdiKey->Unacquire();
		m_lpdiKey->Release();
		m_lpdiKey = NULL;
	}
}

//------------------------------------------------------------------------------------------/

bool DxInput::InitMouse(int xMouse, int yMouse)
{
	if (m_lpdi->CreateDevice(GUID_SysMouse, &m_lpdiMouse, NULL) != DI_OK)
	{
		WriteError("CreateDevice fail");
		return false;
	}

	if (m_lpdiMouse->SetCooperativeLevel(m_hWnd,DISCL_NONEXCLUSIVE | DISCL_BACKGROUND) != DI_OK)
	{
		WriteError("SetCooperativeLevel fail");
		return false;
	}

	if (m_lpdiMouse->SetDataFormat (&c_dfDIMouse) != DI_OK)
	{
		WriteError("SetDataFormat fail");
		return false;
	}

	if (m_lpdiMouse->Acquire() != DI_OK)
	{
		WriteError("AcquireKey fail");
		return false;
	}

	m_xMouse = xMouse;
	m_yMouse = yMouse;

	memset(&m_mouseState, 0, sizeof(m_mouseState));

	return true;
}

//--------------------------------------------------------------------------------------/

void DxInput::ReleaseMouse()
{
	if (m_lpdiMouse)
	{
		m_lpdiMouse->Unacquire();
		m_lpdiMouse->Release();
		m_lpdiMouse = NULL;
	}
}

//-------------------------------------------------------------------------------------------/

bool DxInput::GetKeyState()
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

int DxInput::KeyDown(BYTE DIKey)
{
	return (m_keyState[DIKey] & 0x80);
}

//---------------------------------------------------------------------------------------------/

bool DxInput::GetMouseState()
{
	if (m_lpdiMouse)
	{
		if ((m_lpdiMouse->GetDeviceState(sizeof(m_mouseState), (LPVOID)&m_mouseState)) != DI_OK)
		{
			return false;
		}

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

int DxInput::GetMouseX()
{
	return m_xMouse;
}

//----------------------------------------------------/

int DxInput::GetMouseY()
{
	return m_yMouse;
}

//------------------------------------------/

bool DxInput::LBtnDown()
{
	return m_bLBtn;
}

//--------------------------------------------

bool DxInput::RBtnDown()
{
	return m_bRBtn;
}

void DxInput::WriteError(const char *error, ...)
{
        char buffer[1024];
        va_list arglist;
        
        va_start(arglist, error);
        vsprintf(buffer, error, arglist);
        va_end(arglist);

        common::Error(error, buffer);
}