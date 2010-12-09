#if !defined(AFX_MAPEDITDLG_H__69E85544_0EA6_40DB_976E_B53B23ED6101__INCLUDED_)
#define AFX_MAPEDITDLG_H__69E85544_0EA6_40DB_976E_B53B23ED6101__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapEditDlg dialog
#define GAME_WINDOW_WIDTH 640
#define GAME_WINDOW_HEIGHT 480

#include "../src/Script/map.h"
#include "../src/common/parser.h"
#include <vector>
#include <map>
using namespace std;

struct MapInfo
{
        int index;
        CString name;
        CString fileName;
        bool bLoaded;
        HBITMAP hBitmap;
        CDC *dc;
};

class CMapEditDlg : public CDialog
{
// Construction
public:
	CMapEditDlg(CWnd* pParent = NULL);   // standard constructor

        void CreateNewMap(CString strName, int widht, int height, int cellSize);

        void LoadRes();

        void ShowMap(CDC &dcMemory);

private:
        CDC m_dcMem;
        CDC m_dcView;
        CDC m_dcChoose;
        CDC m_dcDef;
        int m_indexChoose;

    
   

        CStatic m_stDis[8];

 
        int m_curListEnd;
        int m_listPage;

  

        int m_mapCellSize;
        int m_mapWidth;
        int m_mapHeight;
        CString m_mapName;

        UINT m_barPosVer;
        UINT m_barPosHor;
        int m_rangeHor;
        int m_rangeVer;

        RECT m_rcView;
        RECT m_rcIcon;
        bool m_bChoose;

        int *m_data;

        Parser m_parser;
        int m_bmpNum;
        MapInfo *m_pInfo;
        CString m_bmpPath;

        

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
