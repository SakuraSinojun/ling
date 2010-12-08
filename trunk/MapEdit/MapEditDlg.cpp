// MapEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MapEdit.h"
#include "MapEditDlg.h"
#include "NewMapDlg.h"



#include <fstream>
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditDlg dialog


CMapEditDlg::CMapEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMapEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapEditDlg)
	DDX_Control(pDX, IDC_BAR_VERTICAL, m_barVer);
	DDX_Control(pDX, IDC_BAR_HORIZON, m_barHor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapEditDlg, CDialog)
	//{{AFX_MSG_MAP(CMapEditDlg)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_LOAD_CONFIG, OnLoadConfig)
	ON_COMMAND(ID_LOADMAP, OnLoadMap)
	ON_COMMAND(ID_SAVEMAP, OnSaveMap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditDlg message handlers

BOOL CMapEditDlg::OnInitDialog() 
{
        CDialog::OnInitDialog();

        // 窗口布局
        CRect	rcWnd;
	this->MoveWindow(0, 0, 800, 600);  
        m_barHor.MoveWindow(0, GAME_WINDOW_HEIGHT, GAME_WINDOW_WIDTH, 20);  
        m_barVer.MoveWindow(GAME_WINDOW_WIDTH, 0, 20, GAME_WINDOW_HEIGHT);

        // 编辑区
        SetRect(&m_rcView, 0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

        // 选择展示区
        SetRect(&m_rcIcon, 700, 50, 731, 450);

        // 创建静态文本框
        RECT rcStatic;
        int i=0;
        for(i=0; i<8; i++)
        {
                SetRect(&rcStatic, 690, 82 + i*50, 741, 100 + i*50); 
                m_stDis[i].Create("地图",WS_CHILD|WS_VISIBLE|SS_CENTER, rcStatic, this);
                m_stDis[i].SetWindowText("未载入");
        }
        
        // 各种DC

        CDC *dc = this->GetDC();
        HBITMAP hBitmap;

        // 双缓冲DC
        hBitmap = CreateCompatibleBitmap(dc->m_hDC, 800, 600);
        m_dcMem.CreateCompatibleDC(dc);
        m_dcMem.SelectObject(hBitmap);
        RECT rc;
        this->GetClientRect(&rc);
        m_dcMem.FillSolidRect(&rc, GetSysColor(COLOR_3DFACE));
        DeleteObject(hBitmap);

        
        // 创建DC列表
        hBitmap = CreateCompatibleBitmap(dc->m_hDC, 32, 32);

        CDC *tempdc = new CDC;
        m_dcChoose.CreateCompatibleDC(dc);
        m_dcChoose.SelectObject(hBitmap);
        DeleteObject(hBitmap);


        this->Invalidate();
        this->ReleaseDC(dc);

        m_data = NULL;
        m_bChoose = false;
        m_listPage = 0;
        m_indexChoose = 0;
    

        

        // 隐藏滚动条，如果地图大于默认屏幕尺寸，则显示滚动条，支持地图的滚动显示

        m_barVer.ShowWindow(SW_HIDE);
        m_barHor.ShowWindow(SW_HIDE);
        m_barPosHor = 0;
        m_barPosVer = 0;

        SetTimer(1, 50, NULL);


        // TODO: Add extra initialization here
        
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMapEditDlg::OnFileNew() 
{
	// TODO: Add your command handler code here

        // 创建一个新地图，new资源由对话框自行释放

        CNewMapDlg *dlg = new CNewMapDlg;
        dlg->Create(IDD_DLG_NEW);
        dlg->GetMainDlg(this);
        dlg->ShowWindow(SW_SHOW);

	
}

void CMapEditDlg::CreateNewMap(CString strName, int width, int height, int cellSize)
{
        // 此函数由创建新地图的对话框来调用，来进行地图编辑区的初始化

        m_mapWidth = width;
        m_mapHeight = height;
        m_mapCellSize = cellSize;
        m_mapName = strName;

        //HBITMAP hBitmap = CreateCompatibleBitmap(m_memdc.m_hDC, width*cellSize, height*cellSize);
        
        CDC *dc = this->GetDC();
        m_dcView.CreateCompatibleDC(dc);
        //HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), "..//MapEdit//huancai207.bmp",IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
        HBITMAP hBitmap = CreateCompatibleBitmap(dc->m_hDC, width*cellSize, height*cellSize);
        m_dcView.SelectObject(hBitmap);
        DeleteObject(hBitmap);

        m_rangeHor = width  -  GAME_WINDOW_WIDTH/cellSize;
        m_rangeVer = height - GAME_WINDOW_HEIGHT/cellSize;

        if(m_rangeHor>0)
        {
                m_barPosHor = 0;
                m_barVer.SetScrollRange(0, m_rangeHor);
                m_barVer.ShowWindow(SW_SHOW);
        }
        if(height*cellSize > 480)
        {       
                m_barPosVer = 0;
                m_barHor.SetScrollRange(0, m_rangeVer);
                m_barHor.ShowWindow(SW_SHOW);
        }

        // 初始化数据索引区
        m_data = new int[width * height];
        memset((char*)m_data, 0, sizeof(int)*width*height);
      

        
        m_dcMem.BitBlt(0, 0, 640, 480, &m_dcView, 0, 0, SRCCOPY);

        this->Invalidate();
        this->ReleaseDC(dc);


}

void CMapEditDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
        switch(nSBCode)
        {
        case SB_LINELEFT:
                m_barPosHor = max(0, m_barPosHor--);
                break;

        case SB_LINERIGHT:
                m_barPosHor = min(m_barPosHor++, (UINT)m_rangeHor);
                break;

        case SB_PAGELEFT:
                m_barPosHor = max(0, m_barPosHor--);
                break;

        case SB_PAGERIGHT:
                m_barPosHor = min(m_barPosHor++, (UINT)m_rangeVer);
                break;

        case SB_THUMBTRACK:
                m_barPosHor = nPos;
                break;

        case SB_THUMBPOSITION:
                m_barPosHor = nPos;
                break;

        default:
                break;

        }

        m_barHor.SetScrollPos(m_barPosHor);


        this->InvalidateRect(&m_rcView);
        //this->UpdateWindow();
    
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMapEditDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
        // TODO: Add your message handler code here and/or call default
        
        switch(nSBCode) 
        { 
        case SB_PAGEUP: 
                m_barPosVer = max(0, m_barPosVer--);
                break;

        case SB_PAGEDOWN: 
                m_barPosVer = min(m_barPosVer++, (UINT)m_rangeVer);
                break; 

        case SB_LINEUP: 
                m_barPosVer = max(0, m_barPosVer--);
                break; 
                
        case SB_LINEDOWN: 
                m_barPosVer = min(m_barPosVer++, (UINT)m_rangeVer);
                break; 
                
        case SB_THUMBTRACK:
                m_barPosVer = nPos; 
                break;
                
        case SB_THUMBPOSITION: 
                m_barPosVer = nPos; 
                break; 
        } 

       
    
        m_barVer.SetScrollPos(m_barPosVer);

        this->InvalidateRect(&m_rcView);
        //this->UpdateWindow();

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMapEditDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

        if(PtInRect(&m_rcView, point))
        {
                int x = m_mapCellSize * (m_barPosHor + (int)(point.x/m_mapCellSize));
                int y = m_mapCellSize * (m_barPosVer + (int)(point.y/m_mapCellSize));

                CString str;
                str.Format("[%d], [%d]", x, y);

                RECT rc = {0, 500, 100, 520};

                CDC *dc = this->GetDC();
                dc->FillSolidRect(&rc, GetSysColor(COLOR_3DFACE));
                dc->DrawText(str, &rc, DT_LEFT);
                this->ReleaseDC(dc);

        }

        if(m_bChoose)
        {
               
                    //this->Invalidate();
                this->InvalidateRect(&m_rcView,TRUE);

        }
        
        
	CDialog::OnMouseMove(nFlags, point);

}



void CMapEditDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
        if(PtInRect(&m_rcView, point))
        {
                // 在地图编辑器
                if(!m_bChoose)
                {
                        return;
                }

                int xSeg = m_barPosHor + (int)(point.x/m_mapCellSize);
                int ySeg = m_barPosVer + (int)(point.y/m_mapCellSize);
           
                int x = m_mapCellSize * xSeg;
                int y = m_mapCellSize * ySeg;

                
                m_dcView.BitBlt(x, y, m_mapCellSize, m_mapCellSize, &m_dcChoose, 0, 0, SRCCOPY);

                m_data[xSeg + ySeg * m_mapWidth] = m_indexChoose;


                //this->Invalidate();

                
             
        }
        else if(PtInRect(&m_rcIcon, point))
        {
                // 点中了地图图标区域

                int xDiff = point.x - m_rcIcon.left;

                if(xDiff <9 || xDiff > 41)
                {
                        return;
                }

                int yDiff = point.y - m_rcIcon.top;

                int y = yDiff % 50;

                if(y > 32)
                {
                        return;
                }

                int n = (int)(yDiff / 50);

               

                if(n > m_curListEnd)
                {
                        return;
                }

                m_bChoose = true;
                m_indexChoose = m_pInfo[n].index;

                m_dcChoose.BitBlt(0, 0, 32, 32, m_pInfo[n].dc, 0, 0, SRCCOPY);               
        }

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CMapEditDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
        

	m_dcMem.BitBlt(0, 0, 640, 480, &m_dcView, m_barPosHor*m_mapCellSize, m_barPosVer*m_mapCellSize, SRCCOPY);
        if(m_bChoose)
        {
                POINT pt;
                GetCursorPos(&pt);
                this->ScreenToClient(&pt); 
                if(PtInRect(&m_rcView, pt))
                {
                        m_dcMem.BitBlt(pt.x-16, pt.y-16, 800, 600, &m_dcChoose, 0, 0, SRCCOPY);
                }
        }

        if(!pDC->IsPrinting())
        {
                pDC->BitBlt(0, 0, 800, 600, &m_dcMem, 0, 0, SRCCOPY);
        }
        return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CMapEditDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bChoose = FALSE;
	CDialog::OnRButtonDown(nFlags, point);
}

void CMapEditDlg::ShowMap(CDC &dcMemory)
{
        
}

void CMapEditDlg::OnLoadConfig() 
{
        CFileDialog dlg(TRUE, "*.cfg",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"地图配置文件(*.cfg) | *.cfg"); 
        if(IDOK == dlg.DoModal())
        {
                CString path = dlg.GetPathName();

                if(!m_parser.Open((LPTSTR)(LPCTSTR)path))
                {
                        MessageBox("文件打开失败");
                        return;
                }

                
                char *s = (LPTSTR)(LPCTSTR)path;
                for(int i=path.GetLength(); i>0; i--)
                {
                        if(s[i] == '\\')
                        {
                                s[i] = 0;
                                break;
                        }
                }

                m_bmpPath = s;
                
                
                m_parser.SetComment(';');
                
                if(!m_parser.GetLine(PARSER_STRIP_EMPTY_LINES|PARSER_STRIP_COMMENTS))
                {
                        MessageBox("文件不完整");
                }
                
                if(!m_parser.PatternMath(m_parser.strLine.c_str(), "['<NUM>']"))
                {       
                        MessageBox("文件格式不正确");
                        return;
                }
                
                if(!m_parser.GetLine(PARSER_STRIP_EMPTY_LINES|PARSER_STRIP_COMMENTS))
                {
                        MessageBox("文件不完整");
                        return;
                }

                if(!m_parser.PatternMath(m_parser.strLine.c_str(), "[i]"))
                {       
                        MessageBox("文件格式不正确");
                        return;
                }

                m_bmpNum = m_parser.vints[0];
                m_pInfo = new MapInfo[m_bmpNum];

                if(!m_parser.GetLine(PARSER_STRIP_EMPTY_LINES|PARSER_STRIP_COMMENTS))
                {
                        MessageBox("文件不完整");
                        return;
                }

                if(!m_parser.PatternMath(m_parser.strLine.c_str(), "['<INFO>']"))
                {       
                        MessageBox("文件格式不正确");
                        return;
                }

           
                for(i=0; i<m_bmpNum; i++)
                {                  
                        m_pInfo[i].name     = m_parser.GetString();
                        m_pInfo[i].index    = m_parser.GetInt();   
                        m_pInfo[i].fileName = m_parser.GetString();

                }

                m_parser.Close();

                LoadRes();

        }
	// TODO: Add your command handler code here
	
}

void CMapEditDlg::LoadRes()
{
     

       int i=0;
       CDC *dc = this->GetDC();

       for(i=0; i<m_bmpNum; i++)
       {
                CString path = m_bmpPath +'\\'+ m_pInfo[i].fileName;
                //MessageBox(path);

                m_pInfo[i].hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
                        (LPTSTR)(LPCTSTR)path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

                if(NULL == m_pInfo[i].hBitmap)
                {
                        MessageBox("加载位图失败");
                }
                m_pInfo[i].bLoaded = TRUE;

                m_pInfo[i].dc = new CDC;
                m_pInfo[i].dc->CreateCompatibleDC(dc);
                m_pInfo[i].dc->SelectObject(m_pInfo[i].hBitmap);
       }

       int end = 8;
       if(m_bmpNum < end)
               end = m_bmpNum;

       m_curListEnd = end;


       for(i=0; i<end; i++)
       {
                m_dcMem.BitBlt(700, 50 + i*50, 32, 32, m_pInfo[i].dc, 0, 0, SRCCOPY);
                m_stDis[i].SetWindowText((LPTSTR)(LPCTSTR)m_pInfo[i].name);
       }

       this->Invalidate();

       this->ReleaseDC(dc);


}


void CMapEditDlg::OnLoadMap() 
{
	// TODO: Add your command handler code here
        CFileDialog dlg(TRUE, "*.map",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"地图文件(*.map) | *.map"); 
        if(IDOK == dlg.DoModal())
        {
                
        }
	
}

void CMapEditDlg::OnSaveMap() 
{
         CFileDialog dlg(FALSE, "*.map",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"地图文件(*.map) | *.map"); 
        if(IDOK == dlg.DoModal())
        {
                
        }
	// TODO: Add your command handler code here
	
}
