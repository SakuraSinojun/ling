#ifndef DXINPUT_H_101206
#define DXINPUT_H_100206

#pragma comment(lib,"dinput8.lib")
#include <dinput.h>

class DxInput
{
public:
	DxInput(void);
	~DxInput(void);

	// ��ʼ��DXINPUT

	bool Init(HWND hWnd, HINSTANCE hInstance);

	// �ر�DxInput

	void ShutDown();

	// ��ʼ������

	bool InitKeyBoard();

	// �ͷż���

	void ReleaseKeyBoard();

	// ��ʼ����겢��������ʼλ��

	bool InitMouse(int xMouse, int yMouse);

	// �ͷ����

	void ReleaseMouse();

	// ����ǰ�İ�����Ϣ��������

	bool GetKeyState();

	// �ж�ĳ�������Ƿ񱻰��� DIK_***
	int KeyDown(BYTE DIKey);

	// ���������Ϣ

	bool GetMouseState();

	// �õ�����λ��

	int GetMouseX();
	int GetMouseY();

	// �ж��������Ҽ��Ƿ񱻰���

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