

#include "scriptheader.h"



/*
typedef struct
{
        
}EXPORT_FUNCTIONS;

static const char * funnames[] = 
{
        "register_buff",
        "give_buff"

};
*/

// 精灵
int spirit_move(int id_spirit, int x, int z)
{
	return 0;
}

int spirit_get_state_pos_x(int spirit)
{
	return 0;
}

int spirit_get_state_pos_z(int spirit)
{
	return 0;
}



// 法术
int spell_give_timer_buff(int id_target, const char * buff_name ,const char * buff_desc, int uElapse, LPFNBUFFPROC lpfnBuffCallBack)
{
        return 0;
}

int spell_give_property_buff(int id_target, const char * property, const char * bonus, int lasttime)
{
        return 0;
}

int spell_remove_buff(int id_target, int id_buff)
{
        return 0;
}

int spell_create_spirit(const char * spirit_mod, double angle, int createlight, int prameter)
{
	return 0;
}

int spell_delete_spirit(int id_spirit)
{
	return 0;
}


// 生物
int creature_test_defence(int id_creature, const char * testwhat, int difficulty_class)
{
	return 0;
}

int creature_damage(int id_creature, const char * damage, const char * damagetype)
{
	return 0;
}

int creature_heal(int id_creature, int healvolume)
{
	return 0;
}

int creature_get_property(int id_creature, const char * property)
{
        return 0;
}




























