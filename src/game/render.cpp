
/*******************************************************************************

        Ahthor: 樱凌准
        Tab:    8
        
*******************************************************************************/

#include "render.h"
#include <windows.h>
#include <string.h>
#include <memory.h>


#define RENDER_API	__cdecl

typedef BOOL (RENDER_API * LPFNRENDERATTACH)(HWND hWnd);
typedef BOOL (RENDER_API * LPFNRENDERDETACH)(void);
typedef BOOL (RENDER_API * LPFNRENDERPAUSE)(void);
typedef BOOL (RENDER_API * LPFNRENDERSTART)(void);
typedef BOOL (RENDER_API * LPFNRENDERSTOP)(void);
typedef BOOL (RENDER_API * LPFNRENDERRENDERSCENE)(void);
typedef int (RENDER_API * LPFNRENDERCREATEOBJECT)(void* buffer, int len, int type);
typedef int (RENDER_API * LPFNRENDERCREATEPANEL)(void * buffer, int len, int type);
typedef int (RENDER_API * LPFNRENDERCREATEMAP)(void * buffer, int len, int type);
typedef int (RENDER_API * LPFNRENDERADDDATA)(int object, void * buffer, int len, int type);
typedef int (RENDER_API * LPFNRENDERSWITCHDATA)(int index);
typedef int (RENDER_API * LPFNRENDERMOVE2I)(int object, int x, int z);
typedef int (RENDER_API * LPFNRENDERMOVE2F)(int object, float x, float z);
typedef int (RENDER_API * LPFNRENDERMOVE3I)(int object, int x, int y, int z);
typedef int (RENDER_API * LPFNRENDERMOVE3F)(int object, float x, float y, float z);
typedef int (RENDER_API * LPFNRENDERROTATE1D)(int object, double angle);
typedef int (RENDER_API * LPFNRENDERROTATE3D)(int object, double angle, double x, double y, double z);
typedef int (RENDER_API * LPFNRENDERSCALE)(int object, double scale);
typedef int (RENDER_API * LPFNRENDERDELETEOBJECT)(int object);
typedef int (RENDER_API * LPFNRENDERMOVECAMERA2D)(double x, double z);
typedef int (RENDER_API * LPFNRENDERMOVECAMERA3D)(double x, double y, double z);







CRender * CRender::m_render = NULL;


CRender::CRender()
{
	hModule = NULL;
}
	

CRender::~CRender()
{
	m_render = NULL;
}
	
CRender * CRender::GetRender(void)
{
	if(m_render == NULL)
	{
		m_render = new CRender();
	}
	return m_render;
}
	
BOOL CRender::InitRender(const char * strRenderDLLName)
{
	void ** p;
	int	i;
	int	l;
	
	if(strRenderDLLName == NULL)
	{
		return FALSE;
	}

	// 只能加载一个脚本
	if(hModule != NULL)
	{
		return FALSE;
	}
	
	// dll不存在
	if(!file_exists(strRenderDLLName))
	{
		return FALSE;
	}
	
	hModule = LoadLibrary(strRenderDLLName);
	
	// 加载失败, 需要错误消息的话用GetLastError
	if (hModule == NULL)
	{
		return FALSE;
	}
	
	memset(&funs, 0, sizeof(funs));
	
	funs.render_attach = (void *)GetProcAddress(hModule, "render_attach");
	funs.render_detach = (void *)GetProcAddress(hModule, "render_detach");
	funs.render_pause = (void *)GetProcAddress(hModule, "render_pause");
	funs.render_start = (void *)GetProcAddress(hModule, "render_start");
	funs.render_stop = (void *)GetProcAddress(hModule, "render_stop");
	funs.render_RenderScene = (void *)GetProcAddress(hModule, "RenderScene");
	
	funs.render_create_object = (void *)GetProcAddress(hModule, "render_create_object");
	funs.render_create_panel = (void *)GetProcAddress(hModule, "render_create_panel");
	funs.render_create_map = (void *)GetProcAddress(hModule, "render_create_map");
	funs.render_add_data = (void *)GetProcAddress(hModule, "render_add_data");
	funs.render_switch_data = (void *)GetProcAddress(hModule, "render_switch_data");
	funs.render_move_2i = (void *)GetProcAddress(hModule, "render_move_2i");
	funs.render_move_2f = (void *)GetProcAddress(hModule, "render_move_2f");
	funs.render_move_3i = (void *)GetProcAddress(hModule, "render_move_3i");
	funs.render_move_3f = (void *)GetProcAddress(hModule, "render_move_3f");
	funs.render_rotate_1d = (void *)GetProcAddress(hModule, "render_rotate_1d");
	funs.render_rotate_3d = (void *)GetProcAddress(hModule, "render_rotate_3d");
	funs.render_scale = (void *)GetProcAddress(hModule, "render_scale");
	funs.render_delete_object = (void *)GetProcAddress(hModule, "render_delete_object");
	funs.render_move_camera_2d = (void *)GetProcAddress(hModule, "render_move_camera_2d");
	funs.render_move_camera_3d = (void *)GetProcAddress(hModule, "render_move_camera_3d");
	
	
	p = (void **)&funs;
	l = sizeof(funs)/sizeof(void *);
	for(i=0; i<l; i++)
	{
		if(*p == NULL)
		{
			return FALSE;
		}
		p++;
	}
	
	return TRUE;
}
	
