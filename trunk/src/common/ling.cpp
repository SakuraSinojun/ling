

#include "common.h"

#include <windows.h>
#include <stdio.h>



static	HWND	m_hWnd;

void CalcFPS();
BOOL OnIdle(LONG count);
void CreateLingWindow(WNDPROC _lpfnWindowProc, int width, int height, bool onidle);




void CreateLingWindow(WNDPROC _lpfnWindowProc, int width, int height, bool onidle = false)
{
	HWND		hWnd;
	HINSTANCE	hInstance;
	
	hInstance = GetModuleHandle(NULL);
	
	WNDCLASS wndclas; 
	wndclas.cbClsExtra 	= 0; 
	wndclas.cbWndExtra 	= 0; 
	wndclas.hbrBackground 	= (HBRUSH)GetStockObject(BLACK_BRUSH); 
	wndclas.hCursor 	= LoadCursor(NULL,IDC_ARROW); 
	wndclas.hIcon 		= LoadIcon(NULL,IDI_APPLICATION); 
	wndclas.hInstance 	= hInstance; 
	wndclas.lpfnWndProc 	= _lpfnWindowProc; 
	wndclas.lpszClassName	= "LINGWINDOW"; 
	wndclas.lpszMenuName	= NULL; 
	wndclas.style		= CS_VREDRAW | CS_HREDRAW; 
	RegisterClass(&wndclas); 
	
	DWORD	dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_DLGFRAME;
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
			if (::PeekMessage(&msgCur, NULL, 0, 0, PM_NOREMOVE))
			{
				if (!::GetMessage(&msgCur, NULL, 0, 0))
					return;

				//if (!PreTranslateMessage(&msgCur))
				{
					::TranslateMessage(&msgCur);
					::DispatchMessage(&msgCur);
				}
				idle = true;
				count = 0;
			}else if (idle){
				if(count==MAXLONG)
					count=0;
				if(!OnIdle(count++))
					idle = false;
			}else{
				::WaitMessage();
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


void CalcFPS()
{
		
	LARGE_INTEGER 		litmp;
	LONGLONG 			qt1;
	static LONGLONG		qt2 = 0;
	double 			dft;
	double			dff;
	double			dfm;
	char				FPS[100];
	static LONGLONG		framecount=0;
	double 			fps;
	
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
		sprintf_s(FPS,"FPS: %.2f֡/��", fps);
#else
		sprintf(FPS,"FPS: %.2f֡/��", fps);
#endif
#else
		sprintf(FPS,"FPS: %.2f֡/��", fps);
#endif

		SetWindowText(m_hWnd, FPS);
	}
	
}



////////////////////////////////////////////////////////////////////////////////

BOOL OnIdle(LONG count)
{
	CalcFPS();
	return TRUE;
}

void OnPaint(HWND hWnd, HDC hdc)
{
}


////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK _WndProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam) 
{ 

	PAINTSTRUCT 	ps;
	char	*	buffer;
	unsigned int	len;
	
	switch(uMsg) 
	{
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
	case WM_LBUTTONDOWN:
		buffer = (char *)pl::load("common.h", len);
		MessageBox(hwnd, buffer, "..", MB_OK);
		break;
	case WM_RBUTTONDOWN:
		pl::unload("common.h");
		break;
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam); 
	} 
 	return 0; 
}
 

int main(int argc, char ** argv)
{
	CreateLingWindow(_WndProc, 800, 600);
	return 0;
}







