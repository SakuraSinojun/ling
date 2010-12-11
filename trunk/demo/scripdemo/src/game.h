

#pragma once

#ifndef __GAME_H__
#define __GAME_H__

#include <windows.h>

typedef enum
{
	KEY_DOWN,
	KEY_UP,
	MOUSE_DOWN,
	MOUSE_UP,
}KEYSTATE;

bool game_loop(HWND hWnd);
void game_input(KEYSTATE state, int key);




// 导出给脚本的函数，无论是用__stdcall还是用__cdecl，只要与DLL保持一致就可以。
// 脚本中做好一些预定工作，对于最终建立模组的情况，这几个函数就像透明一样。
#define SCRIPT_EXPORT	__cdecl
void SCRIPT_EXPORT map_create(int width, int height);
void SCRIPT_EXPORT map_set_background(int x, int y, int x1, int y1, int x2, int y2, const char * element);
void SCRIPT_EXPORT map_add_trigger(int x, int y, void * trigger_function);


#endif

