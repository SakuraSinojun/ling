


#pragma once

#ifndef __GAME_H__
#define __GAME_H__



class CGame
{
public:
        ~CGame();
        
        static CGame * Get();
        
        void    Run();
        void    Pause();
        void    Stop();

        bool    IsRunning();

private:
        CGame();
        static CGame * m_game;
        
};



#endif