bool CRender::file_exists(const char * file)
{	
	WIN32_FIND_DATA	fd;
	HANDLE		handle;	
		
	handle = FindFirstFile(file, &fd);
	FindClose(handle);
	
	if(handle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	return true;

}

BOOL	CRender::Attach(HWND hWnd)
{
	return ((LPFNRENDERATTACH)funs.render_attach)(hWnd);
}

BOOL	CRender::Detach(void)
{
	return ((LPFNRENDERDETACH)funs.render_detach)();
}

BOOL	CRender::Pause(void)
{
	return ((LPFNRENDERPAUSE)funs.render_pause)();
}

BOOL	CRender::Start(void)
{
	return ((LPFNRENDERSTART)funs.render_start)();
}

BOOL	CRender::Stop(void)
{
	return ((LPFNRENDERSTOP)funs.render_stop)();
}

void	CRender::RenderScene(void)
{
	((LPFNRENDERRENDERSCENE)funs.render_RenderScene)();
}


int CRender::CreateObject(void* buffer, int len, int type)
{
	return ((LPFNRENDERCREATEOBJECT)funs.render_create_object)(buffer, len, type);
}

int CRender::CreatePanel(void* buffer, int len, int type)
{
	return ((LPFNRENDERCREATEPANEL)funs.render_create_panel)(buffer, len, type);
}

int CRender::Createmap(void* buffer, int len, int type)
{
	return ((LPFNRENDERCREATEMAP)funs.render_create_map)(buffer, len, type);
}

int CRender::AddData(int object, void* buffer, int len, int type)
{
	return ((LPFNRENDERADDDATA)funs.render_add_data)(object, buffer, len, type);
}

int CRender::SwitchData(int index)
{
	return ((LPFNRENDERSWITCHDATA)funs.render_switch_data)(index);
}

int CRender::Move(int object, float x, float z)
{
	return ((LPFNRENDERMOVE2F)funs.render_move_2f)(object, x, z);
}

int CRender::Move(int object, float x, float y, float z)
{
	return ((LPFNRENDERMOVE3F)funs.render_move_3f)(object, x, y, z);
}

int CRender::Rotate(int object, double angle)
{
	return ((LPFNRENDERROTATE1D)funs.render_rotate_1d)(object, angle);
}

int CRender::Rotate(int object, double angle, double x, double y, double z)
{
	return ((LPFNRENDERROTATE3D)funs.render_rotate_3d)(object, angle, x, y, z);
}

int CRender::Scale(int object, double scale)
{
	return ((LPFNRENDERSCALE)funs.render_scale)(object, scale);
}

int CRender::DeleteObject(int object)
{
	return ((LPFNRENDERDELETEOBJECT)funs.render_delete_object)(object);
}

int CRender::MoveCamera(double x, double z)
{
	return ((LPFNRENDERMOVECAMERA2D)funs.render_move_camera_2d)(x, z);
}

int CRender::MoveCamera(double x, double y, double z)
{
	return ((LPFNRENDERMOVECAMERA3D)funs.render_move_camera_3d)(x, y, z);
}



























