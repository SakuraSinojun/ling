

#pragma once

#ifndef __COMMON_H__
#define __COMMON_H__

#include "comdef.h"
#include "Memory.h"



namespace common
{
        void Error(const char * string, ...);			// error.cpp
        bool MemoryReport(char * report, int len);		// Memory.cpp
}


// ������ֿռ���ĺ������ڼ����ļ�.
// pl = preload
// ���ظ�����ͬһ���ļ�ʱ,load����������Ѽ��ص��ļ��Է�ֹ�ظ�����.
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
