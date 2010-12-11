


#include "script.h"
#include "game.h"

#include <windows.h>


//////////////////////////////////////////////////////////////////////////////////////////


// ��Ϊ�����DLL��ʽ�ģ������Զ�̬�ػ�ȡ��dll�к����ĵ�ַ�Ϳ�����ɲ�Ӱ�칦�ܵ�dll���滻��
// ��Ȼ���ֻ�ù̶����ֵĵ�һdll����dll�������һ��exe�������ͬ����Ϸ����Ҳ���Ծ�̬����
// ��dll�еĵ�ַ����ֻҪdllÿ�α�����򣨰��������ĵ���˳�򣩲��䣬�Ͳ���Ӱ�쵽���ù��̡�
// �����Ǽ���ȷ�������ڳ�ʼ��dll�ĺ�������Ҫ��dll�еı���һ�¡�
typedef int  (SCRIPT_EXPORT * LPFNSCRIPTINIT)();
typedef int  (SCRIPT_EXPORT * LPFNSCRIPTSETFUNC)(const char * fun_name, void * fun_address);
typedef int  (SCRIPT_EXPORT * LPFNSCRIPTCHECK)();
typedef int  (SCRIPT_EXPORT * LPFNSCRIPTSTART)();


#pragma pack(push)
#pragma pack(1)
static struct
{
	LPFNSCRIPTINIT		lpfn_script_init;
	LPFNSCRIPTSETFUNC	lpfn_script_set_func;
	LPFNSCRIPTCHECK		lpfn_script_check;
	LPFNSCRIPTSTART		lpfn_script_start;
}funs;
#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////////////////////

static HMODULE		hModule = NULL;


//////////////////////////////////////////////////////////////////////////////////////////

void _set_funs();


//////////////////////////////////////////////////////////////////////////////////////////

bool file_exists(const char * filename)
{
	WIN32_FIND_DATA	fd;
	HANDLE		handle;
	
	handle = FindFirstFile(filename, &fd);
	FindClose(handle);
	
	if(handle == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	return true;
	
}

bool script_init(const char * dllname)
{

	void **	p;
	int	i;
	int 	l;
	
	
	// ֻ�ܼ���һ���ű�
	if(hModule != NULL)
	{
		return false;
	}
	
	// �ű�dll������
	if(!file_exists(dllname))
	{
		return false;
	}
	
	hModule = LoadLibrary(dllname);
	
	// ����ʧ��, ��Ҫ������Ϣ�Ļ���GetLastError
	if (hModule == NULL)
	{
		return false;
	}
	
	memset(&funs, 0, sizeof(funs));
	
	funs.lpfn_script_init = (LPFNSCRIPTINIT)GetProcAddress(hModule, "script_init");
	funs.lpfn_script_set_func = (LPFNSCRIPTSETFUNC)GetProcAddress(hModule, "script_set_func");
	funs.lpfn_script_check = (LPFNSCRIPTCHECK)GetProcAddress(hModule, "script_check");
	funs.lpfn_script_start = (LPFNSCRIPTSTART)GetProcAddress(hModule, "script_start");

	
	// ͵���ˡ��������ж�ÿ����������������ȷ��ȡ�õ�ַ��
	p = (void **)&funs;
	l = sizeof(funs)/sizeof(void*);
	for(i=0; i<l; i++)
	{
		if(*p == NULL)
		{
			return false;
		}
		p++;
	}
	

	// ��dll�����ʼ����
	if( !funs.lpfn_script_init() )
	{
		return false;
	}
	
	// �趨dll���õĻص�������������ν���������ṩ���ű��Ľӿ�
	_set_funs();
	

	// DLL�����ӿ��趨������
	if( !funs.lpfn_script_check() )
	{
		return false;
	}
	
	// ������������г�ʼ����ɺ����һ�Σ�����������ʼ�ű���
	if( !funs.lpfn_script_start() )
	{
		return false;
	}
	
	return true;

}


void _set_funs()
{
	// �����õ��ļ���������������game.h����������
	// ����ʵ����game.cpp��
	funs.lpfn_script_set_func("map_create", (void *)map_create);
	funs.lpfn_script_set_func("map_set_background", (void *)map_set_background);
	funs.lpfn_script_set_func("map_add_trigger", (void *)map_add_trigger);
}


















