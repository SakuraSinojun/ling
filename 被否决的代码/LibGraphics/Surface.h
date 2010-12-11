#ifndef SURFACE_H_011204
#define SURFACE_H_011204

class CSurface
{
public:
        CSurface(){}
        ~CSurface(){}
	
	virtual bool CreateMainWnd(int width,int height, bool bWindow, HWND)=0;
        virtual bool Create(int width, int height)=0;
	virtual void ShutDown()=0;
	virtual bool LoadBitmap(const char *fileName)=0;
        virtual bool AttachSurface(const CSurface *sur, int x, int y)=0;
	virtual bool SetColorKey(DWORD color)=0;
	virtual int  GetWidth() const=0;
	virtual int  GetHeight() const=0;
        virtual bool Flip()=0;
        virtual bool DrawSolidRect(DWORD color, LPRECT lpRect)=0;
        virtual bool DrawLine(int x1, int y1, int x2, int y2, DWORD color) = 0;

protected:
};

#endif