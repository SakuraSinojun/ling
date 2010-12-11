


#pragma once

#ifndef __MAP_H__
#define __MAP_H__

#include <windows.h>
#include <vector>


// v_trigger这个数组可以存放某个位置的触发器。
// 当然，，可以加变量以区分触发器的种类。
// 另一方面，，触发器没有必要放到地图类中。
// 设定好触发器所有的满足条件（进入区域，离开区域，跟NPC对话，杀死怪物等等等等)后，
// 可以专门做一个trigger管理类
typedef struct
{
	std::vector<void *>	v_trigger;
	char			bg_file		[MAX_PATH];
	int			x1;
	int			y1;
	int			x2;
	int			y2;
}MAPSQUARE;



// 单件CMap类。用CMap::GetMap()取得地图单件。
class CMap
{
protected:
	CMap();

public:
	~CMap();
	static CMap * GetMap();

public:

	void Create(int width, int height);
	void SetBackground(int x, int y, int x1, int y1, int x2, int y2, const char * filename);
	
	void AddTrigger(int x, int y, void * trigger_function);
	
	MAPSQUARE * GetSqr(int x, int y);

	int Width(){return m_width;}
	int Height(){return m_height;}

private:
	static CMap *	m_map;
	
	int		m_width;
	int		m_height;

	MAPSQUARE * m_square;

private:
	bool CheckPos(int x, int y);

};



#endif
