

#include "modconfig.h"

#include <windows.h>
#include <stdio.h>




int main()
{
        MODCONFIGFILE   mcf;
        
        if(!mc_save_config("config", "����ģ��", "���ģ��ֻ���������ԡ�", (DWORD)0))
        {
                printf("д�����");
                return 0;
        }
        
        if(!mc_read_config("config", &mcf))
        {
                printf("��ȡ����");
                return 0;
        }
        
        printf("%s\n%s\n%u\n", mcf.mod_name, mcf.mod_desc, (unsigned)mcf.mod_revision);
        
        return 0;
}


















