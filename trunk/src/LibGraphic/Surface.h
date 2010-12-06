#ifndef SURFACE_H_011204
#define SURFACE_H_011204

class CSurface
{
public:
        CSurface(){}
        ~CSurface(){}
	
        // 创建一个主画面，这个画面是其他画面存在的前提
	virtual bool CreateMainWnd(int width,int height, bool bWindow, HWND)=0;

        // 创建一个与主画面兼容的画面
        virtual bool Create(int width, int height)=0;

        // 释放画面资源
	virtual void ShutDown()=0;

        // 创建一个与位图大小相同的画面，并加载位图
        virtual bool CreateBitmap(const char *fileName)=0;

        // 加载一个位图，从位图的指定位置(bmpX, bmpY)，加载指定的大小(width, height)
        // 到画面的指定位置(surX, surY)
	virtual bool LoadBitmap(const char *fileName, int surX, int surY, 
                int bmpX, int bmpY, int width, int height)=0;

        // 将另一个画面贴到该画面上
        virtual bool AttachSurface(const CSurface *sur, int x, int y)=0;

        // 设置画面的色彩关键字
	virtual bool SetColorKey(DWORD color)=0;

        // 返回画面尺寸
	virtual int  GetWidth() const=0;
	virtual int  GetHeight() const=0;

        // 双缓冲
        virtual bool Flip()=0;

        // 画实心矩形
        virtual bool DrawSolidRect(DWORD color, LPRECT lpRect)=0;

        // 画线
        virtual bool DrawLine(int x1, int y1, int x2, int y2, DWORD color) = 0;

protected:

};

#endif