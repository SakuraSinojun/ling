


#pragma once

#ifndef __MAP_H__
#define __MAP_H__

#include <windows.h>
#include <vector>
#include <map>

#pragma warning (disable: 4514 4786)



// v_trigger这个数组可以存放某个位置的触发器。
// 当然，，可以加变量以区分触发器的种类。
// 另一方面，，触发器没有必要放到地图类中。
// 设定好触发器所有的满足条件（进入区域，离开区域，跟NPC对话，杀死怪物等等等等)后，
// 可以专门做一个trigger管理类

class CSurface;

typedef struct
{
	std::vector<void *>	v_trigger;
	char			bg_file		[MAX_PATH];
	int			x1;
	int			y1;
	int			x2;
	int			y2;
}MAPSQUARE;

static const char MAPFILEFLAG = '~';
    
#define MODE_CELL 1              
#define MODE_ABS  0

struct BMPHEAD
{
        int width;
        int height;
        int byteNum;
        int index;
        int bpp;
        char name[20];
};

struct BMPINFO
{
        BMPHEAD header;        
        char *buffer;
};

struct MAPHEAD
{
        int cellSize;           // 单元格大小       
        int width;              // 画面宽度
        int height;             // 画面高度
        int bmpNum;  
};

struct MAPINFO
{
        MAPHEAD header;
        int *pMap;

};

// 单件CMap类。用CMap::GetMap()取得地图单件。
class CMap
{
              
public:
         CMap();
	~CMap();
	static CMap * GetMap();

public:

	bool Create(CSurface *mainSur, int width, int height, int cellSize);
	void SetBackground(int x, int y, int x1, int y1, int x2, int y2, const char * filename);
        
        bool AddFrame(const CBitmap &bmp, int frameIndex, int x, int y, int mode);
	
	void AddTrigger(int x, int y, void * trigger_function);
	
	MAPSQUARE * GetSqr(int x, int y);

	int Width(){return m_width;}
	int Height(){return m_height;}
        int BitmapNum(){return m_bmpNum;}

        const BMPINFO *GetBitmap(int index);

        bool SaveMapFile(const char *fileName, int mapWidth, int mapHeight, int cellSize,
                   int *pMap, BMPINFO *bmp, int bmpNum); 

        bool LoadMapFile(const char *fileName);

       

private:
	static CMap *	m_map;
	
	int		m_width;
	int		m_height;
        int             m_cellSize;

	MAPSQUARE * m_square;

        CSurface *m_mainSur;
        CSurface *m_surMap;

        BMPINFO *m_pBmp;

        int m_bmpNum;

        int *m_pInfo;

private:
	bool CheckPos(int x, int y);

};



#endif
