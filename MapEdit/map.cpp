
//#include "../common/common.h"
//#include "../LibGraphic/LibGraphic.h"

#include "map.h"

#include <fstream>
#include <sstream>
#include <string>
using namespace std;



bool CMap::SaveMapFile(const char *fileName, MAPINFO &mapInfo, SEGMENTINFO &segInfo)
{
       

        if(NULL == fileName)
        {
                return false;
        }

        ofstream ofs;
        ofs.open(fileName, ios::out|ios::binary);
        if(!ofs)
        {
                return false;
        }

        
        ofs.write(&MAPFILEFLAG, sizeof(char));

        ofs.write((char*)&mapInfo.head, sizeof(MAPHEAD));

        ofs.write((char*)mapInfo.pData, sizeof(DWORD)*mapInfo.head.width*mapInfo.head.width);

        int i=0;

        for(i=0; i< mapInfo.head.bmpNum; i++)
        {
                ofs.write((char*)&segInfo.heads[i], sizeof(SEGMENTHEAD));
                memset(segInfo.buffer[i], i, segInfo.heads[i].byteNum);
                ofs.write(segInfo.buffer[i], segInfo.heads[i].byteNum);
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

        ifs.read((char*)&mapInfo.head, sizeof(MAPHEAD));
        
        

        m_width  = mapInfo.head.width;
        m_height = mapInfo.head.height;
        m_cellSize = mapInfo.head.cellSize;
        m_bmpNum = mapInfo.head.bmpNum;

        if(m_pData)
        {
                delete[] m_pData;
        }

        m_pData = new DWORD[m_height*m_width];

        
        ifs.read((char*)m_pData, sizeof(DWORD) * m_width * m_height);

        this->ResetSegInfo(m_bmpNum);

        for(i=0; i<m_bmpNum; i++)
        {
                               
                ifs.read((char*)&m_segInfo.heads[i], sizeof(SEGMENTHEAD));
                m_segInfo.buffer[i] = new char[m_segInfo.heads[i].byteNum];
                ifs.read(m_segInfo.buffer[i], sizeof(m_segInfo.heads[i].byteNum));
        }
        
        ifs.close();
        
        return true;
}

void CMap::ResetSegInfo(int newNum)
{
        if(m_segInfo.buffer)
        {
                for(int i=0; i<m_segInfo.segNum; i++)
                {
                        if(m_segInfo.buffer[i])
                        {
                                delete[] m_segInfo.buffer[i];
                                m_segInfo.buffer[i] = NULL;
                        }
                }
                delete[] m_segInfo.buffer;
                m_segInfo.buffer = NULL;
        }

        if(m_segInfo.heads)
        {
                delete[] m_segInfo.heads;
                m_segInfo.heads = NULL;
        }

        m_segInfo.segNum = newNum;

        if(newNum>0)
        {

                m_segInfo.heads = new SEGMENTHEAD[newNum];
                m_segInfo.buffer = new char*[newNum];
        }
        
}
CMap * CMap::m_map = NULL;



CMap::CMap()
{
	m_square = NULL;
        m_pData = NULL;
        memset((char*)&m_segInfo, 0, sizeof(m_segInfo));
}

CMap::~CMap ()
{
        /*
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

  */
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
                //common::Error("地图尺寸不能为负值");
		return false;
	}

	if (m_square != NULL)
	{
		free (m_square);
	}

        if(NULL == sur)
        {
                //common::Error("加载兼容主画面失败");
                return false;
        }
	m_square = (MAPSQUARE *)malloc(sizeof(MAPSQUARE) * width * height);
	m_width = width;
	m_height = height;
        m_cellSize = cellSize;
        m_mainSur = sur;

        return true;

}

/*
bool CMap::AddFrame(const CBitmap &bmp, int frameIndex, int x, int y, int mode)
{
        if(!bmp.IsLoaded())
        {
                //common::Error("加载地图元素失败,位图没有被加载");
                return false;
        }

        CSurface *tmpSur = new CDxSurface;

        tmpSur->Create(m_mainSur, m_cellSize, m_cellSize);

 

        return true;
}

*/

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

/*
const BMPINFO *CMap::GetBitmap(int index)
{
        if(index >= m_bmpNum)
        {
                return NULL;
        }
        return &m_pBmp[index];
}

  */


