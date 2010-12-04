

#pragma once

#ifndef __COMMON_H__
#define __COMMON_H__

#include "comdef.h"
#include "Memory.h"



namespace common
{
        void Error(const char * string, ...);
        bool MemoryReport(char * report, int len);
}


//这个名字空间里的函数用于加载文件.
//pl = preload
//当重复加载同一个文件时,load函数会查找已加载的文件以防止重复利用.
namespace pl
{
	void * load(const char * filename, unsigned int& __out__len);	
	void unload(const char * filename);
	void unloadall();	
}

#endif
