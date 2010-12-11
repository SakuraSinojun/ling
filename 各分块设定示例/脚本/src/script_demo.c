


/************************************************************************************

        这个文件编译完成后作为脚本来存在。
	
        游戏中：
                游戏代码中像使用动态加载的DLL一样与这个文件中的函数交互，一些变量的提
            供、逻辑的控制都可以放到这里来实现。
                用LoadLibrary加载这个DLL比常规方式加载DLL好的地方在于，就算把这个DLL跟
            其它资源一起打包，最终解包后把这个dll放到临时目录后一样可以正常使用。

        Toolset中：
                按照某种规则把脚本制作者写好的C代码样式的脚本作为函数包含到这个文件中的
            话，包含后的整个文件可以正确地被编译成一个DLL。具体构想看"构想.txt"文件中的
	    描述。


	>> 注意 <<
		如果用VC进行单步调试，这个文件中下的断点不会生效。单步需要在exe中调用
	这里的函数之前下断点。
		如果这个文件的内容被修改过，确定先把这个DLL生成后再对exe进行断点调试，
	否则虽然能断点跟踪到这个文件中，但是实际上执行的是DLL中的代码而不是这个文件中
	修改过的代码。

************************************************************************************/

/////////////////////////////////////////////////////////////////////////////////////
//
// 以下内容作为模版般的存在，在最终敲定后，对于单纯制作模组或脚本而言，不需要更改。
//

#include <string.h>
#include <memory.h>

#ifdef __cplusplus
#define DLL_EXPORTS extern "C" __declspec(dllexport)
#else
#define DLL_EXPORTS __declspec(dllexport)
#endif

#define SCRIPT_EXPORT	__cdecl

typedef void (SCRIPT_EXPORT * LPFNMAPCREATE)(int width, int height);
typedef void (SCRIPT_EXPORT * LPFNMAPSETBACKGROUND)(int x, int y, int x1, int y1, int x2, int y2, const char * element);
typedef void (SCRIPT_EXPORT * LPFNMAPADDTRIGGER)(int x, int y, void * trigger);

#pragma pack(push)
#pragma pack(1)
static struct
{
	LPFNMAPCREATE		lpfn_map_create;
	LPFNMAPSETBACKGROUND	lpfn_map_set_background;
	LPFNMAPADDTRIGGER	lpfn_map_add_trigger;
}funs;
#pragma pack(pop)

// 游戏主体给出的脚本接口名称
static const char * funnames[] = {	"map_create",
					"map_set_background",
					"map_add_trigger"
				};

////////////////////////////////////////////////////////////////////////

void tri_init_area();

////////////////////////////////////////////////////////////////////////

DLL_EXPORTS int SCRIPT_EXPORT script_init()
{
	memset(&funs, 0, sizeof(funs));
	return 1;

}

DLL_EXPORTS int SCRIPT_EXPORT script_check()
{
	int	l;
	int	i;
	void **	p;
	
	p = (void **)&funs;
	
	l = sizeof(funs)/sizeof(void *);

	for(i=0; i<l; i++)
	{
		if(*(p+i) == NULL)
		{
			return 0;
		}
	}
	return 1;
}


DLL_EXPORTS int SCRIPT_EXPORT script_set_func(const char * fun_name, void * fun_address)
{
	void **	p = (void **)&funs;
	int	l;
	int	i;
	
	l = sizeof(funs)/sizeof(void *);
	for(i=0; i<l; i++)
	{
		if(strcmp(fun_name, funnames[i]) == 0)
		{
			*(p + i) = fun_address;
			return 1;
		}
	}
	
	return 0;
}

DLL_EXPORTS int SCRIPT_EXPORT script_start()
{
	tri_init_area();
	return 1;
}


/************************************************************************************

	以下各函数算做导出给脚本的函数，这部分也是脚本直接调用游戏中的函数
	来完成功能的透明化实现的一部分。
	完成这部分后，在最终脚本语言中调用这里的某个函数，比如map_create()，
	感觉就像直接调用了game.cpp中的同名函数一样。事实上，下面这个map_create()
	调用之后最终也会跳到game.cpp中map_create()函数的调用。

************************************************************************************/


void map_create(int width, int height)
{
	if(funs.lpfn_map_create == NULL)
	{
		return;
	}
	funs.lpfn_map_create(width, height);
}

void map_set_background(int x, int y, int x1, int y1, int x2, int y2, const char * element)
{
	if(funs.lpfn_map_set_background == NULL)
	{
		return;
	}
	funs.lpfn_map_set_background(x, y, x1, y1, x2, y2, element);
}


void map_add_trigger(int x, int y, void * trigger_function)
{
	if(funs.lpfn_map_add_trigger == NULL)
	{
		return;
	}
	funs.lpfn_map_add_trigger(x, y, trigger_function);
}

/************************************************************************************

	下面的函数的内容应该是模组制作者完成的。

************************************************************************************/


////////////////////////////////////////////////////////////////////////
// toolset手工函数：
// 触发器：
// toolset在函数名前添加tri_字样：

//这个函数是必定生成的
void tri_init_area()
{
	int i, j;

	map_create(10, 10);
	
	for(i=0; i<10; i++)
	{
		for(j=0; j<10; j++)
		{
			map_set_background(i, j, 0, 0, 63, 63, "grass.bmp");
		}
	}
	
	
	for(i=0; i<10; i++)
	{
		map_set_background(i, 0, 64, 0, 127, 63, "grass.bmp");
		map_set_background(i, 8, 64, 0, 127, 63, "grass.bmp");
	}
	
	/*
	for(i=0; i<10; i++)
	{
		map_set_background(0, i, 64, 0, 127, 63, "grass.bmp");
		map_set_background(9, i, 64, 0, 127, 63, "grass.bmp");
	}
	*/

}


void tri_EnterDeepGrassArea()
{
}

void tri_TouchMonster()
{
}

void tri_SwitchMap()
{
}

