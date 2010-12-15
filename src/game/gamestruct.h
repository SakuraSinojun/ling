

#pragma once

#ifndef __GAMESTRUCT_H__
#define __GAMESTRUCT_H__



#include <windows.h>



struct MAPS
{
        DWORD   obj_index;
        LPSTR   map_name;
        //......          这个再整理。
}MAP_STRUCT;

typedef struct tagADVAN
{

}ADVAN, *PADVAN;

typedef struct tagSKILLSTRUCT
{
        LPSTR   skill_name;
        LPSTR   skill_desc;

        struct tagSKILLSTRUCT   *       prev;
        struct tagSKILLSTRUCT   *       next;

}SKILL_STRUCT, SKILL, *PSKILL;

typedef struct tagSPELLSTRUCT
{
        LPSTR   spell_name;
        LPSTR   spell_desc;
        
        //...prev;
        //...next;

}SPELL_STRUCT, SPELL, *PSPELL;


struct CREATURES
{
        DWORD           obj_index;
        DWORD           camp;           // Hero， 商人， 平民， 敌对
        DWORD           ST;
        DWORD           DX;
        DWORD           IQ;
        DWORD           HT;
        DWORD           HP;
        DWORD           Will;
        DWORD           Perp;
        DWORD           FP;
        
        PADVAN          advans;         // 优劣势表。
        PSKILL          skills;         // 技能表。
        PBUFF           buffs;          // buff表。
        PSPELL          spells;         // 法术列表。

};


struct MOD_STRUCT_1
{
        LPSTR   mod_name;
        DWORD   mod_version;
        LPSTR   mod_menu_pic;
        
};


#endif


























