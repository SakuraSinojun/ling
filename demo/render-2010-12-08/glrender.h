


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

DLL_EXPORTS void RenderScene(void);





#endif

