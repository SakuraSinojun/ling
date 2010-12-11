


/************************************************************************************

        ����ļ�������ɺ���Ϊ�ű������ڡ�
	
        ��Ϸ�У�
                ��Ϸ��������ʹ�ö�̬���ص�DLLһ��������ļ��еĺ���������һЩ��������
            �����߼��Ŀ��ƶ����Էŵ�������ʵ�֡�
                ��LoadLibrary�������DLL�ȳ��淽ʽ����DLL�õĵط����ڣ���������DLL��
            ������Դһ���������ս��������dll�ŵ���ʱĿ¼��һ����������ʹ�á�

        Toolset�У�
                ����ĳ�ֹ���ѽű�������д�õ�C������ʽ�Ľű���Ϊ��������������ļ��е�
            ����������������ļ�������ȷ�ر������һ��DLL�����幹�뿴"����.txt"�ļ��е�
	    ������


	>> ע�� <<
		�����VC���е������ԣ�����ļ����µĶϵ㲻����Ч��������Ҫ��exe�е���
	����ĺ���֮ǰ�¶ϵ㡣
		�������ļ������ݱ��޸Ĺ���ȷ���Ȱ����DLL���ɺ��ٶ�exe���жϵ���ԣ�
	������Ȼ�ܶϵ���ٵ�����ļ��У�����ʵ����ִ�е���DLL�еĴ������������ļ���
	�޸Ĺ��Ĵ��롣

************************************************************************************/

/////////////////////////////////////////////////////////////////////////////////////
//
// ����������Ϊģ���Ĵ��ڣ��������ö��󣬶��ڵ�������ģ���ű����ԣ�����Ҫ���ġ�
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

// ��Ϸ��������Ľű��ӿ�����
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

	���¸����������������ű��ĺ������ⲿ��Ҳ�ǽű�ֱ�ӵ�����Ϸ�еĺ���
	����ɹ��ܵ�͸����ʵ�ֵ�һ���֡�
	����ⲿ�ֺ������սű������е��������ĳ������������map_create()��
	�о�����ֱ�ӵ�����game.cpp�е�ͬ������һ������ʵ�ϣ��������map_create()
	����֮������Ҳ������game.cpp��map_create()�����ĵ��á�

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

	����ĺ���������Ӧ����ģ����������ɵġ�

************************************************************************************/


////////////////////////////////////////////////////////////////////////
// toolset�ֹ�������
// ��������
// toolset�ں�����ǰ���tri_������

//��������Ǳض����ɵ�
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

