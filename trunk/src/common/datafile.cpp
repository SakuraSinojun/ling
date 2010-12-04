


#include <stdio.h>
#include <malloc.h>

void * _read_data_file(const char * filename, unsigned int & len)
{
	FILE	*	fp;
	void	*	buffer;
	
	if(filename == NULL)
	{	
		len = -1;
		return NULL;
	}
	
	fp = fopen(filename, "r");
	if( fp == NULL )
	{
		len = -1;
		return NULL;
	}
	
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	if(len == 0)
	{
		fclose(fp);
		return NULL;
	}
	
	buffer = malloc(len);
	fseek(fp, 0, SEEK_SET);
	fread(buffer, len, 1, fp);
	
	fclose(fp);
	return buffer; 
	
}





























