

#include <stdio.h>
#include <stdarg.h>	
#include <string.h>

namespace common
{
        void Error(const char * string, ...)
        {
		FILE * fp;
		char buffer[1024];
                va_list arglist;

                if (NULL == string)
                {
                        return;
                }
                                                
                va_start(arglist, string);
                vsprintf(buffer, string, arglist);
                va_end(arglist);

                fp = fopen("error.log", "a");
		fwrite(buffer, strlen(buffer), 1, fp);
		fwrite("\n", 1, 1, fp);
		fclose(fp);
		
		printf(buffer); 

        }
        
}

