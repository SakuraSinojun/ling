


#include "script.h"
#include "game.h"

#include <windows.h>


//////////////////////////////////////////////////////////////////////////////////////////


// 因为插件是DLL形式的，，所以动态地获取到dll中函数的地址就可以完成不影响功能的dll的替换。
// 当然如果只用固定名字的单一dll（多dll的情况是一个exe包含多个同类游戏），也可以静态链接
// 到dll中的地址，，只要dll每次编译规则（包括函数的导出顺序）不变，就不会影响到调用过程。
// 以下是几个确定的用于初始化dll的函数。需要跟dll中的保持一致。
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
	
	
	// 只能加载一个脚本
	if(hModule != NULL)
	{
		return false;
	}
	
	// 脚本dll不存在
	if(!file_exists(dllname))
	{
		return false;
	}
	
	hModule = LoadLibrary(dllname);
	
	// 加载失败, 需要错误消息的话用GetLastError
	if (hModule == NULL)
	{
		return false;
	}
	
	memset(&funs, 0, sizeof(funs));
	
	funs.lpfn_script_init = (LPFNSCRIPTINIT)GetProcAddress(hModule, "script_init");
	funs.lpfn_script_set_func = (LPFNSCRIPTSETFUNC)GetProcAddress(hModule, "script_set_func");
	funs.lpfn_script_check = (LPFNSCRIPTCHECK)GetProcAddress(hModule, "script_check");
	funs.lpfn_script_start = (LPFNSCRIPTSTART)GetProcAddress(hModule, "script_start");

	
	// 偷懒了。。。。判断每个导出函数都被正确地取得地址。
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
	

	// 让dll自身初始化。
	if( !funs.lpfn_script_init() )
	{
		return false;
	}
	
	// 设定dll中用的回调函数，就是所谓的主程序提供给脚本的接口
	_set_funs();
	

	// DLL检测各接口设定完毕与否
	if( !funs.lpfn_script_check() )
	{
		return false;
	}
	
	// 这个函数在所有初始化完成后调用一次，所以用来开始脚本。
	if( !funs.lpfn_script_start() )
	{
		return false;
	}
	
	return true;

}


void _set_funs()
{
	// 下面用到的几个函数，，都在game.h里有声明。
	// 具体实现在game.cpp里
	funs.lpfn_script_set_func("map_create", (void *)map_create);
	funs.lpfn_script_set_func("map_set_background", (void *)map_set_background);
	funs.lpfn_script_set_func("map_add_trigger", (void *)map_add_trigger);
}


















