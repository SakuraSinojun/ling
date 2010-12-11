

#pragma once

#ifndef __MEMORY_H__
#define __MEMORY_H__


#define MEMCHECK


#ifdef MEMCHECK
#define MNEW 	new(__FILE__,__LINE__)
#define NEW 	new(__FILE__,__LINE__)
#define MDELETE delete


void * operator new(size_t size,const char * file,const int line);
void operator delete(void * p,const char * file,const int line);

void operator delete(void * p);
void operator delete[] (void * p);
//bool MemoryReport(char * report,int len);




#else
#define NEW new
#endif

#endif

