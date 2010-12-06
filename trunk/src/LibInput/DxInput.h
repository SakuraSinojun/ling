#ifndef DXINPUT_H_101206
#define DXINPUT_H_100206

#pragma comment(lib,"dinput8.lib")
#include <dinput.h>

class DxInput
{
public:
	DxInput(void);
	~DxInput(void);

	// 初始化DXINPUT

	bool Init(HWND hWnd, HINSTANCE hInstance);

	// 关闭DxInput

	void ShutDown();

	// 初始化键盘

	bool InitKeyBoard();

	// 释放键盘

	void ReleaseKeyBoard();

	// 初始化鼠标并定义鼠标初始位置

	bool InitMouse(int xMouse, int yMouse);

	// 释放鼠标

	void ReleaseMouse();

	// 将当前的按键信息储存起来

	bool GetKeyState();

	// 判断某个按键是否被按下 DIK_***
	int KeyDown(BYTE DIKey);

	// 储存鼠标信息

	bool GetMouseState();

	// 得到鼠标的位置

	int GetMouseX();
	int GetMouseY();

	// 判断鼠标的左，右键是否被按下

	bool LBtnDown();
	bool RBtnDown();
	
        void WriteError(const char *error, ...);

private:
	LPDIRECTINPUT8       m_lpdi;
	LPDIRECTINPUTDEVICE8 m_lpdiKey;
	LPDIRECTINPUTDEVICE8 m_lpdiMouse;
	DIMOUSESTATE         m_mouseState;
	BYTE                 m_keyState[256];

	int m_xMouse;
	int m_yMouse;

	bool m_bLBtn;
	bool m_bRBtn;

	HINSTANCE m_hInstance;
	HWND	m_hWnd;
};


#endif