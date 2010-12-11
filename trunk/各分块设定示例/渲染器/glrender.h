


#pragma once 


#ifndef __GLRENDER_H__
#define __GLRENDER_H__



#ifdef __cplusplus
#define DLL_EXPORTS extern "C" __declspec(dllexport)
#else
#define DLL_EXPORTS __declspec(dllexport)
#endif

#include <windows.h>

#define RENDER_API	__cdecl

DLL_EXPORTS BOOL RENDER_API render_attach(HWND hWnd);
DLL_EXPORTS BOOL RENDER_API render_detach(void);

DLL_EXPORTS BOOL RENDER_API render_pause(void);
DLL_EXPORTS BOOL RENDER_API render_start(void);
DLL_EXPORTS BOOL RENDER_API render_stop(void);

DLL_EXPORTS void RENDER_API RenderScene(void);





DLL_EXPORTS int RENDER_API render_create_object(void* buffer, int len, int type);
DLL_EXPORTS int RENDER_API render_create_panel(void * buffer, int len, int type);
DLL_EXPORTS int RENDER_API render_create_map(void * buffer, int len, int type);
DLL_EXPORTS int RENDER_API render_add_data(int object, void * buffer, int len, int type);
DLL_EXPORTS int RENDER_API render_switch_data(int index);
DLL_EXPORTS int RENDER_API render_move_2i(int object, int x, int z);
DLL_EXPORTS int RENDER_API render_move_2f(int object, float x, float z);
DLL_EXPORTS int RENDER_API render_move_3i(int object, int x, int y, int z);
DLL_EXPORTS int RENDER_API render_move_3f(int object, float x, float y, float z);
DLL_EXPORTS int RENDER_API render_rotate_1d(int object, double angle);
DLL_EXPORTS int RENDER_API render_rotate_3d(int object, double angle, double x, double y, double z);
DLL_EXPORTS int RENDER_API render_scale(int object, double scale);
DLL_EXPORTS int RENDER_API render_delete_object(int object);
DLL_EXPORTS int RENDER_API render_move_camera_2d(double x, double z);
DLL_EXPORTS int RENDER_API render_move_camera_3d(double x, double y, double z);



#endif

