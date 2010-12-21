


#pragma once

#ifndef __GAMECONFIG_H__
#define __GAMECONFIG_H__



class CGameConfig
{

public:
        static CGameConfig * Get();
        
        ~CGameConfig();
        
        int WindowWidth();
        int WindowHeight();
        bool IsFullScreen();
        char * GetRender();

private:
        CGameConfig();
        static CGameConfig * m_config;
        
        int width;
        int height;
        int fullscreen;
        char render[100];
};


#endif






