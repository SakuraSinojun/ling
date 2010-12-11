

/*


        Author:  SakuraSinojun

*/

#pragma once

#ifndef __INPUT_H__
#define __INPUT_H__


#include "../common/common.h"

enum INPUT
{
        INPUT_NONE,

};

typedef struct
{
        DWORD     input;
        DWORD     arg1;
        DWORD     arg2;
        DWORD     arg3;
        DWORD     arg4;
}INPUT_STRUCT;



namespace Input
{        
        void    ParseInput(INPUT_STRUCT& input);
        

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


















