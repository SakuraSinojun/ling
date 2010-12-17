

#pragma once


#ifndef __SCRIPTHEADER_H__
#define __SCRIPTHEADER_H__



#ifdef __cplusplus
#define DLL_EXPORTS extern "C" __declspec(dllexport)
#else
#define DLL_EXPORTS __declspec(dllexport)
#endif

#define SCRIPT_EXPORT	__cdecl

typedef int (SCRIPT_EXPORT * LPFNBUFFPROC)(int idCaster, int idTarget, int idBuff);


// 下面是exe导出函数表。

// 精灵
int spirit_move(int id_spirit, int x, int z);
int spirit_get_state_pos_x(int spirit);
int spirit_get_state_pos_z(int spirit);


// 法术
int spell_give_timer_buff(int id_target, const char * buff_name ,const char * buff_desc, int uElapse, LPFNBUFFPROC lpfnBuffCallBack);
int spell_give_property_buff(int id_target, const char * property, const char * bonus, int lasttime);
int spell_remove_buff(int id_target, int id_buff);
int spell_create_spirit(const char * spirit_mod, double angle, int createlight, int prameter);
int spell_delete_spirit(int id_spirit);

// 生物
int creature_test_defence(int id_creature, const char * testwhat, int difficulty_class);
int creature_damage(int id_creature, const char * damage, const char * damagetype);
int creature_heal(int id_creature, int healvolume);
int creature_get_property(int id_creature, const char * property);


#endif
















