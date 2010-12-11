
/*******************************************************************************

        Ahthor: ׼
        Tab:    8

*******************************************************************************/


#pragma once

#ifndef __COMMON_H__
#define __COMMON_H__

#include "comdef.h"
#include "typedef.h"


#ifndef __cplusplus
#define common::Error   Error
#define pl::load        load
#define pl::unload      unload
#define pl::unloadall   unloadall
#define res::loadimg    loadimg
#endif



#ifdef __cplusplus
namespace common
{
#endif
        void Error(const char * string, ...);			// error.cpp

#ifdef __cplusplus
}
#endif



// ������ֿռ���ĺ������ڼ����ļ�.
// pl = preload
// ���ظ�����ͬһ���ļ�ʱ,load����������Ѽ��ص��ļ��Է�ֹ�ظ�����.
// preload.cpp
#ifdef __cplusplus
namespace pl
{
#endif

	void * load(const char * filename, unsigned int& __out__len);
	void unload(const char * filename);
	void unloadall();
#ifdef __cplusplus
}
#endif


// datafile.cpp
#ifdef __cplusplus
namespace res
{
#endif
	void * loadimg(const char * filename, BMPINFOHEADER * info);
#ifdef __cplusplus
}
#endif


#endif













