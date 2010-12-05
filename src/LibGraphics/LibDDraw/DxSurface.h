#ifndef DXSURFACE_H_WD101204
#define DXSURFACE_H_WD101204

#define DEF_BPP 16	// Ĭ��ɫ�ʸ�ʽ

#include "../Surface.h"
#include <ddraw.h>
class CSurface;

class CDxSurface :public CSurface
{
        // ����ָ����

	class AutoSurPtr
	{
		friend class CDxSurface;
		AutoSurPtr(LPDIRECTDRAWSURFACE7 lpSur):m_use(1),m_lpddSur(lpSur){}
		~AutoSurPtr()
		{
			if(m_lpddSur)
			{
				m_lpddSur->Release();
				m_lpddSur = NULL;
			}
		}
		LPDIRECTDRAWSURFACE7 m_lpddSur;
		
		// ����
		
		int m_use;
                
        };
public:
        // ���ֹ��캯���͸�ֵ����
        CDxSurface():m_ptr(new AutoSurPtr(NULL)){}
	//CDxSurface(LPDIRECTDRAWSURFACE7 lpSur = NULL):m_ptr(new AutoSurPtr(lpSur)){}

        // ����surface��Ϣ
        int GetWidth() const {return m_width;}
        int GetHeight() const {return m_height;}

        // surface���趨
	bool CreateMainWnd(int width,int height, bool bWindow = true, HWND = NULL);
	bool Create(int width, int height);
	void ShutDown();
        bool SetColorKey(DWORD color);

        // �򵥵Ļ�ͼ
        bool DrawLine(int x1, int y1, int x2, int y2, DWORD color);
        bool DrawSolidRect(DWORD color, LPRECT lpRect);

        // ���渴��
        bool AttachSurface(const CSurface *sur, int x, int y);
        bool Flip();

        // λͼ���
        bool LoadBitmap(const char *fileName);

protected:
        CDxSurface(const CDxSurface &orig);
	CDxSurface& operator= (const CDxSurface& rhs);
	~CDxSurface();

        // ��ʼ����Ա����
	void Init();

        // ������
        void WriteError(const char *error);

        // ����ָ����Ϣ
	inline LPDIRECTDRAWSURFACE7& GetPtr();
	inline const LPDIRECTDRAWSURFACE7& GetPtr() const;
	inline void SetPtr(LPDIRECTDRAWSURFACE7 lpSur);

protected:

	bool m_bMainWnd;        // ���浱ǰ�����Ƿ�Ϊ������
	AutoSurPtr *m_ptr;      // ָ��ǰ����
	int m_width;            // ����ߴ�
	int m_height;             
	int m_pixelFormat;      // �������ظ�ʽ

protected:
                                  
	static LPDIRECTDRAW7 s_lpdd;                    // DDRAW��Ҫ��Ϣ
	static LPDIRECTDRAWSURFACE7 s_lpdsPrimary;      // ˫�������ʾ����
	static LPDIRECTDRAWCLIPPER  s_lpddClipperWin;   // WINDOWS���ڵļ�������
	static HWND s_hWnd;                             // �����Ĵ��ھ��
        static bool s_bWindow;                          // SURFACE�Ƿ��ڴ���ģʽ������
	

};


#endif
