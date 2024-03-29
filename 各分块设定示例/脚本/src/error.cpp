

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>	


namespace common
{
        void Error(const char * string, ...)
        {
                if (NULL == string)
                {
                        return;
                }

                std::ofstream ofs("error.log", std::ios::app);

                char buffer[1024];
                va_list arglist;

                va_start(arglist, string);
                vsprintf(buffer, string, arglist);
                va_end(arglist);

                std::cout<<buffer<<std::endl;
                ofs<<buffer;
                ofs.close();

        }
}

