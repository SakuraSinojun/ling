#if !defined(AFX_NEWMAPDLG_H__B200CE18_5F5D_41A2_93DF_3418F91EE4C0__INCLUDED_)
#define AFX_NEWMAPDLG_H__B200CE18_5F5D_41A2_93DF_3418F91EE4C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewMapDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewMapDlg dialog
#include "MapEditDlg.h"

class CNewMapDlg : public CDialog
{
// Construction
public:
	CNewMapDlg(CWnd* pParent = NULL);   // standard constructor
        void GetMainDlg(CMapEditDlg *dlg);
private:
        CMapEditDlg *m_dlg;

// Dialog Data
	//{{AFX_DATA(CNewMapDlg)
	enum { IDD = IDD_DLG_NEW };
	CString	m_name;
	int	m_width;
	int	m_height;
	int	m_cellsize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewMapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewMapDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWMAPDLG_H__B200CE18_5F5D_41A2_93DF_3418F91EE4C0__INCLUDED_)
