#pragma comment(lib, "d3d9.lib")	//DX9
#pragma comment(lib, "d3dx9.lib")	//D3DX
#pragma comment(lib, "winmm.lib")	//
#pragma comment(lib, "D3dxof.lib")	// DXFILE
#pragma comment(lib,"DXGUID.LIB")  // GUID
#pragma comment(lib,"dinput8.lib")

#include <windows.h>
#include <stdio.h>

#include "DxInput.h"
#include "Graphics.h"


HWND          g_hWnd;
HINSTANCE     g_hInst;
static TCHAR   g_szClass[]   = _T("ShellClass");
static TCHAR   g_szCaption[] = _T("Shell Application");


CDXInput      g_input;
CTexture      g_texture;
CGraphics     g_table;
CVertex       g_vb;
CFont         g_font;
D3DXMATRIX    g_worldMatrix;
D3DXMATRIX    g_viewMatrix;
CLight        g_light;
CCamera       g_camera;
CPosition     g_pos;
CMesh         g_frame;

CMesh         g_sceneMesh;
CMesh         g_heroMesh;
CObject       g_object;
CAnimation    g_animation;

CMesh         g_roomMesh[2];
CObject       g_roomObj[8];

char *name[6] = {
	"Idle",
	"Walk",
	"Swing",
	"Hurt",
	"Die",
	"Spell"};
BOOL DoInit()
{
	g_table.Init(g_hWnd);
	g_input.Init(g_hWnd, g_hInst);
	g_input.InitKeyBoard();
	g_input.InitMouse(0, 0);

	g_font.Create(&g_table, 24, _T("黑体"));

	//g_camera.SetLookAt(0.0f, 50.0f, -250.0f, 0.0f, 50.0f, 0.0f);

	//g_table.SetCamera(g_camera);

	g_table.EnableZBuffer(TRUE);

	if(!g_object.LoadXFile(&g_table, "..\\Data\\Warrior.x","..\\Data\\"))
	{
		return FALSE;
	}
	g_object.SetLoop("Walk", TRUE);
	g_object.SetLoop("Idle", TRUE);
	g_object.SetLoop("Swing", TRUE);
	g_object.SetLoop("Hurt", TRUE);
	g_object.SetLoop("Die", TRUE);
	g_object.SetLoop("Spell", TRUE);
	g_object.ScaleTo(10.0f, 10.0f, 10.0f);
	g_object.MoveTo(-2000.0f, 0.0f, -1000.0f);


	if(!g_roomMesh[0].LoadXFile(&g_table, "..\\Data\\Corridor.x", "..\\Data\\"))
	{
		return FALSE;
	}
	if(!g_roomMesh[1].LoadXFile(&g_table, "..\\Data\\Room.x", "..\\Data\\"))
	{
		return FALSE;
	}

	g_roomObj[0].Create(&g_roomMesh[1]);
	g_roomObj[1].Create(&g_roomMesh[0]);
	g_roomObj[2].Create(&g_roomMesh[1]);
	g_roomObj[3].Create(&g_roomMesh[0]);
	g_roomObj[4].Create(&g_roomMesh[0]);
	g_roomObj[5].Create(&g_roomMesh[1]);
	g_roomObj[6].Create(&g_roomMesh[0]);
	g_roomObj[7].Create(&g_roomMesh[1]);

	// Move room objects into position
	g_roomObj[0].MoveTo(-2000.0f, 0.0f,  2000.0f);
	g_roomObj[1].MoveTo(    0.0f, 0.0f,  2000.0f);
	g_roomObj[2].MoveTo( 2000.0f, 0.0f,  2000.0f);
	g_roomObj[3].MoveTo(-2000.0f, 0.0f,     0.0f);
	g_roomObj[4].MoveTo( 2000.0f, 0.0f,     0.0f);
	g_roomObj[5].MoveTo(-2000.0f, 0.0f, -2000.0f);
	g_roomObj[6].MoveTo(    0.0f, 0.0f, -2000.0f);
	g_roomObj[7].MoveTo( 2000.0f, 0.0f, -2000.0f);

	// Rotate room objects into proper position
	g_roomObj[1].RotateTo(0.0f,  1.57f, 0.0f);
	g_roomObj[2].RotateTo(0.0f,  1.57f, 0.0f);
	g_roomObj[5].RotateTo(0.0f, -1.57f, 0.0f);
	g_roomObj[6].RotateTo(0.0f, -1.57f, 0.0f);
	g_roomObj[7].RotateTo(0.0f,  3.14f, 0.0f);

	// Position view in a room

	return TRUE;
}

BOOL DoShutdown()
{
	g_table.ShutDown();
	g_font.Destroy();
	g_texture.Destroy();
	g_vb.Destroy();


	return TRUE;
}

