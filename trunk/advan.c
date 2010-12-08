

/*******************************************************************************

	说明：这个小程序用来生成预定优劣势二进制文件。

*******************************************************************************/

#include "gamedef.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>



#pragma pack(push)
#pragma pack(1)
typedef struct
{
	G_INT		reserved;
}RESERVED;
typedef struct
{
	G_CHAR		name[64];
	G_INT		charge;
	G_INT		script;
	RESERVED	reserved;
}ADVAN;
#pragma pack(pop)



typedef struct __link
{
	ADVAN			advan;
	struct __link *	prev;
	struct __link *	next;
}link;

link * head = NULL;
link * tail = NULL;

////////////////////////////////////////////////////////////////////////////////


void display_main_menu(void);
void main_loop(void);

void on_menu_add();
void on_menu_view();
void on_menu_modify();
void on_menu_delete();
void file_save();
void file_load();

void link_add(ADVAN * advan);
link * link_del(link * lk);
void link_clear();

////////////////////////////////////////////////////////////////////////////////


int main()
{
	link_clear();
	main_loop();
	link_clear();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
void display_main_menu()
{
	
	printf ("-------------------------------------------\n");
	printf ("|    a. 增加  b. 查看  c.修改   d.删除    |\n");
	printf ("|    q. 退出                              |\n");
	printf ("-------------------------------------------\n");
	
}

void main_loop()
{
	
	G_CHAR	input[100];
	
	file_load();
	
	while(G_TRUE)
	{
		display_main_menu();
		fgets(input, 100, stdin);
		switch(input[0])
		{
		case 'q':
			return;
		case 'a':
			on_menu_add();
			break;
		case 'b':
			on_menu_view();
			break;
		case 'c':
			on_menu_modify();
			break;
		case 'd':
			on_menu_delete();
			break;
		default:
			break;
		}
		file_save();
	}

}

void on_menu_add()
{
	ADVAN	advan;
	G_CHAR	input[100];
	
	while(G_TRUE)
	{
		memset(&advan, 0, sizeof(advan));
		printf("名称：");
		fgets(advan.name, 64, stdin);
		advan.name[strlen(advan.name)-1] = '\0';
		printf("花费："); 
		fgets(input, 100, stdin);
		advan.charge = atoi(input);
		
		link_add(&advan);

		file_save();
		
		printf("返回么？");
		fgets(input, 100, stdin);
		if(input[0] == 'y')
		{
			return;
		}	
	
	}
	
}

void on_menu_view()
{
	link * lk = head;
	while(lk != NULL)
	{
		printf("名称：[%10s] 花费：[%5d]\n", lk->advan.name, (int)lk->advan.charge);
		lk = lk->next;
	}
}


void on_menu_modify()
{
	G_CHAR	input[100];
	G_BOOL	flag;
	link *	lk;

	while(G_TRUE)
	{
		flag = G_FALSE;
		lk = head;
		while(lk != NULL)
		{
			printf("名称：[%10s] 花费：[%5d]\n", lk->advan.name, (int)lk->advan.charge);
			printf("--------------------\n");
			printf("修改这个么？");
			fgets(input, 100, stdin);
			if(input[0] == 'y')
			{
				flag = G_TRUE;
				break;
			}
			lk = lk->next;
		}
		if(!flag)
		{
			return;
		}
		lk = link_del(lk);
		file_save();
		if(lk == NULL)
		{
			return;
		}
		on_menu_add();
		printf("继续修改么？");
		fgets(input, 100, stdin);
		if(input[0] != 'y')
		{
			return;
		}
	}
}

void on_menu_delete()
{

	G_CHAR	input[100];
	G_BOOL	flag;
	link *	lk;

	while(G_TRUE)
	{
		flag = G_FALSE;
		lk = head;
		while(lk != NULL)
		{
			printf("名称：[%10s] 花费：[%5d]\n", lk->advan.name, (int)lk->advan.charge);
			printf("--------------------\n");
			printf("删除这个么？");
			fgets(input, 100, stdin);
			if(input[0] == 'y')
			{
				flag = G_TRUE;
				break;
			}
			lk = lk->next;
		}
		if(!flag)
		{
			return;
		}
		lk = link_del(lk);
		file_save();
		if(lk == NULL)
		{
			return;
		}
		printf("继续删除么？");
		fgets(input, 100, stdin);
		if(input[0] != 'y')
		{
			return;
		}
	}
	
	
}

void file_save()
{
	FILE *	fp;
	link *	lk;

	fp = fopen("advan.bin", "wb");
	
	lk = head;
	while(lk != NULL)
	{	
		if(fwrite(&lk->advan, sizeof(ADVAN), 1, fp) != 1)
		{
			printf("写文件失败。\n");
			fclose(fp);
			return;
		}
		lk = lk->next;
	}
	fclose(fp);
	
}

void file_load()
{
	FILE *	fp;
	ADVAN	advan;
	G_INT	len;
	G_INT	i;
	
	fp = fopen("advan.bin", "rb");
	if(fp == NULL)
	{
		printf("打不开文件。\n");
		return;
	}
	
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	
	if(len == 0)
	{
		fclose(fp);
		return;
	}
	
	fseek(fp, 0, SEEK_SET);

	link_clear();
	
	len = len/sizeof(ADVAN);
	
	for(i=0; i<len; i++)
	{
		fread(&advan, sizeof(ADVAN), 1, fp);
		link_add(&advan);
	}
	
	fclose(fp);
	
}



void link_add(ADVAN * advan)
{
	link * lk = (link*)malloc(sizeof(link));
	
	memcpy(&lk->advan, advan, sizeof(ADVAN));
	lk->next = NULL;
	lk->prev = NULL;

	if(tail == NULL)
	{
		head = lk;
	}else{
		tail->next = lk;
		lk->prev = tail;
	}
	tail = lk;
		
}

link * link_del(link * lk)
{
	link *	prev;
	link *	next;

	prev = lk->prev;
	next = lk->next;

	free(lk);
	

	if(prev != NULL)
	{
		prev->next = next;
	}else{
		head = next;
	}

	if(next != NULL)
	{
		next->prev = prev;
	}else{
		tail = prev;
	}
	if(prev == NULL)
	{
		return head;
	}else{
		return prev;
	}
}

void link_clear()
{
	tail = head;
	while(tail != NULL)
	{
		head = head->next;
		free(tail);
		tail = head;
	}
	head = NULL;
	tail = NULL;
}





































