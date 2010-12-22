
/*******************************************************************************

        Author: ׼
        Tab:    8


        log:    ��ͣ��eventһֱ���ܳɹ�..��CriticalSection+״̬����
                �����.
        
*******************************************************************************/

#include "glrender.h"

#include <stdio.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>


typedef struct GAME_OBJECT
{
        int     obj_id;

}GAME_OBJECT;


typedef struct OBJECTMAP
{
        void *  object;
}OBJECTMAP, *POBJECTMAP;

typedef enum GAME_STATE
{
	ST_INIT = 0,
	ST_RUN,
	ST_PAUSE,
	ST_STOP,
}GAME_STATE;

static HWND     m_hWnd = NULL;
static HDC      m_hDC = NULL;
static HGLRC    m_hRC = NULL;
static HANDLE   hThread = NULL;
static DWORD    lpThreadId = 0; 
static HANDLE   hEvent = NULL;
static BOOL     bObjectChanged = TRUE;

static GAME_STATE               m_State = ST_INIT;
static CRITICAL_SECTION         cs;
static CRITICAL_SECTION         cs_changed;
//static POBJECTMAP       *       object_map;

///////////////////////////////////////////////////////////////////////////

DWORD WINAPI _render_thread(LPVOID lpParameter);
void RenderScene(void);
void SetupRC(void);
void ChangeSize(GLsizei w, GLsizei h);


///////////////////////////////////////////////////////////////////////////


BOOL _is_changed()
{
        BOOL res ;
        EnterCriticalSection(&cs_changed);
        {
                res = bObjectChanged;
        }
        LeaveCriticalSection(&cs_changed);
        return res;
}

void _set_change(BOOL bChange)
{
        EnterCriticalSection(&cs_changed);
        {
                bObjectChanged = bChange;
        }
        LeaveCriticalSection(&cs_changed);
}


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
		sprintf(FPS,"FPS: %.2f֡/��", fps);
		SetWindowText(m_hWnd, FPS);
	}
	
}

DWORD WINAPI _render_thread(LPVOID lpParameter)
{
	GAME_STATE      s;
	RECT	        orc;
	RECT	        rect;
	BOOL            flag;

	memset(&orc, 0, sizeof(RECT));
	
	/////////////////////////////////////////////////////
	//      ����������ܷ������߳������
	//
	m_hRC = wglCreateContext(m_hDC);
	if(m_hRC == NULL)
	{
		_render_error("���ܴ���OpenGL��Ⱦ��������");
		return FALSE;
	}
	if(!wglMakeCurrent(m_hDC, m_hRC))
	{
		_render_error("���ܼ��ǰOpenGL��Ⱦ������.");
		return FALSE;
	}

	SetupRC();
	
	while(TRUE)
	{
		
                EnterCriticalSection(&cs);
			s = m_State;
                        if(m_State == ST_PAUSE)
                        {
                                m_State = ST_INIT;
                        }
		LeaveCriticalSection(&cs);

		switch (s)
		{
		case ST_RUN:
			if(!IsWindow(m_hWnd))
			{
				return 1;
			}
			if(!IsWindowVisible(m_hWnd))
			{
				Sleep(100);
				break;
			}
                        _render_calcFPS();

                        GetClientRect(m_hWnd, &rect);
			if(rect.right-rect.left != orc.right-orc.left || rect.bottom-rect.top != orc.bottom-orc.top)
			{
				ChangeSize(rect.right-rect.left, rect.bottom-rect.top);
				orc = rect;
			}

                        if(!_is_changed())
                        {
                                Sleep(100);
                                continue;
                        }else{
                                _set_change(TRUE);
                                RenderScene();
                        }
			//Rectangle(m_hDC, 0, 0, 100, 100);
			Sleep(0);
			break;
		case ST_STOP:
                        flag = TRUE;
	                if(!wglMakeCurrent(m_hDC, NULL))
	                {
		                _render_error("�ͷ�DC��RCʧ�ܣ�");
		                flag = FALSE;
	                }
	                
	                if(!wglDeleteContext(m_hRC))
	                {
		                _render_error("�ͷ�RCʧ��!");
		                flag = FALSE;
	                }
	                m_hRC = NULL;

	                if(!ReleaseDC(m_hWnd, m_hDC))
	                {
		                _render_error("�ͷ�DCʧ��!");
		                flag = FALSE;
	                }
	                m_hDC = NULL;

	               if(flag)
                       {
                               return 0;
                       }else{
                               return 1;
                       }
                case ST_PAUSE:
                        Sleep(100);
                        SetEvent(hEvent);
                        //PulseEvent(hEvent);
                        Sleep(100);
                        break;
                case ST_INIT:
		default:
			Sleep(100);
			break;
		}
	}
	return 0;
}

