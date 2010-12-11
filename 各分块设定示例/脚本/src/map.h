


#pragma once

#ifndef __MAP_H__
#define __MAP_H__

#include <windows.h>
#include <vector>


// v_trigger���������Դ��ĳ��λ�õĴ�������
// ��Ȼ�������Լӱ��������ִ����������ࡣ
// ��һ���棬��������û�б�Ҫ�ŵ���ͼ���С�
// �趨�ô��������е��������������������뿪���򣬸�NPC�Ի���ɱ������ȵȵȵ�)��
// ����ר����һ��trigger������
typedef struct
{
	std::vector<void *>	v_trigger;
	char			bg_file		[MAX_PATH];
	int			x1;
	int			y1;
	int			x2;
	int			y2;
}MAPSQUARE;



// ����CMap�ࡣ��CMap::GetMap()ȡ�õ�ͼ������
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
