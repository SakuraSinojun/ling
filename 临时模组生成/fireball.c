

#include "scriptheader.h"

#include <string.h>
#include <memory.h>



// 如果有多个飞行道具可以为其设置不同的parameter值。

DLL_EXPORTS int SCRIPT_EXPORT spell_cast(int idCaster, int idTarget, int vecter_x, int vector_z)
{
        int fireball;
        int x, z;
        
        fireball = spell_create_spirit("fireball.3ds", 0, 0, 0);
        x = spirit_get_state_pos_x(idCaster);
        z = spirit_get_state_pos_z(idCaster);
        
        spirit_move(fireball, x, z);
        
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
        
        
        x = spirit_get_state_pos_x(idSpirit);
        z = spirit_get_state_pos_z(idSpirit);
        
        // 这里没加速度计算。
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
        
        if((x-ox)*(x-ox)+(z-oz)*(z-oz) > 100)
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
                res = creature_test_defence(idTarget, "DX");   // 测试敏捷
                
                if (res != 0)
                {
                        // 通过
                        creature_damage(idTarget, "4d6", "fire");
                }else{
                        creature_damage(idTarget, "2d6", "fire");
                }
                spell_delete_spirit(idSpirit);
                
        }
        
        return 0;

}