BOOL _render_create_thread()
{
	SECURITY_ATTRIBUTES	sa;
	//static HANDLE	hThread;
	//static LPDWORD	lpThreadId;
	
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = 0;

	if(hThread != NULL)
	{
		_render_error("�߳��Ѿ����ڡ�");
		return FALSE;
	}

	InitializeCriticalSection(&cs);
        InitializeCriticalSection(&cs_changed);

        hEvent = CreateEvent(&sa, TRUE, FALSE, "PauseEvent");

	hThread = CreateThread(&sa, 0, 
			_render_thread, NULL, 
			0, &lpThreadId);
	
	if(hThread == NULL)
	{
		_render_error("���ܴ�����Ⱦ�̡߳�");
		return FALSE;
	}

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
	PIXELFORMATDESCRIPTOR	pfd;
	GLuint			uPixelFormat;

	if(hWnd == NULL)
	{
		_render_error("��Ч�Ĵ��ھ����");
		return FALSE;
	}
	if(m_hWnd != NULL)
	{
		_render_error("�Ѿ�Attach��һ�Ρ�");
		return FALSE;
	}

	memset(&pfd, 0, sizeof(pfd));	
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	
	m_hWnd = hWnd;
	m_hDC  = GetDC(hWnd);
	
	uPixelFormat = ChoosePixelFormat (m_hDC, &pfd);
	if(uPixelFormat == 0)
	{
		_render_error("�����������ظ�ʽ��");
		return FALSE;
	}

	if(!SetPixelFormat(m_hDC, uPixelFormat, &pfd))
	{
		_render_error("�����������ظ�ʽ��");
		return FALSE;
	}
	

	return _render_init();


}


BOOL render_pause()
{
        printf("pausing render\n");
        
        ResetEvent(hEvent);
	EnterCriticalSection(&cs);
		m_State = ST_PAUSE;
	LeaveCriticalSection(&cs);
	if(WaitForSingleObject(hEvent, 3000) == WAIT_TIMEOUT)
        {
                printf("��ͣ��ʱ,������Event����������.\n");
        }

        ResetEvent(hEvent);

        printf("paused.\n");

	return TRUE;

}

BOOL render_start()
{
	EnterCriticalSection(&cs);
		m_State = ST_RUN;
	LeaveCriticalSection(&cs);
	printf("resume render\n");

	return TRUE;
}

BOOL render_stop()
{
        if(hThread == NULL)
        {
                return TRUE;
        }

        printf("stoping render\n");
        
	EnterCriticalSection(&cs);
		m_State = ST_STOP;
	LeaveCriticalSection(&cs);
	
        if(WaitForSingleObject (hThread, 5000) == WAIT_TIMEOUT)
        {
                _render_error("�̳߳�ʱ��ǿ�ƽ�����");
                TerminateProcess (hThread, 1);
        }
        
        CloseHandle(hEvent);
	
        printf("Stopped.\n");
        return TRUE;
}


void _render_render(void)
{
        _set_change(TRUE);
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



///////////////////////////////////////////////////////////////////////////


#define GL_PI 3.14159265359
static GLdouble	eye_vangle;
static GLdouble	eye_x;
static GLdouble	eye_y;
static GLdouble	eye_z;
static GLdouble	eye_hangle;



void RenderScene(void)
{
	GLfloat dist = 100;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	
	glPushMatrix();
		
		gluLookAt(eye_x, eye_y, eye_z, 
				eye_x + dist * cos(eye_vangle) * cos(eye_hangle),
				eye_y + dist * sin(eye_hangle),
				eye_z + dist * sin(eye_vangle) * cos(eye_hangle),
				cos(eye_vangle) * cos(eye_hangle + GL_PI / 2.0),
				sin(eye_hangle + GL_PI / 2.0),
				sin(eye_vangle) * cos(eye_hangle + GL_PI / 2.0));

		

	glPopMatrix();
	//glAccum(GL_RETURN, 1.0f);
	//glFlush();
	SwapBuffers(m_hDC);

}



void SetupRC(void)
{


	glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
	
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_FILL);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	
	
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);

}

void ChangeSize(GLsizei w, GLsizei h)
{
	
	printf("������С��%d, %d\n", w, h);

	if(h==0)
	{
		h=1;
	}
	
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 1.0, 400.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

 