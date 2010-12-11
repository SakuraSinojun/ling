#include "common.h"
#include "Game.h"

#include <windows.h>
#include <stdio.h>


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32
#define MAX_COLOR 256
#define WINDOW_MODE	1


CGame game;



typedef struct
{
	BITMAPINFOHEADER	Info;
	DWORD			BitField[3];
}HEADER;


static	HWND	m_hWnd;

void CalcFPS();
BOOL OnIdle(LONG count);
void CreateLingWindow(WNDPROC _lpfnWindowProc, int width, int height, bool onidle);




void CreateLingWindow(WNDPROC _lpfnWindowProc, int width, int height, bool onidle = true)
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
	rect.right = SCREEN_WIDTH;
	rect.bottom = SCREEN_HEIGHT;
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


 
       

/*	RECT rcClient = {0, 0, SCREEN_WIDTH - 1,SCREEN_HEIGHT - 1};

	if(WINDOW_MODE)
	{
		hWnd = CreateWindow ("ling", TEXT("ling"), WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION,
			CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
	
		AdjustWindowRectEx(&rcClient,
			GetWindowLong(hWnd,GWL_STYLE),
			GetMenu(hWnd) != NULL, 
			GetWindowLong(hWnd,GWL_EXSTYLE)); 

		MoveWindow(hWnd, 50 ,50 ,rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,true);

		ShowWindow(hWnd, SW_SHOW);
	}
	else
	{
		hWnd = CreateWindow ("ling", TEXT("hello win32"), WS_POPUP | WS_VISIBLE, 0, 
			0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);
	}*/
        
        
	
	MSG 		msgCur; 
	bool		idle=true;
	int		count = 0;
	

               
	if(onidle)
	{
                if(!game.InitGame(SCREEN_WIDTH,SCREEN_HEIGHT,WINDOW_MODE, hWnd))
                {
                        MessageBox(NULL, "initGame fail", "¥ÌŒÛ", 0);
                        return;
                }

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

                        if(!game.GameLoop())
                        {
                                MessageBox(NULL, "game loop fail", "¥ÌŒÛ", 0);
                                return ;
                        }
		}
	}else{
		while(GetMessage(&msgCur, NULL, 0, 0))  {
			TranslateMessage(&msgCur);
			DispatchMessage(&msgCur);
		}
	}

        game.ShutGame();

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
		sprintf_s(FPS,"FPS: %.2f÷°/√Î", fps);
#else
		sprintf(FPS,"FPS: %.2f÷°/√Î", fps);
#endif
#else
		sprintf(FPS,"FPS: %.2f÷°/√Î", fps);
#endif

		SetWindowText(m_hWnd, FPS);
	}
	
}



////////////////////////////////////////////////////////////////////////////////

static HBITMAP	m_hBitmap;


BOOL OnIdle(LONG count)
{
	CalcFPS();
	return TRUE;
}

void OnPaint(HWND hWnd, HDC hdc)
{
	HDC	memdc;
	memdc = CreateCompatibleDC(hdc);
	SelectObject(memdc, m_hBitmap);
	BitBlt(hdc, 0, 0, 640, 480, memdc, 0, 0, SRCCOPY);
	DeleteDC(memdc);

}

void OnCreate(HWND hWnd)
{

	BMPINFOHEADER	biHeader;
	void *	buffer = res::loadimg ("bg4.bmp", &biHeader);
	
	m_hBitmap = CreateBitmap(biHeader.biWidth, biHeader.biHeight, biHeader.biPlanes,
				biHeader.biBitCount, buffer);
	
	//m_hBitmap = CreateDIBSection(hdc, (BITMAPINFO *)&Header, DIB_RGB_COLORS, (void **)&buffer, NULL, 0);
	
}

void OnLButtonDown(HWND hWnd, int x, int y)
{
	LARGE_INTEGER 		litmp;
	LONGLONG 		qt1;
	LONGLONG		qt2;
	double 			dft;
	double			dff;
	double			dfm;
	int			i;
	unsigned int	len;
	char	*	buffer;

	QueryPerformanceFrequency(&litmp);
	dff = (double)litmp.QuadPart;	
	
	//buffer = (char *)pl::load("common.h", len);

	QueryPerformanceCounter(&litmp);
	qt1 = litmp.QuadPart;
	
	for(i=0; i<100000; i++)
	{
		buffer = (char *)res::loadimg("bg1.bmp", NULL);
	}

	QueryPerformanceCounter(&litmp);
	qt2 = litmp.QuadPart;
	
	dfm = (double)(qt2 - qt1);
	dft = dfm / dff;
	
	printf("º”‘ÿ100000¥Œ£¨π≤”√ ± %f √Î\n", dft);
	//MessageBox(hWnd, "Ω· ¯", "..", MB_OK);


}

void OnRButtonDown(int x, int y)
{
	pl::unload("common.h");
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
	CreateLingWindow(_WndProc, 800, 600);
	return 0;
}







