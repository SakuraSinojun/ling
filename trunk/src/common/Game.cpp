#include "Game.h"
#include <iostream>
using namespace std;

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

           

        if(!m_meinv->Create(320, 200))
        {
                return false;
        }

        if(!m_meinv->LoadBitmap("..//res//Quensp5_24.bmp"))
        {
                return false;
        }

        m_count = 0;

        m_start = GetTickCount();

        return true;
}

bool CGame::GameLoop()
{
        _StartClock();

        RECT rc = {50, 50, 100, 100};

        m_gameWnd->DrawSolidRect(ARGB(0, 0, 255, 0), NULL);

        m_gameWnd->AttachSurface(m_meinv, 0, 0);
        m_gameWnd->AttachSurface(m_meinv, 50, 0);
        m_gameWnd->AttachSurface(m_meinv, 100, 0);
        m_gameWnd->AttachSurface(m_meinv, 150, 0);
        m_gameWnd->AttachSurface(m_meinv, 200, 0);
        m_gameWnd->AttachSurface(m_meinv, 250, 0);
        m_gameWnd->AttachSurface(m_meinv, 300, 0);

        //m_gameWnd->DrawSolidRect(ARGB(0, 255, 0, 255), &rc);



        m_gameWnd->Flip();

        m_count++;


        if(GetTickCount() - m_start >= 1000)
        {
                cout<<m_count<<endl;
                m_count = 0;
                m_start = GetTickCount();
        }
        //_WaitClock(33);

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