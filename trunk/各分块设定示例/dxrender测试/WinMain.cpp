//需要加载以下库文件
//#pragma comment(lib, "d3d9.lib")	//DX9
//#pragma comment(lib, "d3dx9.lib")	//D3DX
//#pragma comment(lib, "winmm.lib")	//
//#pragma comment(lib, "D3dxof.lib")	// DXFILE
//#pragma comment(lib,"DXGUID.LIB")  // GUID

#include <windows.h>
#include <stdio.h>

#include "d3d9.h"
#include <d3dx9.h>
#include <DXFile.h>
#include "Graphics.h"


HWND          g_hWnd;
HINSTANCE     g_hInst;
static char   g_szClass[]   = "ShellClass";
static char   g_szCaption[] = "Shell Application";



CTexture      g_texture;
CGraphics     g_table;
CVertex       g_vb;
CFont         g_font;
D3DXMATRIX    g_worldMatrix;
D3DXMATRIX    g_viewMatrix;
CLight        g_light;
CCamera       g_camera;
CPosition     g_pos;
CFrame        g_frame;


BOOL DoInit()
{
	g_table.Init(g_hWnd);

	
	g_camera.SetLookAt(0.0f, 50.0f, -120.0f, 0.0f, 50.0f, 0.0f);

	g_table.SetCamera(g_camera);

	g_table.EnableZBuffer(TRUE);

	g_frame.LoadXFile(&g_table, "Warrior.x");

	g_table.EnableLighting(TRUE);

	
	//g_font.Create(&g_table, 12, NULL, TRUE);

	
	g_table.EnableLighting(TRUE);
	g_light.SetDiffuseColor(255, 255, 255);
	g_light.SetAmbientColor(100, 100, 100);
	g_light.SetRange(1000.0f);
	g_light.SetAttenuation0(0.5f);
	g_light.MoveTo(1.0f, -1.0f, 1.0f);

	g_table.SetLignt(0, g_light);
	g_table.EnableLight(0,TRUE);
	
	/*
	
	g_texture.LoadFromFile(&g_table, "Texture.bmp");

	WireVertex vertices[8];
	vertices[0] = WireVertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = WireVertex(-1.0f,  1.0f, -1.0f);
	vertices[2] = WireVertex( 1.0f,  1.0f, -1.0f);
	vertices[3] = WireVertex( 1.0f, -1.0f, -1.0f);
	vertices[4] = WireVertex(-1.0f, -1.0f,  1.0f);
	vertices[5] = WireVertex(-1.0f,  1.0f,  1.0f);
	vertices[6] = WireVertex( 1.0f,  1.0f,  1.0f);
	vertices[7] = WireVertex( 1.0f, -1.0f,  1.0f);

	g_vb.CreateBuffer(&g_table, 8, 1, sizeof(WireVertex), WireVertex::FVF);
	g_vb.SetBuffer(0, 8, (void*)vertices);

	WORD indices[36];

	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
	indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;

	// back side
	indices[6]  = 4; indices[7]  = 6; indices[8]  = 5;
	indices[9]  = 4; indices[10] = 7; indices[11] = 6;

	// left side
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	// right side
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	// top
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	// bottom
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	g_vb.CreateIndex(&g_table, 36, sizeof(WORD), D3DFMT_INDEX16);
	g_vb.SetIndex(0, 36, indices);
	*/


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
	g_table.Clear();

	//g_table.ClearDisplay();

	if(g_table.BeginScene())
	{	

		//g_table.SetShadeMode();
		//D3DXMatrixTranslation(&g_worldMatrix, -1.25f, 0.0f, 0.0f);
		//D3DXMatrixRotationY(&g_worldMatrix, (float)timeGetTime() / 1000.0f);
		//g_table.SetWorldPos(g_worldMatrix);

		//g_camera.RotateTo(0.0f, (float)timeGetTime() / 1000.0f, 0.0f);
		//g_table.SetCamera(g_camera);
		//g_table.SetWorldPos(g_worldMatrix);

		

		
		g_pos.RotateTo(0.0f, (float)timeGetTime() / 1000.0f,0.0f);
		g_table.SetWorldPos(g_pos);
		//g_vb.Render(0, 2, D3DPT_TRIANGLESTRIP);
		

		//D3DXMatrixTranslation(&g_worldMatrix, 1.25f, 0.0f, 0.0f);
		//g_table.SetShadeMode(D3DSHADE_GOURAUD);
		//g_table.SetWorldPos(g_worldMatrix);
		//g_vb.Render(0, 1);

		//g_texture.Blt(0, 0, 20, 20, 50, 50);

		//g_font.Print("hello, world", 0, 0, 500, 300);

		//g_pos.RotateTo(0.0f, 0.0f, (float)timeGetTime() / 1000.0f);
		//g_table.SetWorldPos(g_pos);

		/*
		static float y = 0.0f;
		g_pos.RotateTo(3.14f / 4.0f, y, 0.0f);
		y += 0.05;
		if(y > 6.28)
		{
			y = 0;
		}

		g_table.SetWorldPos(g_pos);

		g_vb.Render(0, 12, D3DPT_TRIANGLELIST);
		*/

		g_frame.Render();

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


