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
static char   g_szClass[]   = "ShellClass";
static char   g_szCaption[] = "Shell Application";


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


BOOL DoInit()
{
	g_table.Init(g_hWnd);
	g_input.Init(g_hWnd, g_hInst);
	g_input.InitKeyBoard();
	g_input.InitMouse(0, 0);


	g_camera.SetLookAt(0.0f, 50.0f, -250.0f, 0.0f, 50.0f, 0.0f);

	//g_camera.MoveTo(0.0f, 400.0f, -2000.0f);
	g_table.SetCamera(g_camera);

	g_table.EnableZBuffer(TRUE);

	g_object.LoadXFile(&g_table, "Yodan.x");
	g_object.SetLoop("Walk", TRUE);
	g_object.SetLoop("Idle", TRUE);
	/*

	g_heroMesh.LoadXFile(&g_table, "Yodan.x");

	g_animation.LoadXFile("Yodan.x", &g_heroMesh);
	g_animation.SetLoop("Idle", TRUE);
	g_animation.SetLoop("Walk", TRUE);
	*/
	//g_object.Create(&g_table, &g_heroMesh);

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
		unsigned long Elapsed;
		float         XMove, ZMove;
		short         i;
		static float m_XPos = -50.0f, m_ZPos = -500.0f;

		// Calculate elapsed time (plus speed boost)
		Elapsed = (timeGetTime() - Timer) * 0.5;
		Timer = timeGetTime();

		g_input.GetKeyState();
		g_input.GetMouseState();

		if(g_input.KeyDown(KEY_ESC))
			return FALSE;

		// Process movement
		XMove = ZMove = 0.0f;

		// Process keyboard input to move view

		// Move viewpoint forward
		if(g_input.KeyDown(KEY_UP)) 
		{
		
			XMove = (float)sin(g_camera.GetYRotation()) * Elapsed;
			ZMove = (float)cos(g_camera.GetYRotation()) * Elapsed;
		}

		// Move viewpoint backward
		if(g_input.KeyDown(KEY_DOWN))
		{
		
			XMove = -(float)sin(g_camera.GetYRotation()) * Elapsed;
			ZMove = -(float)cos(g_camera.GetYRotation()) * Elapsed;
		}

		// Strafe left
		if(g_input.KeyDown(KEY_LEFT)) 
		{
		
			XMove=(float)sin(g_camera.GetYRotation()-1.57f)*Elapsed;
			ZMove=(float)cos(g_camera.GetYRotation()-1.57f)*Elapsed;
		}

		// Strafe right
		if(g_input.KeyDown(KEY_RIGHT)) 
		{

			XMove=(float)sin(g_camera.GetYRotation()+1.57f)*Elapsed;
			ZMove=(float)cos(g_camera.GetYRotation()+1.57f)*Elapsed;
		}

		m_XPos += XMove;
		m_ZPos += ZMove;
		

		g_camera.MoveTo(m_XPos + XMove, 50.0f, m_ZPos + ZMove);
		g_camera.Rotate((float)g_input.GetMouseDeltaY() / 500.0f,
			(float)g_input.GetMouseDeltaX() / 500.0f,
			0.0f);
		

		g_table.SetCamera(g_camera);
		static bool a = true;

		if(a)
		{
			g_object.SetAnimation("Walk", timeGetTime()/20);
			//g_animation.SetCurAnimSet("Walk",timeGetTime()/20);
			//g_object.SetAnimation(&g_animation, "Idle", timeGetTime()/20);
			a = false;
		}
		g_object.Render(timeGetTime()/20, TRUE);


		//g_animation.UpdateCurAnim(timeGetTime()/20, TRUE);
		 //g_object.UpdateAnimation(timeGetTime()/20, TRUE);

		//g_heroMesh.Render();
		 //g_object.Render();


		//g_frame.Render();
		
		

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


