


#pragma once

#ifndef __MAP_H__
#define __MAP_H__

#include <windows.h>
#include <vector>
#include <map>

#pragma warning (disable: 4514 4786)



// v_trigger���������Դ��ĳ��λ�õĴ�������
// ��Ȼ�������Լӱ��������ִ����������ࡣ
// ��һ���棬��������û�б�Ҫ�ŵ���ͼ���С�
// �趨�ô��������е��������������������뿪���򣬸�NPC�Ի���ɱ������ȵȵȵ�)��
// ����ר����һ��trigger������

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
    
static DWORD MAP_MASK = 0xffff;
#define MODE_CELL 1              
#define MODE_ABS  0



struct SEGMENTHEAD
{
        DWORD width;
        DWORD height;
        DWORD byteNum;
        DWORD index;
        WORD bpp;
        char name[48];
};

struct MAPHEAD
{
        WORD  cellSize;           // ��Ԫ���С 
        WORD  bmpNum; 
        WORD  width;              // ������
        WORD  height;             // ����߶�    
};

struct SEGMENTINFO
{
        int segNum;
        SEGMENTHEAD *heads;
        char **buffer;
        
};

struct MAPINFO
{
        MAPHEAD head;
        DWORD *pData;    
};




// ����CMap�ࡣ��CMap::GetMap()ȡ�õ�ͼ������
class CMap
{
              
public:
         CMap();
	~CMap();
	static CMap * GetMap();

public:

	bool Create(CSurface *mainSur, int width, int height, int cellSize);
	void SetBackground(int x, int y, int x1, int y1, int x2, int y2, const char * filename);
        
        //bool AddFrame(const CBitmap &bmp, int frameIndex, int x, int y, int mode);
	
	void AddTrigger(int x, int y, void * trigger_function);
	
	MAPSQUARE * GetSqr(int x, int y);

	int Width(){return m_width;}
	int Height(){return m_height;}
        int Size(){return m_cellSize;}
        int BitmapNum(){return m_bmpNum;}

        const SEGMENTINFO &GetSegInfo(){return m_segInfo;};

        
        bool SaveMapFile(const char *fileName, MAPINFO &mapInfo, SEGMENTINFO &segInfo); 

        bool LoadMapFile(const char *fileName);
        
        void ResetSegInfo(int newNum);
        
        DWORD *GetMapData(DWORD* &data);
        

       

private:
	static CMap *	m_map;
	
	int		m_width;
	int		m_height;
        int             m_cellSize;

	MAPSQUARE * m_square;

        CSurface *m_mainSur;
        CSurface *m_surMap;

        SEGMENTINFO m_segInfo;

        int m_bmpNum;

        DWORD *m_pData;

private:
	bool CheckPos(int x, int y);

};



#endif
