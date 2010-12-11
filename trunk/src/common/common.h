
/*******************************************************************************

        Ahthor: 准
        Tab:    8

*******************************************************************************/


#pragma once

#ifndef __COMMON_H__
#define __COMMON_H__

#include "comdef.h"
#include "typedef.h"


namespace common
{
        void Error(const char * string, ...);			// error.cpp
}


// 这个名字空间里的函数用于加载文件.
// pl = preload
// 当重复加载同一个文件时,load函数会查找已加载的文件以防止重复利用.
// preload.cpp
namespace pl
{
	void * load(const char * filename, unsigned int& __out__len);
	void unload(const char * filename);
	void unloadall();
}

// datafile.cpp
namespace res
{
	void * loadimg(const char * filename, BMPINFOHEADER * info);
}

#endif
