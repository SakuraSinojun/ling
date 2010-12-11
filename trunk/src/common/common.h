
/*******************************************************************************

        Ahthor: ׼
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
