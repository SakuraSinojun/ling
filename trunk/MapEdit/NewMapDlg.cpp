// NewMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MapEdit.h"
#include "NewMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewMapDlg dialog


CNewMapDlg::CNewMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewMapDlg)
	m_name = _T("NewMap");
	m_width = 50;
	m_height = 50;
	m_cellsize = 32;
	//}}AFX_DATA_INIT
}


void CNewMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewMapDlg)
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_width);
	DDV_MinMaxInt(pDX, m_width, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_height);
	DDV_MinMaxInt(pDX, m_height, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_CELLSIZE, m_cellsize);
	DDV_MinMaxInt(pDX, m_cellsize, 0, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewMapDlg, CDialog)
	//{{AFX_MSG_MAP(CNewMapDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewMapDlg message handlers

void CNewMapDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

        if(NULL == m_dlg)
        {
                MessageBox("没有传递主对话框");
        }

        m_dlg->CreateNewMap(m_name, m_width, m_height, m_cellsize);

	CDialog::OnOK();
}

void CNewMapDlg::GetMainDlg(CMapEditDlg *dlg)
{
        m_dlg = dlg;
}

void CNewMapDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();

        // 释放本身
        delete this;
}
