

#include "common.h"
#include "game.h"
#include "script.h"
#include "render.h"
#include "map.h"

#include <memory.h>


static bool	key_state[256];


static bool _game_init(void);


bool game_loop(HWND hWnd)
{
	static bool	firstRun = true;
	
	if(firstRun)
	{
		if(!_game_init())
		{
			return false;
		}
		firstRun = false;
	}
	
	

	render(hWnd);
	return true;
}

void game_input(KEYSTATE state, int key)
{
	
	switch(state)
	{
	case KEY_DOWN:
		key_state[key] = true;
		break;
	case KEY_UP:
		key_state[key] = false;
		break;
	default:
		break;
	}
	
}


static bool _game_init(void)
{
	memset(key_state, 0, 256* sizeof(bool));
	if (!script_init("demo.dll"))
	{
		common::Error ("脚本加载失败。");
		return false;
	}
	return true;
	
}






////////////////////////////////////////////////////////////////////////////////////////////////
//

// 重定向
void SCRIPT_EXPORT map_create(int width, int height)
{
	CMap * map = CMap::GetMap ();
	map->Create(width, height);

}

void SCRIPT_EXPORT map_set_background(int x, int y, int x1, int y1, int x2, int y2, const char * element)
{
	CMap * map = CMap::GetMap();
	map->SetBackground (x, y, x1, y1, x2, y2, element);

}

void SCRIPT_EXPORT map_add_trigger(int x, int y, void * trigger_function)
{
	CMap * map = CMap::GetMap();
	map->AddTrigger (x, y, trigger_function);
}














