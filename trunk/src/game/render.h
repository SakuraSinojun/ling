
/*******************************************************************************

        Author: Ó£Áè×¼
        Tab:    8
        
*******************************************************************************/

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
	static CRender * Get(void);
	BOOL	InitRender(const char * strRenderDLLName);
	
	
	BOOL	Attach(HWND hWnd);
	BOOL	Pause(void);
	BOOL	Start(void);
	BOOL	Stop(void);
	void	RenderScene(void);
	int	CreateObject(void* buffer, int len, int type);
	int	CreatePanel(void* buffer, int len, int type);
	int	Createmap(void* buffer, int len, int type);
	int	AddData(int object, void* buffer, int len, int type);
	int	SwitchData(int index);
	int	Move(int object, float x, float z);
	int	Move(int object, float x, float y, float z);
	int	Rotate(int object, double angle);
	int	Rotate(int object, double angle, double x, double y, double z);
	int	Scale(int object, double scale);
	int	DeleteObject(int object);
	int	MoveCamera(double x, double z);
	int	MoveCamera(double x, double y, double z);
	
private:
	static CRender * m_render;
	HMODULE	hModule;


	#pragma pack(push)
	#pragma pack(1)
	struct
	{
		void *	render_attach;
		void *	render_pause;
		void *	render_start;
		void *	render_stop;
		void *	render_RenderScene;

		void *  render_create_object;
		void *  render_create_panel;
		void *  render_create_map;
		void *  render_add_data;
		void *  render_switch_data;
		void *  render_move_2i;
		void *  render_move_2f;
		void *  render_move_3i;
		void *  render_move_3f;
		void *  render_rotate_1d;
		void *  render_rotate_3d;
		void *  render_scale;
		void *  render_delete_object;
		void *  render_move_camera_2d;
		void *  render_move_camera_3d;

	}funs;
	#pragma pack(pop)
	
	bool file_exists(const char * filename);

};

#endif



















