


#pragma once

#ifndef __MOD_H__
#define __MOD_H__


#include "input.h"



namespace Mod
{

        
        bool LoadMod(const char * modpath);
        void ParseInput(INPUT_STRUCT& input);
        
        
        void ShowMainMenu(HWND hWnd);
        void PaintMainMenu();
        

};



#endif

