
#include "../common/common.h"
#include "../LibGraphic/LibGraphic.h"
#include "bitmap.h"
#include "map.h"

#include <fstream>
#include <sstream>
#include <string>
using namespace std;


bool CMap::SaveMapFile(const char *fileName, int mapWidth, int mapHeight, int cellSize,
                   int *pMap, BMPINFO *bmp, int bmpNum)
{
        if(NULL == pMap || NULL == bmp)
        {
                return false;
        }
        MAPINFO mapInfo;

        mapInfo.header.bmpNum = bmpNum;
        mapInfo.header.cellSize = cellSize;
        mapInfo.header.height = mapHeight;
        mapInfo.header.width = mapHeight;
        mapInfo.pMap = pMap;
        ofstream ofs;
        ofs.open(fileName, ios::out|ios::binary);
        if(!ofs)
        {
                return false;
        }
        ofs.write((char*)&MAPFILEFLAG, sizeof(char));

        ofs.write((char*)&mapInfo.header, sizeof(mapInfo.header));
        ofs.write((char*)mapInfo.pMap, mapWidth * mapHeight * sizeof(int));
        for(int i=0; i<bmpNum; i++)
        {
                ofs.write((char*)&bmp[i].header, sizeof(bmp[i].header));
                ofs.write((char*)bmp[i].buffer, sizeof(bmp[i].header.byteNum));
        }

        ofs.close();
        return true;
}

bool CMap::LoadMapFile(const char *fileName)
{
        if(NULL == fileName)
        {
                return false;
        }

        ifstream ifs;
        ifs.open(fileName, ios::in|ios::binary);
        if(!ifs)
        {
                return false;
        }

        char flag=0;
        int i=0;

        ifs.read(&flag, sizeof(char));

        if(flag != MAPFILEFLAG)
        {
                return false;
        }

        MAPINFO mapInfo;

        ifs.read((char*)&mapInfo.header, sizeof(mapInfo.header));

        m_width = mapInfo.header.width;
        m_height = mapInfo.header.height;
        m_cellSize = mapInfo.header.cellSize;
        m_bmpNum = mapInfo.header.bmpNum;

        if(m_pInfo)
        {
                delete[] m_pInfo;
        }
        m_pInfo = new int[m_width * m_height];
        ifs.read((char*)m_pInfo, sizeof(int) * m_width * m_height);

        if(m_pBmp)
        {
                for(i=0; i<m_bmpNum; i++)
                {
                        if(m_pBmp[i].buffer)
                        {
                                delete[] m_pBmp[i].buffer;
                        }
                }
                
                delete[] m_pBmp;
        }
        m_pBmp = new BMPINFO[mapInfo.header.bmpNum];
        for(i=0; i<m_bmpNum; i++)
        {
                               
                ifs.read((char*)&m_pBmp[i].header, sizeof(m_pBmp[i].header));
                m_pBmp[i].buffer = new char[m_pBmp[i].header.byteNum];
                ifs.read((char*)m_pBmp[i].buffer, sizeof(m_pBmp[i].header.byteNum));
        }
        

        ifs.close();
        return true;
}
CMap * CMap::m_map = NULL;



CMap::CMap()
{
	m_square = NULL;
        memset((char*)&m_pInfo, 0, sizeof(m_pInfo));
        m_pBmp = NULL;
}

CMap::~CMap ()
{
        if(m_pBmp)
        {
                for(int i=0; i<m_bmpNum; i++)
                {
                        if(m_pBmp[i].buffer)
                        {
                                delete[] m_pBmp[i].buffer;
                                m_pBmp[i].buffer = NULL;
                        }
                }
                delete[] m_pBmp;
                m_pBmp = NULL;
        }
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

const BMPINFO *CMap::GetBitmap(int index)
{
        if(index >= m_bmpNum)
        {
                return NULL;
        }
        return &m_pBmp[index];
}


