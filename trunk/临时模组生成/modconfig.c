
/*
        author: Sakura

*/

#include "modconfig.h"
#include <windows.h>
#include <string.h>
#include <stdio.h>

BOOL file_exists(const char * file)
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


BOOL mc_save_config(const char * mod_file, 
                    const char * mod_name, 
                    const char * mod_desc, 
                    DWORD revision)
{
        MODCONFIGFILE   mcf;
        FILE    *       file;
        
        
        memset(&mcf, 0, sizeof(mcf));
        strcpy(mcf.mod_file_type, "MOD");
        strncpy(mcf.mod_name, mod_name, 63);
        strncpy(mcf.mod_desc, mod_desc, 511);
        mcf.mod_revision = revision;
        
        
        file = fopen(mod_file, "wb");
        if(file == NULL)
        {
                return FALSE;
        }
        if(fwrite(&mcf, sizeof(mcf), 1, file) != 1)
        {
                fclose(file);
                return FALSE;
        }
        fclose(file);
        return TRUE;
}


BOOL mc_read_config(const char * mod_file, PMODCONFIGFILE __out mcf)
{
        FILE    *       file;
        
        if(mcf == NULL)
        {
                return FALSE;
        }
        
        memset(mcf, 0, sizeof(MODCONFIGFILE));
        
        if(!file_exists(mod_file))
        {
                return FALSE;
        }
        
        file = fopen(mod_file, "rb");
        if(file == NULL)
        {
                return FALSE;
        }
        if(fread(mcf, sizeof(MODCONFIGFILE), 1, file) != 1)
        {
                fclose(file);
                return FALSE;
        }
        
        fclose(file);
        return TRUE;

}











