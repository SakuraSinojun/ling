#include "Game.h"

CGame::CGame()
{
        m_gameWnd = new CDxSurface;
        m_meinv = new CDxSurface;
}
CGame::~CGame()
{
        if(m_gameWnd)
        {
                m_gameWnd->ShutDown();
                delete m_gameWnd;
                m_gameWnd = NULL;
        }
}

bool CGame::InitGame(int width,int height, bool bWindow, HWND hWnd)
{
        srand(GetTickCount());

        if(!m_gameWnd->CreateMainWnd(width, height, bWindow, hWnd))
        {
                return false;
        }


        if(!m_gameWnd->LoadBitmap("..//res//bg1.bmp"))
        {
                return false;
        }

        return true;
}

bool CGame::GameLoop()
{
        _StartClock();

        RECT rc = {50, 50, 100, 100};

        m_gameWnd->DrawSolidRect(ARGB(0, 255, 0, 255), &rc);
        m_gameWnd->Flip();


        _WaitClock(33);

        return true;
}

bool CGame::ShutGame()
{
        return true;
}

void CGame::_StartClock()
{
	m_startClock = GetTickCount();
}

//-------------------------------------------------/

void CGame::_WaitClock(DWORD count)
{
	while((GetTickCount() - m_startClock) < count);
}