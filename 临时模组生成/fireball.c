

#include "scriptheader.h"

#include <string.h>
#include <memory.h>



/*
            ����������
                ʩ�Ż�������ʩ���ߵõ�һ��״̬��buff��IQ����1d4�㣬����30�롣
                ���������ض�������У����ײ����һ��Ŀ�꣬��Ŀ����Ҫ��һ������
            �춨���춨�Ѷ�Ϊ 
                20 + (ʩ����IQ-10)/2
                ���Ŀ��ͨ�������Ѷȼ춨�����ܵ�2d6������˺������û��ͨ����
            �ݼ춨�����ܵ�4d6���˺���
                Ŀ�걻���к󣬻��һ��������ȼ����buff�����buff��Ч����ÿ��
            3������һ�����ݼ춨������춨ͨ����buff��ʧ������춨ʧ�ܣ����ܵ�
            1d6������˺���
*/


int SCRIPT_EXPORT buff_burning(int idCaster, int idTarget, int idBuff)
{
        int res;
        idCaster = 0;
        
        res = creature_test_defence(idTarget, "DX", 20);
        if(res)
        {
                spell_remove_buff(idTarget, idBuff);
        }else{
                creature_damage(idTarget, "1d6", "fire");
        }
        return 0;
        
}


DLL_EXPORTS char * SCRIPT_EXPORT spell_name(void)
{
        return "������";
}

DLL_EXPORTS char * SCRIPT_EXPORT spell_desc(void)
{
        return "��������������";
}

// ����ж�����е��߿���Ϊ�����ò�ͬ��parameterֵ��
DLL_EXPORTS int SCRIPT_EXPORT spell_cast(int idCaster, int idTarget, int vector_x, int vector_z)
{
        int fireball;
        int x, z;
        
        // ��������ʵû�á���ֻ�Ǵ�/WX��ʱ�����֡�δʹ�õ��βΡ��ľ��档
        x = vector_x;
        z = vector_z;
        idTarget = 0;
        
        fireball = spell_create_spirit("fireball.3ds", 0, 0, 0);
        x = spirit_get_state_pos_x(idCaster);
        z = spirit_get_state_pos_z(idCaster);
        
        spirit_move(fireball, x, z);
        
        spell_give_property_buff(idCaster, "IQ", "1d4", 30);
        
        return 0;
}


DLL_EXPORTS int SCRIPT_EXPORT spell_wander_about(       
        int idCaster, 
        int idTarget, 
        int idSpirit, 
        int parameter,
        int vector_x, 
        int vector_z
)
{
        int x, z;
        int res;
        static int ox = 0, oz = 0;
        
        parameter = 0;
        
        x = spirit_get_state_pos_x(idSpirit);
        z = spirit_get_state_pos_z(idSpirit);
        
        // ����û�м����ٶȡ�
        x += vector_x;
        z += vector_z;
        
        if(ox == 0)
        {
                ox = x;
        }
        if(oz == 0)
        {
                oz = z;
        }
        
        if((x-ox)*(x-ox)+(z-oz)*(z-oz) > 300)
        {
                spell_delete_spirit(idSpirit);
                return 0;
        }
        
        if(idTarget == 0)
        {
                // δ���С�
                spirit_move(idSpirit, x, z);
        }else{
                // ���С�
                res = creature_test_defence(idTarget, "DX", 20+(creature_get_property(idCaster, "IQ")-10)/2);   // ��������
                
                if (res != 0)
                {
                        // ͨ��
                        creature_damage(idTarget, "2d6", "fire");
                }else{
                        creature_damage(idTarget, "4d6", "fire");
                }
                spell_delete_spirit(idSpirit);
                spell_give_timer_buff(idTarget, "����ȼ", 
                        "�㱻���������У�����ÿ��3���ӻ��ܵ�һ���˺���ֱ��DX����ͨ����",
                        3, buff_burning);
        }
        
        return 0;

}















