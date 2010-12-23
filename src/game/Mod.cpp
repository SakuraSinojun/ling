

#define __DEBUG__MOD_CPP__

#include "../common/common.h"
#include "mod.h"
#include "game.h"
#include "render.h"
#include "input.h"





#include <windows.h>



/*

    >> Warning <<
        按住鼠标再按ESC来退出游戏会卡住线程锁，如果没有其它问题只需加个
    MessageBox卡住线程即可。现在可以同时检测多个按键。

*/


namespace       Mod
{

        struct MOD_STRUCT
        {
                char    modpath[256];
                char    modname[256];
                char    menubgpic[256];
        }ms;
        
        static HWND     m_hWnd = NULL;
        static bool     m_ShowMainMenu = false; 
        
        static CRITICAL_SECTION  cs;
        static INPUT_STRUCT      wi;
        static RECT              rcStart;
        static RECT              rcQuit;
        static BOOL              bModLoaded = FALSE;
      
        
        void MouseButtonDown();
        void MouseMove(int x, int y);
        void StartMod();
        void QuitMod();
        bool IsMouseInPanel(int x, int y);
        bool IsMainMenu();
        void SetMainMenu(bool bShow);

///////////////////////////////////////////////////////////////////////////////////////
//
//
        bool LoadMod(const char * modpath)
        {
                memset(&ms, 0, sizeof(ms));
                strcpy(ms.modpath, modpath);
                strcpy(ms.modname, "测试MOD");
                strcpy(ms.menubgpic, "mainmenu.jpg");
                InitializeCriticalSection (&cs);

                rcStart.left = 100;
                rcStart.right = 700;
                rcStart.top = 350;
                rcStart.bottom = 400;
                rcQuit.left = 100;
                rcQuit.right = 700;
                rcQuit.top = 450;
                rcQuit.bottom = 500;
                
                bModLoaded = TRUE;
                return true;
        
        }
        

///////////////////////////////////////////////////////////////////////////////////////
//
//

        void ParseInput(INPUT_STRUCT& input)
        {
                static POINT    oldpoint;
                RECT            rect;
                POINT           point;

                if(!bModLoaded)
                {
                        return;
                }

                memcpy(&wi, &input, sizeof(wi));
                if(IsMouseInPanel(wi.x, wi.y))
                {
                        input.input = INPUT_NONE;
                }

                if(wi.input != INPUT_NONE)
                {
                        if(wi.key[VK_ESCAPE] == 1)
                        {
                                input.input = INPUT_NONE;
                        }
                
                        // ESC键
                        if(wi.key[VK_ESCAPE] == 1)
                        {
                                if(!IsMainMenu())
                                {
                                        ShowMainMenu(NULL);
                                }else{
                                        QuitMod();
                                }
                        }
                        if(IsMouseInPanel(wi.x, wi.y))
                        {
                                if(wi.key[VK_LBUTTON] == 1 || wi.key[VK_RBUTTON] == 1)
                                {
                                        MouseButtonDown();
                                }
                        }                 
                }

                //MouseMove(wi.x, wi.y);

                GetClientRect(m_hWnd, &rect);
                
                if(IsMainMenu())
                {
                        point.x = wi.x;
                        point.y = wi.y;
                        if( PtInRect(&rect, point) && (oldpoint.x != wi.x || oldpoint.y != wi.y) )
                        {
                                if(     PtInRect(&rcStart, point) != PtInRect(&rcStart, oldpoint) ||
                                        PtInRect(&rcQuit, point) != PtInRect(&rcQuit, oldpoint)
                                )
                                {
                                        InvalidateRect(m_hWnd, NULL, FALSE);
                                        oldpoint.x = wi.x;
                                        oldpoint.y = wi.y;
                                }
                        }
                        //PaintMainMenu();
                }
        }
        
        void MouseButtonDown()
        {
                //EnterCriticalSection(&cs);
                POINT   point;
                point.x = wi.x;
                point.y = wi.y;
                if(wi.key[VK_LBUTTON] == 1)
                {
                        if(PtInRect(&rcStart, point))
                        {
                                StartMod();
                        }
                        if(PtInRect(&rcQuit, point))
                        {
                                QuitMod();
                        }
                }
        }

        void MouseMove(int x, int y)
        {
        }

///////////////////////////////////////////////////////////////////////////////////////
//
//
        
        void StartMod()
        {
                CGame * game = CGame::Get();
                CRender * render = CRender::Get();
                SetMainMenu(false);
                render->Start();
                game->Run();
                
        }

        void QuitMod()
        {
                //DWORD   tid;
                //DWORD   pid;
                //tid = GetWindowThreadProcessId(m_hWnd, &pid);
                PostMessage(m_hWnd, WM_CLOSE, 0, 0);
                /*
                if(!PostThreadMessage(tid, WM_USER + 0x401, 0, 0))
                {
                        exit(1);
                }
                */
        }
        
///////////////////////////////////////////////////////////////////////////////////////
//
//

