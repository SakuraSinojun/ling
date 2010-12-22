

#pragma once

#ifndef __DEBUG_H__
#define __DEBUG_H__

#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>



#define __GAME_DEBUG_MESSAGE__

#ifdef  __GAME_DEBUG_MESSAGE__

#ifndef __func__
#       ifdef __FUNCTION__
#               define __func__  __FUNCTION__
#       else
#               define __func__  __FILE__
#       endif
#endif


#define malloc(x) malloc_debug(x, __FILE__, __LINE__, __func__)
#define free(x) free_debug(x)
#define NEW_DEBUG new(__FILE__, __LINE__, __func__)


void __out_debug_(const char * string, ...);
void * malloc_debug(    unsigned int num_bytes,
                        const char * file,
                        const int line,
                        const char * function
                   );


void free_debug(void * ptr);

void * operator new(    size_t size, 
                          const char * file, 
                          const int line, 
                          const char * function);
void operator delete(void * p);
void operator delete[] (void * p);
void operator delete( void * p,
                        const char * file,
                        const int line, 
                        const char * function
                      )throw();


#else

#define NEW_DEBUG       new

inline void __out_debug_(const char * string, ...)
{
}
        
        
#endif

#endif

