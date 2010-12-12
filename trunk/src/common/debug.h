

#pragma once

#ifndef __DEBUG_H__
#define __DEBUG_H__



#define __RUNTIME_DEBUG_MESSAGE__


#ifdef  __RUNTIME_DEBUG_MESSAGE__
#include <stdio.h>
        inline void __out_debug_(const char * string, ...)
        {
                char buffer[1024];
                va_list arglist;

                if (NULL == string)
                {
                        return;
                }
                                                
                va_start(arglist, string);
                vsprintf(buffer, string, arglist);
                va_end(arglist);	
                printf(buffer); 
        }
#else
        inline void __out_debug_(const char * string, ...)
        {
        }
#endif

#endif

