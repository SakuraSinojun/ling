


#include <windows.h>


bool file_exists(const char * file)
{	
        WIN32_FIND_DATA	fd;
        HANDLE		handle;	
                
        handle = FindFirstFile(file, &fd);
        FindClose(handle);
        
        if(handle == INVALID_HANDLE_VALUE)
        {
                return FALSE;
        }
        
        return TRUE;

}

char * GetGamePath()
{
        static char * path = NULL;
        int i;
        if(path == NULL)
        {
                path = (char *)malloc(MAX_PATH);
                memset(path, 0, MAX_PATH);
                GetModuleFileName(NULL, path, MAX_PATH);
                for(i=MAX_PATH-1; i>=0; i--)
                {
                        if(*(path+i) != '\\')
                        {
                                *(path+i) = '\0';
                        }else{
                                *(path+i) = '\0';
                                break;
                        }
                }
        }
        
        return path;

}
