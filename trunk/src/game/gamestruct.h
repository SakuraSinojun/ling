

#pragma once

#ifndef __GAMESTRUCT_H__
#define __GAMESTRUCT_H__



#include <windows.h>



typedef struct MAPS
{
        DWORD   obj_index;
        LPSTR   map_name;
        //......          这个再整理。
}MAP_STRUCT, MAPS, *PMAPS;

typedef struct tagADVAN
{
        LPSTR   advan_name;
        LPSTR   advan_desc;
        DWORD   advan_charge;
        DWORD   advan_script;
        
        struct tagADVAN * prev;
        struct tagADVAN * next;
}ADVAN, *PADVAN;

typedef struct tagSKILLSTRUCT
{
        LPSTR   skill_name;
        LPSTR   skill_desc;
        DWORD   skill_script;
        
        struct tagSKILLSTRUCT   *       prev;
        struct tagSKILLSTRUCT   *       next;

}SKILL_STRUCT, SKILL, *PSKILL;

typedef struct tagSPELLSTRUCT
{
        LPSTR   spell_name;
        LPSTR   spell_desc;
        DWORD   spell_script;
        
        struct tagSPELLSTRUCT * prev;
        struct tagSPELLSTRUCT * next;

}SPELL_STRUCT, SPELL, *PSPELL;


typedef struct CREATURES
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
        BOOL            isDead;
        
        PADVAN          advans;         // 优劣势表。
        PSKILL          skills;         // 技能表。
        PSPELL          spells;         // 法术列表。
        
        PBUFF           buffs;          // buff表。

}CREATURES, *PCREATURES;

typedef struct INDEXMAPPING
{
        LPSTR   index;
        LPSTR   mapping;
        
        struct INDEXMAPPING * prev;
        struct INDEXMAPPING * next;
}INDEXMAPPING, *PINDEXMAPPING;

struct MOD_STRUCT_1
{
        LPSTR           mod_name;
        DWORD           mod_version;
        LPSTR           mod_menu_pic;        
        PINDEXMAPPING   mod_index_mapping;
        
        PCREATURES      mod_creatures;
        
};


#endif


























