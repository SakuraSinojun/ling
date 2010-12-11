
/*******************************************************************************

        Ahthor: Ó£Áè×¼
        Tab:    8

*******************************************************************************/


#include "common.h"
#include <malloc.h>
#include <string.h>

BYTE * _read_data_file(const char * filename, unsigned int & len);

////////////////////////////////////////////////////////////////////////////////
typedef struct tagFILELINK
{
	char 	*		filename;
	void	*		content;
	unsigned int		len;
	
	struct tagFILELINK *	prev;
	struct tagFILELINK * 	next;

}FILE_LINK, *P_FILE_LINK;

static	FILE_LINK	*	_fl_first = NULL;
static	FILE_LINK	*	_fl_last  = NULL;

////////////////////////////////////////////////////////////////////////////////

P_FILE_LINK _find_file(const char * filename)
{
	FILE_LINK	* fl = _fl_first;
	while (fl != NULL)
	{
		if(strcmp(fl->filename, filename) == 0)
		{
			return fl;
		}
		fl = fl->next;
	}
	return NULL;
}

FILE_LINK * _add_file(const char * filename)
{
	FILE_LINK *	fl;
	void	*	buffer;
	unsigned int	len;
	
	buffer = _read_data_file(filename, len);
	if(buffer == NULL)
	{
		return NULL;
	}
	
	fl = (FILE_LINK *)malloc(sizeof(FILE_LINK));
	fl->filename = (char *)malloc(strlen(filename)+1);
	fl->content = buffer;
	fl->len = len;
	strcpy(fl->filename, filename);
	
	fl->prev = NULL;
	fl->next = NULL;
	
	if(_fl_last == NULL)
	{
		_fl_first = fl;
	}else{
		_fl_last->next = fl;
		fl->prev = _fl_last;
	}
	_fl_last = fl;
	
	return fl;
	
}


void * _load_file(const char * filename, unsigned int& len)
{
	FILE_LINK * 	fl;
	
 	fl = _find_file(filename);
	if(fl == NULL)
	{
		fl = _add_file(filename);
	}
	if(fl == NULL)
	{
		len = -1;
		return NULL;
	}else{
		len = fl->len;
		return fl->content;
	}
}

void _unload_file(const char * filename)
{
	FILE_LINK *	prev;
	FILE_LINK *	next;
	FILE_LINK *	fl = _find_file(filename);
	
	if( fl == NULL )
	{
		return;
	}
	
	prev = fl->prev;
	next = fl->next;
	
	
	free(fl->filename);
	free(fl);
	if(prev != NULL)
	{
		prev->next = next;
	}else{
		_fl_first = prev;
	}
	if(next != NULL)
	{
		next->prev = prev;
	}else{
		_fl_last = next;
	}
}

void _unload_all(void)
{
	_fl_last = _fl_first;
	while(_fl_last != NULL)
	{	
		_fl_first = _fl_first->next;
		free(_fl_last->filename);
		free(_fl_last);
		_fl_last = _fl_first;
	}
	
}

////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
namespace pl
{
#endif

	void * load(const char * filename, unsigned int& len)
	{
		return _load_file(filename, len);
	}
	
	void unload(const char * filename)
	{
		_unload_file(filename);
	}
	
	void unloadall(void)
	{
		_unload_all();
	}

#ifdef __cplusplus
}
#endif








