

#include "../common/common.h"
#include "../game/render.h"
#include "../game/game.h"
#include "../game/mod.h"
#include "../game/input.h"


#include <windows.h>
#include <stdio.h>



static HWND m_hWnd = NULL;
static CRITICAL_SECTION  cs;

BOOL OnIdle(LONG count);
void CreateLingWindow(WNDPROC _lpfnWindowProc, int width, int height, bool onidle);


//////////////////////////////////////////////////////////////////////////////////////////////////


HWND GetMainHwnd()
{
        return m_hWnd;
}


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
                                /*
                                if(msgCur.message == WM_USER + 0x401)
                                {
                                       //PeekMessage(&msgCur, NULL, 0, 0, PM_REMOVE);
                                        PostMessage(hWnd, WM_CLOSE, 0, 0);
                                }
                                */
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
                while(true)
                {
                        /*
		        if(PeekMessage(&msgCur, NULL, 0, 0, PM_NOREMOVE))
                        {
                                if(msgCur.message == WM_USER + 0x401)
                                {
                                        PostMessage(hWnd, WM_CLOSE, 0, 0);
                                }
                        }
                        */
                        if(GetMessage(&msgCur, NULL, 0, 0) == 0)
                        {
                                break;
                        }
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


void OnPaint(HWND hWnd, HDC hdc)
{
        Mod::PaintMainMenu();
        CRender::Get()->RenderScene ();
}

void OnCreate(HWND hWnd)
{

        InitializeCriticalSection(&cs);
        m_hWnd = hWnd;


        CGame * game = CGame::Get();
        CRender * render = CRender::Get();
        
        Input::AtachInput (hWnd);

        if(!render->InitRender("glrender.dll"))
        {
                common::Error("no render dll...");
                MessageBox(hWnd, "找不到渲染引擎。", "错误", MB_OK);
                exit(1);
        }
        if(!render->Attach(hWnd))
        {
                common::Error("无法附加到窗口。");
                MessageBox(hWnd, "无法附加到窗口。", "错误", MB_OK);
                exit(1);
        }
        render->Start();
        game->Run();
        
        Mod::LoadMod (".\\Mod");
        Mod::ShowMainMenu (hWnd);

}

void OnClose()
{       
        CGame * game = CGame::Get();
        CRender * render = CRender::Get();
        
        game->Stop();
        render->Stop();

        
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
                OnClose();
		PostQuitMessage (0);
		break;
	case WM_DESTROY:
		exit(0);
		break;
        case WM_PAINT:
                BeginPaint(hwnd, &ps);
                OnPaint(hwnd, ps.hdc);
                EndPaint(hwnd, &ps);
                break;
        case WM_ERASEBKGND:
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam); 
	} 
 	return 0; 
}
 

int main(int argc, char ** argv)
{
	CreateLingWindow(_WndProc, 800, 600, false);
	return 0;
}







