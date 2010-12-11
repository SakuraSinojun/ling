

#include "common/common.h"
#include "render.h"

#include <windows.h>
#include <stdio.h>





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
	wndclas.style		= CS_VREDRAW | CS_HREDRAW | CS_OWNDC; 
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




////////////////////////////////////////////////////////////////////////////////


BOOL OnIdle(LONG count)
{
	return TRUE;
}

void OnCreate(HWND hWnd)
{
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







