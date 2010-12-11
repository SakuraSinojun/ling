


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





