        // 第一次调用这个函数的时候设置窗口句柄..
        void ShowMainMenu(HWND hWnd)
        {
                CGame * game = CGame::Get();
                CRender * render = CRender::Get();

                if(hWnd != NULL)
                {
                        m_hWnd = hWnd;
                }
                if(m_hWnd == NULL)
                {
                        return;
                }
                
                game->Pause();
                render->Pause();
                SetMainMenu(true);
                
                InvalidateRect(m_hWnd, NULL, TRUE);
                
        }
        
        // 注意这个函数在另一条线程中执行..尽量不要访问wi.
        void PaintMainMenu()
        {
                RECT            rect;
                HDC             hdc;
                HDC             memDC;
                HDC             bmpDC;
                BMPINFOHEADER   biHeader;
                void *          buff;
                void *          buffer; 
                HBITMAP         hBitmap;
                HBITMAP         memBitmap;
                char            path[1024];
                POINT           point;
                HBRUSH          hBrush;
                //HPEN            hPen;
                HBRUSH          oldBrush;
                UINT            fmt;
                char            temp[100];

                if(!IsMainMenu() || m_hWnd == NULL)
                {
                        return;
                }
        
                GetCursorPos(&point);
                ScreenToClient (m_hWnd, &point);

                strcpy(path, "mainmenu.jpg");
                GetClientRect (m_hWnd, &rect);
                hdc     = GetDC(m_hWnd);
                memDC   = CreateCompatibleDC(hdc);
                bmpDC   = CreateCompatibleDC(hdc);
                buffer  = res::loadimg(path, &biHeader);
                hBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&biHeader,
                                                   DIB_RGB_COLORS, &buff,
                                                   NULL, 0);
                memBitmap = CreateCompatibleBitmap(hdc, rect.right-rect.left, rect.bottom-rect.top);
                
                memcpy(buff, buffer, biHeader.biWidth*biHeader.biHeight*biHeader.biBitCount/8);
                
                SelectObject(memDC, memBitmap);
                SelectObject(bmpDC, hBitmap);
                StretchBlt(memDC, 0, 0, 
                           rect.right-rect.left, rect.bottom-rect.top,
                           bmpDC, 0, biHeader.biHeight, biHeader.biWidth, -biHeader.biHeight,
                           SRCCOPY);
                
                
                
                if(PtInRect(&rcStart, point))
                {
                        hBrush = CreateSolidBrush(RGB(255, 255, 0));
                }else{
                        hBrush = CreateSolidBrush(RGB(128, 0, 255));
                }
                oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
                Rectangle(memDC, rcStart.left, rcStart.top, rcStart.right, rcStart.bottom);
                SelectObject(memDC, oldBrush);
                DeleteObject(hBrush);

                if(PtInRect(&rcQuit, point))
                {
                        hBrush = CreateSolidBrush(RGB(255, 255, 0));
                }else{
                        hBrush = CreateSolidBrush(RGB(128, 0, 255));
                }
                oldBrush = (HBRUSH)SelectObject(memDC, hBrush);
                Rectangle(memDC, rcQuit.left, rcQuit.top, rcQuit.right, rcQuit.bottom);
                SelectObject(memDC, oldBrush);
                DeleteObject(hBrush);
                
                SetTextCharacterExtra (memDC, 2);
                SetTextColor(memDC, RGB(255, 0, 0));

                SetBkMode (memDC, TRANSPARENT);
                fmt =  DT_CENTER | DT_VCENTER | DT_SINGLELINE;
                strcpy(temp, "开始游戏");
                DrawText(memDC, temp, strlen(temp), &rcStart, fmt);
                strcpy(temp, "结束游戏");
                DrawText(memDC, temp, strlen(temp), &rcQuit, fmt);

                BitBlt(hdc, 0, 0, rect.right-rect.left, rect.bottom-rect.top, memDC, 0, 0, SRCCOPY);
                DeleteObject(hBitmap);
                DeleteObject(memBitmap);
                DeleteDC(bmpDC);
                DeleteDC(memDC);
                ReleaseDC(m_hWnd, hdc);
                
        }
        
 ///////////////////////////////////////////////////////////////////////////////////////
//
//
       
        bool IsMouseInPanel(int x, int y)
        {
                
                if(IsMainMenu())
                {
                        return true;
                }
                
                return false;

        }

         bool IsMainMenu()
         {
                 bool res; 
                 EnterCriticalSection(&cs);
                        res = m_ShowMainMenu;
                 LeaveCriticalSection(&cs);
                 return res;
         }

         void SetMainMenu(bool bShow)
         {
                EnterCriticalSection(&cs);
                        m_ShowMainMenu = bShow;
                LeaveCriticalSection(&cs);
                 
         }

};
















