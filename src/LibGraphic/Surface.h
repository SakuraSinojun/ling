#ifndef SURFACE_H_011204
#define SURFACE_H_011204

class CSurface
{
public:
        CSurface(){}
        ~CSurface(){}
	
        // ����һ�������棬�������������������ڵ�ǰ��
	virtual bool CreateMainWnd(int width,int height, bool bWindow, HWND)=0;

        // ����һ������������ݵĻ���
        virtual bool Create(int width, int height)=0;

        // �ͷŻ�����Դ
	virtual void ShutDown()=0;

        // ����һ����λͼ��С��ͬ�Ļ��棬������λͼ
        virtual bool CreateBitmap(const char *fileName)=0;

        // ����һ��λͼ����λͼ��ָ��λ��(bmpX, bmpY)������ָ���Ĵ�С(width, height)
        // �������ָ��λ��(surX, surY)
	virtual bool LoadBitmap(const char *fileName, int surX, int surY, 
                int bmpX, int bmpY, int width, int height)=0;

        // ����һ�����������û�����
        virtual bool AttachSurface(const CSurface *sur, int x, int y)=0;

        // ���û����ɫ�ʹؼ���
	virtual bool SetColorKey(DWORD color)=0;

        // ���ػ���ߴ�
	virtual int  GetWidth() const=0;
	virtual int  GetHeight() const=0;

        // ˫����
        virtual bool Flip()=0;

        // ��ʵ�ľ���
        virtual bool DrawSolidRect(DWORD color, LPRECT lpRect)=0;

        // ����
        virtual bool DrawLine(int x1, int y1, int x2, int y2, DWORD color) = 0;

protected:

};

#endif