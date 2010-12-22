
/*******************************************************************************

        Author: 
        Tab:   
        
*******************************************************************************/

#include "dxrender.h"
#include <windows.h>
#include <stdio.h>

typedef enum GAME_STATE
{
	ST_INIT = 0,
	ST_RUN,
	ST_PAUSE,
	ST_STOP,
}GAME_STATE;

static GAME_STATE               m_State = ST_INIT;


// 注意这个错误记录不要用common::Error，否则可能会有线程冲突问题。
void _render_error(const char * string, ...)
{
	FILE    *fp;
	char    buffer[1024];
        va_list arglist;

        if (NULL == string)
        {
                return;
        }
                                        
        va_start(arglist, string);
        vsprintf(buffer, string, arglist);
        va_end(arglist);

        fp = fopen("rendererror.log", "a");
	fwrite(buffer, strlen(buffer), 1, fp);
	fwrite("\n", 1, 1, fp);
	fclose(fp);
	printf(buffer); 

}

void _render_calcFPS()
{		
	LARGE_INTEGER           litmp;
	LONGLONG                qt1;
	static LONGLONG         qt2 = 0;
	double                  dft;
	double                  dff;
	double                  dfm;
	char                    FPS[100];
	static LONGLONG         framecount=0;
	double                  fps;
	
	QueryPerformanceFrequency(&litmp);
	dff = (double)litmp.QuadPart;
	
	QueryPerformanceCounter(&litmp);
	qt1 = litmp.QuadPart;
	
	
	framecount++;
	
	if(qt2 == 0)
	{
		qt2 = qt1;
		return;
	}
	
	dfm = (double)(qt1 - qt2);
	dft = dfm / dff;
	
	if(dft > 1)
	{
		fps = framecount / dft;
		framecount = 0;
		qt2 = qt1;
		sprintf(FPS,"FPS: %.2f帧/秒", fps);
		//SetWindowText(m_hWnd, FPS);
	}
	
}

DWORD WINAPI _render_thread(LPVOID lpParameter)
{
	return 0;
}

BOOL _render_create_thread()
{
	return TRUE;
}


BOOL _render_init()
{
	m_State = ST_INIT;
	return _render_create_thread();
}



///////////////////////////////////////////////////////////////////////////

BOOL render_attach(HWND hWnd)
{
	return _render_init();
}


BOOL render_pause()
{
	return TRUE;
}

BOOL render_start()
{
	return TRUE;
}

BOOL render_stop()
{
        return TRUE;
}


void _render_render(void)
{

}

int render_create_object(void* buffer, int len, int type)
{
	return 0;
}

int render_create_panel(void * buffer, int len, int type)
{
	return 0;
}

int render_create_map(void * buffer, int len, int type)
{
	return 0;
}

int render_add_data(int object, void * buffer, int len, int type)
{
	return 0;
}

int render_switch_data(int index)
{
	return 0;
}

int render_move_2i(int object, int x, int z)
{
	return 0;
}

int render_move_2f(int object, float x, float z)
{
	return 0;
}

int render_move_3i(int object, int x, int y, int z)
{
	return 0;
}

int render_move_3f(int object, float x, float y, float z)
{
	return 0;
}

int render_rotate_1d(int object, double angle)
{
	return 0;
}

int render_rotate_3d(int object, double angle, double x, double y, double z)
{
	return 0;
}

int render_scale(int object, double scale)
{
	return 0;
}

int render_delete_object(int object)
{
	return 0;
}

int render_move_camera_2d(double x, double z)
{
	return 0;
}

int render_move_camera_3d(double x, double y, double z)
{
	return 0;
}




 