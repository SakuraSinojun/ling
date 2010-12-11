#if !defined(AFX_MAPEDITDLG_H__69E85544_0EA6_40DB_976E_B53B23ED6101__INCLUDED_)
#define AFX_MAPEDITDLG_H__69E85544_0EA6_40DB_976E_B53B23ED6101__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapEditDlg dialog

#include "../src/Script/map.h"
#include "../src/common/parser.h"

#include <vector>
#include <map>
using namespace std;

#define ICON_NUM 6
struct CellInfo
{
        int index;
        CString name;
        CString fileName;
        bool bLoaded;
        HBITMAP hBitmap;
        CDC *dc;
        int width;
        int height;
};

class CMapEditDlg : public CDialog
{
// Construction
public:
	CMapEditDlg(CWnd* pParent = NULL);   // standard constructor

        void CreateNewMap(CString strName, int widht, int height, int cellSize);

        void LoadRes();

        void ShowMap(CDC &dcMemory);

        void InitList();

        void UpdateIconList();

        void ClientToMap(POINT &pt);

        HBITMAP CopyDCToBitmap(HDC hSrcDc, LPRECT lpRect);

        BOOL SaveBmp(HBITMAP hBitmap, CString FileName);

        BOOL SaveMyBmp(HBITMAP hBitmap, BMPINFO &bmp, int mapIndex, const char *name);

        void ReleaseCellInfo();

private:

        // �������򴰿ڵĳߴ�
        int m_width;
        int m_height;

        // ��ͼ�༭���ĳߴ�
        int m_viewWidth;
        int m_viewHeight;

        // ��ǰѡ��ͼ��ĳߴ�
        int m_curWidth;
        int m_curHeight;

        // ��ǰѡ���ͼ�������
        int m_indexChoose;

        // �Ƿ�ѡ����һ��ͼ��
        bool m_bChoose;

        // ��ǰѡ���ͼ��
        CDC m_dcChoose;

        // ��ʾ����ڵ�ͼ�е�λ��
        RECT m_rcText;

        // ����ͼ������У�ÿ��ͼ��ľ�������
        RECT m_rcIcons[ICON_NUM];

        // ˫����
        CDC m_dcMem;

        // ��ͼ�༭��
        CDC m_dcView;
        
        // Ĭ�ϵ�δ����ͼ�����ʽ
        CDC m_dcDef;

        // ͼ����Ϣ��ʾ�ռ�
        CStatic m_stDis[ICON_NUM];
 
        // ��ǰͼ���б�����һ��ͼ��λ��
        int m_curListEnd;

        // ��ǰ����ͼ���б��ҳ
        int m_listPage;

        // ��ͼ�༭����С��Ԫ�ߴ�
        int m_mapCellSize;

        // ��ͼ�ߴ�
        int m_mapWidth;
        int m_mapHeight;

        // ��ͼ����
        CString m_mapName;

        // ��������λ��
        UINT m_barPosVer;
        UINT m_barPosHor;

        // �������ķ�Χ
        int m_rangeHor;
        int m_rangeVer;

        // ��ͼ�༭���ľ���λ��
        RECT m_rcView;

        // ͼ������λ��
        RECT m_rcIcon;

        // �����ͼ������
        int *m_pMap;

        // �ı�����
        Parser m_parser;
        
        // ���ص�����ͼ����б�
        CellInfo *m_pInfo;

        // ͼ���б�Ĵ�С
        int m_bmpNum;

        // ͼ�����·��
        CString m_bmpPath;

        CMap m_map;

// Dialog Data
	//{{AFX_DATA(CMapEditDlg)
	enum { IDD = IDD_DLG_MAPEDIT };
	CComboBox	m_combo;
	CScrollBar	m_barVer;
	CScrollBar	m_barHor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMapEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnFileNew();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLoadConfig();
	afx_msg void OnLoadMap();
	afx_msg void OnSaveMap();
	afx_msg void OnSelchangeCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITDLG_H__69E85544_0EA6_40DB_976E_B53B23ED6101__INCLUDED_)
