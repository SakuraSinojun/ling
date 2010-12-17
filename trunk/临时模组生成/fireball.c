

#include "scriptheader.h"

#include <string.h>
#include <memory.h>



/*
            火球术规则：
                施放火球术后，施法者得到一个状态型buff：IQ增加1d4点，持续30秒。
                火球术向特定方向飞行，如果撞到了一个目标，则目标需要做一次敏捷
            检定，检定难度为 
                20 + (施法者IQ-10)/2
                如果目标通过法术难度检定，则受到2d6点火焰伤害。如果没有通过敏
            捷检定，则受到4d6点伤害。
                目标被击中后，获得一个“被点燃”的buff。这个buff的效果是每隔
            3秒钟做一次敏捷检定，如果检定通过，buff消失；如果检定失败，则受到
            1d6点火焰伤害。
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
        return "火球术";
}

DLL_EXPORTS char * SCRIPT_EXPORT spell_desc(void)
{
        return "火球术的描述。";
}

// 如果有多个飞行道具可以为其设置不同的parameter值。
DLL_EXPORTS int SCRIPT_EXPORT spell_cast(int idCaster, int idTarget, int vector_x, int vector_z)
{
        int fireball;
        int x, z;
        
        // 这三行其实没用。。只是打开/WX的时候会出现“未使用的形参”的警告。
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
        
        // 这里没有计算速度。
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
                // 未击中。
                spirit_move(idSpirit, x, z);
        }else{
                // 击中。
                res = creature_test_defence(idTarget, "DX", 20+(creature_get_property(idCaster, "IQ")-10)/2);   // 测试敏捷
                
                if (res != 0)
                {
                        // 通过
                        creature_damage(idTarget, "2d6", "fire");
                }else{
                        creature_damage(idTarget, "4d6", "fire");
                }
                spell_delete_spirit(idSpirit);
                spell_give_timer_buff(idTarget, "被点燃", 
                        "你被火球术击中，所以每隔3秒钟会受到一次伤害，直至DX豁免通过。",
                        3, buff_burning);
        }
        
        return 0;

}