BOOL DoFrame()
{


	g_table.Clear(D3DCOLOR_RGBA(0,64,128,255));

	if(g_table.BeginScene())
	{	

		static DWORD  Timer = timeGetTime();
		static DWORD startTime = timeGetTime();
		static DWORD count = 0;
		static DWORD freq = 0;
		
		unsigned long Elapsed;
		float         XMove, ZMove;
		static float m_XPos = -2000.0f, m_ZPos = -2000.0f;

		// Calculate elapsed time (plus speed boost)
		Elapsed = (timeGetTime() - Timer) * 2;
		Timer = timeGetTime();

		count++;

		TCHAR szFreq[100];
		_stprintf(szFreq, _T("帧速:%d"), freq);

		
		
		g_font.Print(szFreq, 0, 0, 100, 25);
		g_font.Print(_T("空格键切换动作"), 0, 25, 200, 25);

		
		if(Timer - startTime> 1000)
		{
			startTime = Timer;
			freq = count;
			count = 0;
		}
		
		

		g_input.GetKeyState();
		g_input.GetMouseState();

		if(g_input.KeyDown(KEY_ESC))
			return FALSE;

		XMove = ZMove = 0.0f;

		if(g_input.KeyDown(KEY_UP)) 
		{
		
			XMove = (float)sin(g_camera.GetYRotation()) * Elapsed;
			ZMove = (float)cos(g_camera.GetYRotation()) * Elapsed;
		}

		if(g_input.KeyDown(KEY_DOWN))
		{
		
			XMove = -(float)sin(g_camera.GetYRotation()) * Elapsed;
			ZMove = -(float)cos(g_camera.GetYRotation()) * Elapsed;
		}

		if(g_input.KeyDown(KEY_LEFT)) 
		{
		
			XMove=(float)sin(g_camera.GetYRotation()-1.57f)*Elapsed;
			ZMove=(float)cos(g_camera.GetYRotation()-1.57f)*Elapsed;
		}

		if(g_input.KeyDown(KEY_RIGHT)) 
		{

			XMove=(float)sin(g_camera.GetYRotation()+1.57f)*Elapsed;
			ZMove=(float)cos(g_camera.GetYRotation()+1.57f)*Elapsed;
		}

		static int nAnim = 0;
		char **apAnim = name;
		static bool bTran = true;
		
		if(g_input.KeyDown(KEY_SPACE))
		{
			Sleep(100);
			nAnim++;
			if(nAnim >5)
			{
				nAnim = 0;
			}

			apAnim+= nAnim;

			bTran = true;
		}

		static int dist = 1;
		static int z = 0;

		if(nAnim == 1)
		{
			if(dist == 1)
			{
				z++;
				g_object.Move(0, 0, 5.0f);
			}
			else
			{
				z--;
				g_object.Move(0, 0, -5.0f);
			}
			if(z>500)
			{
				dist = 0;
				g_object.Rotate(0, 3.14f, 0);
				
			}
			if(z<0)
			{
				dist = 1;
				g_object.Rotate(0, 3.14f, 0);
			}
		}

		m_XPos += XMove;
		m_ZPos += ZMove;

		g_camera.MoveTo(m_XPos + XMove, 400.0f, m_ZPos + ZMove);
		g_camera.Rotate((float)g_input.GetMouseDeltaY() / 500.0f,
			(float)g_input.GetMouseDeltaX() / 500.0f,
			0.0f);
		
		g_table.SetCamera(g_camera);
		

		if(bTran)
		{
			g_object.SetAnimation(*apAnim, timeGetTime()/20);
			bTran = false;
		}

		for(int i=0; i<8; i++)
		{
			g_roomObj[i].Render();
		}

		g_object.Render(timeGetTime()/20, TRUE);
		

		g_table.EndScene();

	}
	g_table.Display();

	

	return TRUE;
}


// Function prototypes
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev,        
				   LPSTR szCmdLine, int nCmdShow);
long FAR PASCAL WindowProc(HWND hWnd, UINT uMsg,            
						   WPARAM wParam, LPARAM lParam);

int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;
	MSG        Msg;

	g_hInst = hInst;

	// Create the window class here and register it
	wcex.cbSize        = sizeof(wcex);
	wcex.style         = CS_CLASSDC;
	wcex.lpfnWndProc   = WindowProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInst;
	wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = g_szClass;
	wcex.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	if(!RegisterClassEx(&wcex))
		return FALSE;

	// Create the Main Window
	g_hWnd = CreateWindow(
		g_szClass, g_szCaption,
		WS_CAPTION | WS_SYSMENU,
		0, 0, 640, 480,
		NULL, NULL,
		hInst, NULL );

	if(!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, SW_NORMAL);
	UpdateWindow(g_hWnd);

	// Run init function and return on error
	if(DoInit() == FALSE)
	{
		return FALSE;
	}

	// Start message pump, waiting for signal to quit
	ZeroMemory(&Msg, sizeof(MSG));
	while(Msg.message != WM_QUIT) 
	{
		if(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		if(DoFrame() == FALSE)
		{
			break;
		}
	}

	// Run shutdown function
	DoShutdown();

	UnregisterClass(g_szClass, hInst);

	return Msg.wParam;
}

long FAR PASCAL WindowProc(HWND hWnd, UINT uMsg,              \
						   WPARAM wParam, LPARAM lParam)
{
	switch(uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


