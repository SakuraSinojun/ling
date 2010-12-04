#ifndef SURFACE_H_011204
#define SURFACE_H_011204

class CSurface
{
	virtual ~CSurface(){}
	
	virtual bool CreateGameWnd(int width,int height, bool bWindow = true, HWND = NULL)=0;
	virtual bool ShutDown()=0;
	virtual bool Load(const char * name)=0;			
	virtual bool SetColorKey(bool bColorKey)=0;	

	virtual int Width() const=0;					
	virtual int Height() const=0;				

protected:
	CSurface(){}
};

#endif