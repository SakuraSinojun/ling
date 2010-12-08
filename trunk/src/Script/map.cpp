
#include "../common/common.h"
#include "../LibGraphic/LibGraphic.h"
#include "bitmap.h"
#include "map.h"

#include <fstream>
#include <sstream>
#include <string>
using namespace std;


CMap * CMap::m_map = NULL;



CMap::CMap()
{
	m_square = NULL;
	m_width = 0;
	m_height = 0;
}

CMap::~CMap ()
{
	m_map = NULL;
        m_mainSur = NULL;
}

CMap * CMap::GetMap ()
{
	if(m_map == NULL)
	{
		m_map = new CMap();
	}
	return m_map;
}



bool CMap::Create(CSurface *sur, int width, int height, int cellSize)
{
	
	if(width<=0 || height<=0 || cellSize<=0)
	{
                common::Error("地图尺寸不能为负值");
		return false;
	}

	if (m_square != NULL)
	{
		free (m_square);
	}

        if(NULL == sur)
        {
                common::Error("加载兼容主画面失败");
                return false;
        }
	m_square = (MAPSQUARE *)malloc(sizeof(MAPSQUARE) * width * height);
	m_width = width;
	m_height = height;
        m_cellSize = cellSize;
        m_mainSur = sur;

        return true;

}

bool CMap::AddFrame(const CBitmap &bmp, int frameIndex, int x, int y, int mode)
{
        if(!bmp.IsLoaded())
        {
                common::Error("加载地图元素失败,位图没有被加载");
                return false;
        }

        CSurface *tmpSur = new CDxSurface;

        tmpSur->Create(m_mainSur, m_cellSize, m_cellSize);

 

        return true;
}

void CMap::SetBackground(int x, int y, int x1, int y1, int x2, int y2, const char * filename)
{
	int		offset;
	MAPSQUARE *	sqr;

	if(!CheckPos(x, y))
	{
		return;
	}

	offset = y * m_width + x;
	sqr = m_square + offset;

	strcpy(sqr->bg_file, filename);
	sqr->x1 = x1;
	sqr->y1 = y1;
	sqr->x2 = x2;
	sqr->y2 = y2;
}
	

void CMap::AddTrigger(int x, int y, void * trigger_function)
{
	int		offset;
	MAPSQUARE *	sqr;

	if(!CheckPos(x, y))
	{
		return;
	}
	offset = y * m_width + x;
	sqr = m_square + offset;
	
	if(trigger_function == NULL)
	{
		return;
	}

	sqr->v_trigger.push_back(trigger_function);


}
bool CMap::CheckPos(int x, int y)
{
	if(m_square == NULL)
	{
		return false;
	}

	if(x>m_width-1 || y>m_height-1)
	{
		return false;
	}
	
	if(x<0 || y<0)
	{
		return false;
	}
	return true;

}

MAPSQUARE * CMap::GetSqr(int x, int y)
{
	int		offset;

	if(!CheckPos(x, y))
	{
		return NULL;
	}
	offset = y * m_width + x;
	return m_square + offset;
}

bool CMap::WriteMap(const char *fileName)
{
        ofstream ofs(fileName);
        if(!ofs)
        {
                return false;
        }

        ofs<<"<SIZE>"<<endl;
        ofs<<m_width<<" "<<m_height<<" "<<m_cellSize<<endl;
        ofs<<"<RES>"<<endl;
        for(map<int, CBitmap>::iterator iter =0; iter != m_mapBmp.end(); iter++)
        {
                ofs<<"["<<iter->first<<"]"<<endl;
                ofs<<iter->second.GetBuffer()<<endl;
        }
        ofs<<"<MAPINFO>"<<endl;
        ofs<<m_mapInfo<<endl;
        ofs.close();

        return true;
}

