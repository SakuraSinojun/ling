

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

// ����
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



// ����
int spell_register_buff(const char * buff_name ,const char * buff_desc, int buff_type)
{
	return 0;
}

int spell_give_buff(int id_target, int id_buff)
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


// ����
int creature_test_defence(int id_creature, const char * testwhat)
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





























