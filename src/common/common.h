

#pragma once

#ifndef __COMMON_H__
#define __COMMON_H__

#include "comdef.h"
#include "Memory.h"




//��󳤶�1024�ֽ�
void com_error(const char * string, ...);




//������ֿռ���ĺ������ڼ����ļ�.
//pl = preload
//���ظ�����ͬһ���ļ�ʱ,load����������Ѽ��ص��ļ��Է�ֹ�ظ�����.
namespace pl
{
	void * load(const char * filename, unsigned int& __out__len);	
	void unload(const char * filename);
	void unloadall();
	
}

#endif
