

#include "modconfig.h"

#include <windows.h>
#include <stdio.h>




int main()
{
        MODCONFIGFILE   mcf;
        
        if(!mc_save_config("config", "测试模组", "这个模组只是用来测试。", (DWORD)0))
        {
                printf("写入错误！");
                return 0;
        }
        
        if(!mc_read_config("config", &mcf))
        {
                printf("读取错误！");
                return 0;
        }
        
        printf("%s\n%s\n%u\n", mcf.mod_name, mcf.mod_desc, (unsigned)mcf.mod_revision);
        
        return 0;
}


















