
/*



*/

#include "../common/common.h"
#include "gameconfig.h"
#include <windows.h>


CGameConfig * CGameConfig::m_config = NULL;


CGameConfig::CGameConfig()
{
        char inifile[1024];
        char temp[2048];

        strcpy(inifile, GetGamePath());
        strcat(inifile, "\\ling.ini");
        
        GetPrivateProfileString("game", "width", "800", temp, 2048, inifile);
        width = atoi(temp);
        GetPrivateProfileString("game", "height", "600", temp, 2048, inifile);
        height = atoi(temp);
        GetPrivateProfileString("game", "fullscreen", "0", temp, 2048, inifile);
        fullscreen = atoi(temp);
        GetPrivateProfileString("game", "render", "none", render, 100, inifile);

}

CGameConfig::~CGameConfig()
{
        m_config = NULL;
}

CGameConfig * CGameConfig::Get()
{
        if(m_config == NULL)
        {
                m_config = new CGameConfig();
        }
        return m_config;
}

int CGameConfig::WindowWidth()
{
        return width;
}

int CGameConfig::WindowHeight()
{
        return height;
}

bool CGameConfig::IsFullScreen()
{
        return (fullscreen!=0);
}

char * CGameConfig::GetRender ()
{
        return render;
}
































