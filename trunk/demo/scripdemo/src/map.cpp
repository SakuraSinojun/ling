


#include "map.h"


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
}

CMap * CMap::GetMap ()
{
	if(m_map == NULL)
	{
		m_map = new CMap();
	}
	return m_map;
}



void CMap::Create(int width, int height)
{
	
	if(width<=0 || height<=0)
	{
		return;
	}

	if (m_square != NULL)
	{
		free (m_square);
	}
	m_square = (MAPSQUARE *)malloc(sizeof(MAPSQUARE) * width * height);
	m_width = width;
	m_height = height;

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


