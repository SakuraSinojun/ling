#ifndef GAME_H_101205
#define GAME_H_101205

#include "../LibGraphics/LibGraphics.h"

class CGame
{
public:
        CGame();
        ~CGame();

        bool InitGame(int width,int height, bool bWindow = true, HWND = NULL);
        bool GameLoop();
        bool ShutGame();
protected:
        void _StartClock();
        void _WaitClock(DWORD count);

        DWORD m_startClock;

        CSurface *m_gameWnd;
        CSurface *m_meinv;

};
#endif