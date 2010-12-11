

#include "common.h"

#include <windows.h>
#include <stdio.h>

#include "game.h"



typedef struct
{
	BITMAPINFOHEADER	Info;
	DWORD			BitField[3];
}HEADER;


//static	HWND	m_hWnd;

BOOL OnIdle(HWND hWnd, LONG count);
void CreateLingWindow(WNDPROC _lpfnWindowProc, int width, int height, bool onidle);




void CreateLingWindow(WNDPROC _lpfnWindowProc, int width, int height, bool onidle = false)
{
	HWND		hWnd;
	HINSTANCE	hInstance;
	
	hInstance = GetModuleHandle(NULL);
	
	WNDCLASS wndclas; 
	wndclas.cbClsExtra 	= 0; 
	wndclas.cbWndExtra 	= 0; 
	wndclas.hbrBackground 	= (HBRUSH)GetStockObject(WHITE_BRUSH); 
	wndclas.hCursor 	= LoadCursor(NULL,IDC_ARROW); 
	wndclas.hIcon 		= LoadIcon(NULL,IDI_APPLICATION); 
	wndclas.hInstance 	= hInstance; 
	wndclas.lpfnWndProc 	= _lpfnWindowProc; 
	wndclas.lpszClassName	= "LINGWINDOW"; 
	wndclas.lpszMenuName	= NULL; 
	wndclas.style		= CS_VREDRAW | CS_HREDRAW; 
	RegisterClass(&wndclas); 
	
	DWORD	dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_DLGFRAME | WS_MAXIMIZEBOX | WS_THICKFRAME;
	RECT	rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;
	AdjustWindowRect(&rect, dwStyle, FALSE);
	
	hWnd = CreateWindow("LINGWINDOW",
				"ling",
				dwStyle ,
				CW_USEDEFAULT,
				CW_USEDEFAULT, 
				rect.right-rect.left,
				rect.bottom-rect.top,
				NULL,
				NULL,
				hInstance,
				NULL); 
	
	ShowWindow (hWnd,SW_SHOWNORMAL); 
	
	MSG 		msgCur; 
	bool		idle=true;
	int		count = 0;
	

	if(onidle)
	{
		while(true)
		{
			if (PeekMessage(&msgCur, NULL, 0, 0, PM_NOREMOVE))
			{
				if (!GetMessage(&msgCur, NULL, 0, 0))
					return;

				//if (!PreTranslateMessage(&msgCur))
				{
					//TranslateMessage(&msgCur);
					DispatchMessage(&msgCur);
				}
				idle = true;
				count = 0;
			}else if (idle){
				if(count==MAXLONG)
					count=0;
				if(!OnIdle(hWnd, count++))
					idle = false;
			}else{
				WaitMessage();
			}
		}
	}else{
		while(GetMessage(&msgCur, NULL, 0, 0))  {
			TranslateMessage(&msgCur);
			DispatchMessage(&msgCur);
		}
	}

	return ;
}




////////////////////////////////////////////////////////////////////////////////

HBITMAP	m_hBitmap;


void CalcFPS(HWND hWnd)
{
		
	LARGE_INTEGER 		litmp;
	LONGLONG 		qt1;
	static LONGLONG	qt2 = 0;
	double 		dft;
	double			dff;
	double			dfm;
	char			FPS[100];
	static LONGLONG	framecount=0;
	double 		fps;
	
	QueryPerformanceFrequency(&litmp);
	dff = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);
	qt1 = litmp.QuadPart;
	
	framecount++;
	
	if(qt2 == 0)
	{
		qt2 = qt1;
		return;
	}
	
	dfm = (double)(qt1 - qt2);
	dft = dfm / dff;
	
	if(dft > 1)
	{
		fps = framecount / dft;
		framecount = 0;
		qt2 = qt1;
		
#ifdef _MSC_VER
#if _MSC_VER > 1200
		sprintf_s(FPS,"FPS: %.2f÷°/√Î", fps);
#else
		sprintf(FPS,"FPS: %.2f÷°/√Î", fps);
#endif
#else
		sprintf(FPS,"FPS: %.2f÷°/√Î", fps);
#endif

		SetWindowText(hWnd, FPS);
	}
	
}


BOOL OnIdle(HWND hWnd, LONG count)
{
	CalcFPS(hWnd);
	return game_loop(hWnd);
	
}

void OnPaint(HWND hWnd, HDC hdc)
{
	game_loop(hWnd);
	return;
	/*
	HDC	memdc;
	memdc = CreateCompatibleDC(hdc);
	SelectObject(memdc, m_hBitmap);
	BitBlt(hdc, 0, 0, 640, 480, memdc, 0, 0, SRCCOPY);
	DeleteDC(memdc);
	*/
}

void OnCreate(HWND hWnd)
{
	/*
	BMPINFOHEADER	biHeader;
	void *	buffer = res::loadimg ("CGData\\bg1.bmp", &biHeader);
	
	m_hBitmap = CreateBitmap(biHeader.biWidth, biHeader.biHeight, biHeader.biPlanes,
				biHeader.biBitCount, buffer);
	
	//m_hBitmap = CreateDIBSection(hdc, (BITMAPINFO *)&Header, DIB_RGB_COLORS, (void **)&buffer, NULL, 0);
	*/
}

void OnLButtonDown(HWND hWnd, int x, int y)
{

}

void OnRButtonDown(int x, int y)
{
	//pl::unload("common.h");
}


////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK _WndProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam) 
{ 

	PAINTSTRUCT 	ps;
	
	
	switch(uMsg) 
	{
	case WM_CREATE:
		OnCreate(hwnd);
		break;
	case WM_CLOSE:
		PostQuitMessage (0);
		break;
	case WM_DESTROY:
		exit(0);
		break;
	case WM_PAINT:
		BeginPaint (hwnd, &ps);
		OnPaint(hwnd, ps.hdc);
		EndPaint (hwnd,&ps);
		break;
	case WM_KEYDOWN:
		//printf("wParam = %d, lParam = %d\n", wParam, lParam);
		game_input(KEY_DOWN, (int)wParam);
		break;
	case WM_KEYUP:
		//printf("wParam = %d, lParam = %d\n", wParam, lParam);
		game_input(KEY_UP, (int)wParam);
		break;
	case WM_LBUTTONDOWN:
		OnLButtonDown(hwnd, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		OnRButtonDown(LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam); 
	} 
 	return 0; 
}
 

int main(int argc, char ** argv)
{
	CreateLingWindow(_WndProc, 800, 600, true);
	return 0;
}







