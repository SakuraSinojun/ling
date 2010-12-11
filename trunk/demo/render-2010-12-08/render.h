

#pragma once

#ifndef __RENDER_H__
#define __RENDER_H__

#include <windows.h>



class CRender
{

protected:
	CRender();
public:
	~CRender();
	static CRender * GetRender(void);
	BOOL	InitRender(const char * strRenderDLLName);
	
	
	BOOL	Attach(HWND hWnd);
	BOOL	Detach(void);
	BOOL	Pause(void);
	BOOL	Start(void);
	BOOL	Stop(void);
	void	RenderScene(void);
	
private:
	static CRender * m_render;
	HMODULE	hModule;


	#pragma pack(push)
	#pragma pack(1)
	struct
	{
		void *	render_attach;
		void *	render_detach;
		void *	render_pause;
		void *	render_start;
		void *	render_stop;
		void *	render_RenderScene;
	}funs;
	#pragma pack(pop)
	
	bool file_exists(const char * filename);

};

#endif



















