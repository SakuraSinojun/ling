
/*
        author: SakuraSinojun

*/


#pragma once

#ifndef __MODCONFIG_H__
#define __MODCONFIG_H__

#include <windows.h>


#define __out   
#define __in


typedef struct MODCONFIGFILE
{
        CHAR    mod_file_type[4];
        CHAR    mod_name[64];
        CHAR    mod_desc[512];
        DWORD   mod_revision;
}MODCONFIGFILE, *PMODCONFIGFILE;



BOOL mc_save_config(const char * mod_file, 
                    const char * mod_name, 
                    const char * mod_desc, 
                    DWORD revision);
                    
BOOL mc_read_config(const char * mod_file, PMODCONFIGFILE __out mcf);


#endif















