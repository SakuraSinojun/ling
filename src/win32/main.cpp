

#include "../common/common.h"
#include "../game/render.h"
#include "../game/game.h"
#include "../game/mod.h"
#include "../game/input.h"
#include "../game/gameconfig.h"


#include "display.h"

#include <windows.h>




static HWND m_hWnd = NULL;
static CRITICAL_SECTION  cs;

BOOL OnIdle();
void CreateLingWindow(WNDPROC _lpfnWindowProc, int width, int height, bool onidle);


//////////////////////////////////////////////////////////////////////////////////////////////////


HWND GetMainHwnd()
{
        return m_hWnd;
}


void SwitchFullScreen(BOOL bFull)
{
        CDisplay * display = CDisplay::Get();
        CGameConfig *   pConfig = CGameConfig::Get();
        LONG    style;

        
        if(m_hWnd == NULL)
        {       
                return ;
        }
        
        style = GetWindowLong(m_hWnd, GWL_STYLE);

        if(bFull)
        {
                style &= ~WS_CAPTION;
                MoveWindow (m_hWnd, 0, 0, pConfig->WindowWidth(), pConfig->WindowHeight(), FALSE);
                display->ChangeDisplayMode (pConfig->WindowWidth(), pConfig->WindowHeight());
        }else{
                style |= WS_CAPTION;
                display->RestoreDisplayMode ();
        }
        
        SetWindowLong(m_hWnd, GWL_STYLE, style);


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
	
	DWORD	dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX ;
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
		for(;;)
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
				if(!OnIdle())
					idle = false;
			}else{
				::WaitMessage();
			}
		}
	}else{
                for(;;)
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


BOOL OnIdle()
{
	return TRUE;
}


void OnPaint()
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
        CGameConfig * config = CGameConfig::Get();
        

        Input::AtachInput (hWnd);

        if(!render->InitRender(config->GetRender()))
        {
                common::Error("no render dll...");
                MessageBox(hWnd, "�Ҳ�����Ⱦ���档", "����", MB_OK);
                exit(1);
        }
        if(!render->Attach(hWnd))
        {
                common::Error("�޷����ӵ����ڡ�");
                MessageBox(hWnd, "�޷����ӵ����ڡ�", "����", MB_OK);
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
        CDisplay * display = CDisplay::Get();

        display->RestoreDisplayMode ();
        game->Stop();
        render->Stop();

        delete display;
        delete render;
        delete game;
        
}

////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK _WndProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam) 
{ 

        PAINTSTRUCT 	ps;
	CGameConfig *   pConfig;
        // CGame *         game; 
        // CRender *       render; 

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
                OnPaint();
                EndPaint(hwnd, &ps);
                break;
        case WM_KILLFOCUS:
                // game = CGame::Get();
                // render = CRender::Get();
                // game->Pause ();
                // render->Pause ();
                SwitchFullScreen(FALSE);
                // ShowWindow(hwnd, SW_MINIMIZE);
                break;
        case WM_SETFOCUS:
                pConfig = CGameConfig::Get();
                // game = CGame::Get();
                // render = CRender::Get();

                // render->Start ();
                // game->Run ();      

                if(pConfig->IsFullScreen ())
                {
                        SwitchFullScreen(TRUE);
                }
                break;
        case WM_ERASEBKGND:
	default:
	        return DefWindowProc(hwnd,uMsg,wParam,lParam); 
	}

	
 	return 0; 
}
 

int main()
{
        CGameConfig *   pConfig = CGameConfig::Get();

	CreateLingWindow(_WndProc, pConfig->WindowWidth(), pConfig->WindowHeight(), false);
	return 0;
}







