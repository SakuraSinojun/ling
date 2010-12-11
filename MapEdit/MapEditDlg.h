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

        // 整个程序窗口的尺寸
        int m_width;
        int m_height;

        // 地图编辑区的尺寸
        int m_viewWidth;
        int m_viewHeight;

        // 当前选择图标的尺寸
        int m_curWidth;
        int m_curHeight;

        // 当前选择的图标的索引
        int m_indexChoose;

        // 是否选择了一个图标
        bool m_bChoose;

        // 当前选择的图标
        CDC m_dcChoose;

        // 显示鼠标在地图中的位置
        RECT m_rcText;

        // 保存图标类表中，每个图标的矩形区域
        RECT m_rcIcons[ICON_NUM];

        // 双缓冲
        CDC m_dcMem;

        // 地图编辑区
        CDC m_dcView;
        
        // 默认的未加载图标的样式
        CDC m_dcDef;

        // 图标信息显示空间
        CStatic m_stDis[ICON_NUM];
 
        // 当前图标列表的最后一个图标位置
        int m_curListEnd;

        // 当前所在图标列表的页
        int m_listPage;

        // 地图编辑的最小单元尺寸
        int m_mapCellSize;

        // 地图尺寸
        int m_mapWidth;
        int m_mapHeight;

        // 地图名称
        CString m_mapName;

        // 滚动条的位置
        UINT m_barPosVer;
        UINT m_barPosHor;

        // 滚动条的范围
        int m_rangeHor;
        int m_rangeVer;

        // 地图编辑区的矩形位置
        RECT m_rcView;

        // 图标区的位置
        RECT m_rcIcon;

        // 储存地图的索引
        int *m_pMap;

        // 文本分析
        Parser m_parser;
        
        // 加载的所有图标的列表
        CellInfo *m_pInfo;

        // 图标列表的大小
        int m_bmpNum;

        // 图标加载路径
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
