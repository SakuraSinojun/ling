


#include "game.h"
#include "../common/common.h"
#include "input.h"
#include "Mod.h"


#include <stdio.h>
#include <windows.h>

typedef enum GAME_STATE
{
        ST_INIT = 0,
        ST_RUN,
        ST_PAUSE,
        ST_STOP,
}GAME_STATE;

CGame *                  CGame::m_game = NULL;
static HANDLE            hThread = NULL;
static GAME_STATE        state = ST_INIT;
static DWORD	         lpThreadId = 0; 
static CRITICAL_SECTION  cs;



DWORD WINAPI _game_thread(LPVOID lpParameter)
{
        GAME_STATE	st;
        INPUT_STRUCT	input;

        while(TRUE)
        {
                EnterCriticalSection(&cs);
                        st = state;
                LeaveCriticalSection(&cs);
                
                Input::ParseInput(input);
                Mod::ParseInput (input);
                //CHero::ParseInput(input, arg1, arg2, arg3, arg4);
                        
                switch(st)
                {
                case ST_RUN:
                       
                        //CSpell::CalcSpellFrame();	// 法术漂来漂去。。。
                        //CNpc::WanderAbout();		// NPC走来走去。。。
                        //CMonster::WanderAbout();	// 怪物也走来走去。。。
                        //CPet::WanderAbout();		// 如果有宝宝。它也走来走去。。
                        Sleep(0);
                        break;
                case ST_PAUSE:
                        Sleep(1);
                        break;
                case ST_STOP:
                        hThread = NULL;
                        return 0;
                default:
                        Sleep(500);
                        break;
                }
        }

        return 0;
}


CGame::CGame()
{
        SECURITY_ATTRIBUTES     sa;

        if(hThread != NULL)
        {
                common::Error("线程已经建立。单件类被多次构造？");
                return;
        }
        
        state = ST_INIT;
        InitializeCriticalSection(&cs);
        
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = 0;
        
        
        hThread = CreateThread(&sa, 0, _game_thread, NULL, 0, &lpThreadId);
        
        if(hThread == NULL)
        {
                common::Error("不能创建游戏线程。");
                exit(2);
        }
        
}

CGame::~CGame()
{
        m_game = NULL;
}

CGame * CGame::Get()
{
        if(m_game == NULL)
        {
                m_game = new CGame();
        }
        return m_game;
        
}

void CGame::Run()
{
        EnterCriticalSection(&cs);
                state = ST_RUN;
        LeaveCriticalSection(&cs);
        printf("running game...\n");
        
}

void CGame::Pause()
{
        EnterCriticalSection(&cs);
                state = ST_PAUSE;
        LeaveCriticalSection(&cs);
        printf("pause game...\n");
        
}

void CGame::Stop()
{
        if(hThread == NULL)
        {
                printf("thread not started...\n");
                return;
        }
        
        EnterCriticalSection(&cs);
                state = ST_STOP;
        LeaveCriticalSection(&cs);
        printf("stopping game...\n");
        
        if(WaitForSingleObject(hThread, 5000) == WAIT_TIMEOUT)
        {
                printf("game thread timeout...\n");
        }
        
}

bool CGame::IsRunning()
{
        int run;
        EnterCriticalSection(&cs);
                run = state;
        LeaveCriticalSection(&cs);
        if(run == ST_RUN)
        {
                return true;
        }else{
                return false;
        }
}





























