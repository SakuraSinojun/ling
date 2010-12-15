

#pragma once

#ifndef __GAMESTRUCT_H__
#define __GAMESTRUCT_H__



#include <windows.h>



struct MAPS
{
        DWORD   obj_index;
        LPSTR   map_name;
        //......          ���������
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
        DWORD           camp;           // Hero�� ���ˣ� ƽ�� �ж�
        DWORD           ST;
        DWORD           DX;
        DWORD           IQ;
        DWORD           HT;
        DWORD           HP;
        DWORD           Will;
        DWORD           Perp;
        DWORD           FP;
        
        PADVAN          advans;         // �����Ʊ�
        PSKILL          skills;         // ���ܱ�
        PBUFF           buffs;          // buff��
        PSPELL          spells;         // �����б�

};


struct MOD_STRUCT_1
{
        LPSTR   mod_name;
        DWORD   mod_version;
        LPSTR   mod_menu_pic;
        
};


#endif


























