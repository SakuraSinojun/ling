#include "Memory.h"
#include <malloc.h>
//#include <stdio.h>
#include <string>

#include "stdafx.h"

#include <windows.h>


#ifdef MEMCHECK

	static long _MemorySize=0;
	class CMLink
	{
	public:
		CMLink * next;
		CMLink * prev;
		size_t size;
		char file[50];
		int line;

	public:
		void * pointer;
		
		void AddToLink(void * _p,size_t _size,const char *_file,int _line)
		{
			pointer=_p;
			prev=0;
			next=0;
			if(last==0)
			{
				first=this;
			}else{
				last->next=this;
				this->prev=last;
			}
			last=this;
			size=_size;
			strcpy_s(file,_file);
			line=_line;
			_MemorySize+=size;
		}

		CMLink(void * _p,size_t _size,const char * _file,int _line)
		{
		}

		~CMLink()
		{
			CMLink * prev;
			CMLink * next;
			prev=this->prev;
			next=this->next;
			if(prev!=0)
			{
				prev->next=next;
			}else{
				first=next;
			}
			if(next!=0)
			{
				next->prev=prev;
			}else{
				last=prev;
			}
			_MemorySize-=size;
		}
	}*first=0,*last=0;

	void Remove(void *p)
	{
		CMLink * _lk=first;
		while(_lk!=0)
		{
			if(_lk->pointer ==p)
			{
				_lk->~CMLink ();
				free(_lk);
				break;
			}
			_lk=_lk->next;	
		}

	}
	

	bool MemoryReport(char * report,int len)
	{
		CMLink * _lk=first;
		char temp[150];
		
		memset(report,0,len);
		
		int lenused=0;
		int l=150;

		while(_lk!=0)
		{
			//_set_printf_count_output(1);
			sprintf_s(temp,"未释放的内存:地址：0x00%x，大小：%d字节，分配于%s的第%d行。\n",(long)_lk->pointer,_lk->size,_lk->file,_lk->line);
			lenused+=l;
			
			if(lenused>=len)
				return false;
			if(strcat_s(report,len,temp)!=0)
				return false;
			_lk=_lk->next;
		}
		
		return true;

	}


	void *__cdecl operator new(size_t size,const char * file,const int line) throw(...)
    {   
		//void * p = ::operator new(size);
		void *p=malloc(size);

		//CMLink * _cm=::operator new CMLink(p,size,file,line);
		
		CMLink *_cm=(CMLink *)malloc(sizeof(CMLink));
		_cm->AddToLink (p,size,file,line);

		return (p);
    }

	void operator delete(void * p)
	{
		Remove(p);

		//operator delete(p);
		free(p);
	}

	void operator delete[] (void * p)
	{
		Remove(p);
		free(p);
	}

	void operator delete(void * p,const char * file,const int line) throw(...)
	{
		TRACE("内存分配失败！");
		free(p);
	}

//};




#else
#endif