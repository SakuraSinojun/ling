

/*


        Author:  SakuraSinojun

*/

#pragma once

#ifndef __INPUT_H__
#define __INPUT_H__


#include "../common/common.h"
#include <windows.H>

enum GAMEINPUT
{
        INPUT_NONE,
        INPUT_NEWINPUT,
};

typedef struct
{
        DWORD     input;
        int       x;
        int       y;
        int       key[256];
}INPUT_STRUCT;



namespace Input
{        
        void    ParseInput(INPUT_STRUCT& input);
        void    AtachInput(HWND hWnd);

        /*
        void    MouseLButtonDown(int x, int y);
        void    MouseRButtonDown(int x, int y);
        void    MouseWheelUp(int x, int y);
        void    MouseWheelDown(int x, int y);
        void    MouseMove(int x, int y);
        void    KeyDown(int key);
        void    KeyUp(int key);
        */
        
};


#endif


















