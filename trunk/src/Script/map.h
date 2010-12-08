


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
class CLBitmap;

typedef struct
{
	std::vector<void *>	v_trigger;
	char			bg_file		[MAX_PATH];
	int			x1;
	int			y1;
	int			x2;
	int			y2;
}MAPSQUARE;

#define MODE_CELL 1              
#define MODE_ABS  0

// ����CMap�ࡣ��CMap::GetMap()ȡ�õ�ͼ������
class CMap
{
protected:
	CMap();

public:
	~CMap();
	static CMap * GetMap();

public:

	bool Create(CSurface *mainSur, int width, int height, int cellSize);
	void SetBackground(int x, int y, int x1, int y1, int x2, int y2, const char * filename);
        
        bool AddFrame(const CBitmap &bmp, int frameIndex, int x, int y, int mode);
	
	void AddTrigger(int x, int y, void * trigger_function);

        bool LoadMap(const char *fileName);
        bool WriteMap(const char *fileName);
	
	MAPSQUARE * GetSqr(int x, int y);

	int Width(){return m_width;}
	int Height(){return m_height;}

private:
	static CMap *	m_map;
	
	int		m_width;
	int		m_height;
        int             m_cellSize;

	MAPSQUARE * m_square;

        CSurface *m_mainSur;

        char *m_mapInfo;
        std::vector<CSurface*> m_vSur;
        std::map<int, CBitmap> m_mapBmp;

private:
	bool CheckPos(int x, int y);

};



#endif
