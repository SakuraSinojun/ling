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
	DDX_Control(pDX, IDC_COMBO, m_combo);
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
	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditDlg message handlers

BOOL CMapEditDlg::OnInitDialog() 
{
        CDialog::OnInitDialog();

        m_width = 1000;
        m_height = 700;

        m_viewWidth = 800;
        m_viewHeight = 600;

        m_pMap = NULL;
        m_bChoose = false;
        m_listPage = 0;
        m_indexChoose = 0;
        m_pInfo = NULL;

        
        m_barPosHor = 0;
        m_barPosVer = 0;

        // 窗口布局

        // 主窗口
	this->MoveWindow(0, 0, m_width, m_height);  

        // 滚动条
        m_barHor.MoveWindow(0, m_viewHeight, m_viewWidth, 20);  
        m_barVer.MoveWindow(m_viewWidth, 0, 20, m_viewHeight);

        // 隐藏滚动条，如果地图大于默认屏幕尺寸，则显示滚动条，支持地图的滚动显示
        m_barVer.ShowWindow(SW_HIDE);
        m_barHor.ShowWindow(SW_HIDE);

         // 组合框
        m_combo.ShowWindow(SW_HIDE);
        m_combo.MoveWindow(m_width - 120, 5, 100, 20);

        // 编辑区
        SetRect(&m_rcView, 0, 0, m_viewWidth, m_viewHeight);

        // 鼠标文字区
        SetRect(&m_rcText, 0, m_height-80, 300, m_height-60);

         // 选择展示列表区
        SetRect(&m_rcIcon, m_width-150, 50, m_width - 22, 680);

        CDC *dc = this->GetDC();

        // 双缓冲DC
        HBITMAP hBitmap = CreateCompatibleBitmap(dc->m_hDC, m_width, m_height);
        m_dcMem.CreateCompatibleDC(dc);
        m_dcMem.SelectObject(hBitmap);
        RECT rc;
        this->GetClientRect(&rc);
        m_dcMem.FillSolidRect(&rc, GetSysColor(COLOR_3DFACE));
        DeleteObject(hBitmap);

        m_dcChoose.CreateCompatibleDC(dc);
        m_dcDef.CreateCompatibleDC(dc);
        m_dcView.CreateCompatibleDC(dc);


        // 创建静态文本框

        RECT rcStatic;
        int i=0;
        for(i=0; i<ICON_NUM; i++)
        {
                SetRect(&m_rcIcons[i], m_width - 124, 56  + i*90, m_width - 58, 120 + i*90);
                SetRect(&rcStatic,     m_width - 160 ,125 + i*90, m_width - 20, 140 + i*90); 
                m_stDis[i].Create("地图",WS_CHILD|WS_VISIBLE|SS_CENTER, rcStatic, this);
        }

        
  

        // 先默认加载一个地图
        CreateNewMap("NewMap", 50, 50, 32);
        InitList();
        

        this->Invalidate();
        this->ReleaseDC(dc);

        


        // TODO: Add extra initialization here
        
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMapEditDlg::ReleaseCellInfo()
{
        if(m_pInfo)
        {
                for(int i=0; i<m_bmpNum; i++)
                {
                        DeleteObject(m_pInfo[i].hBitmap);
                        if(m_pInfo[i].dc)
                        {
                                ReleaseDC(m_pInfo[i].dc);
                                delete m_pInfo[i].dc;
                        }             
                }

                delete[] m_pInfo;
                m_bmpNum = 0;
                m_curListEnd = 0;
                m_listPage = 0;

                InitList();
        }
}

void CMapEditDlg::InitList()
{
        HBITMAP hBitmap = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDM_BLANK));
        m_dcDef.SelectObject(hBitmap);
        DeleteObject(hBitmap);

        for(int i=0; i<ICON_NUM; i++)
        {
                m_dcMem.BitBlt(m_rcIcons[i].left, m_rcIcons[i].top, 
                        m_rcIcons[i].right - m_rcIcons[i].left, 
                        m_rcIcons[i].bottom - m_rcIcons[i].top, 
                        &m_dcDef, 0, 0, SRCCOPY); 

              
                m_stDis[i].SetWindowText("未载入");

        }

   
        
        // 创建默认空白图标
}

void CMapEditDlg::UpdateIconList()
{
        // 设置当前列表的最后一个图标
        m_curListEnd =  m_bmpNum - ICON_NUM * m_listPage;
        if(m_curListEnd > ICON_NUM)
                m_curListEnd = ICON_NUM;
        
        // 设置当前所显示的列表
        
        
        // 绘制当前列表
        int i=0;

        for(i=0; i<ICON_NUM; i++)
        {
                m_dcMem.BitBlt(m_rcIcons[i].left, m_rcIcons[i].top, 
                        m_rcIcons[i].right - m_rcIcons[i].left, 
                        m_rcIcons[i].bottom - m_rcIcons[i].top, 
                        &m_dcDef, 0, 0, SRCCOPY); 
                m_stDis[i].SetWindowText("未载入");
        }

        for(i=0; i<m_curListEnd; i++)
        {
                int index = i + ICON_NUM * m_listPage;

                int width = m_pInfo[index].width;
                int height = m_pInfo[index].height;
                
                int bigger = (width > height) ? width : height;
                float scale = 64.0 / (float)bigger;
                
                int left = m_rcIcons[i].left;
                int top = m_rcIcons[i].top;
                
                int newWidth = m_rcIcons[i].right - left;
                
                int newHeight = m_rcIcons[i].bottom - top;
                
                if(scale < 1.0f)
                {
                        // 需要缩放
                        newWidth  = (int)(width*scale);
                        newHeight = (int)(height*scale);
                        
                        if(newWidth < 63)
                        {
                                left = left + 32 - (int)(32.0f* newWidth / 64.0f);
                        }
                        if(newHeight < 63)
                        {
                                top = top + 32 - (int)(32.0f * newHeight / 64.0f);
                        }
                }
                else
                {
                        left = left + 32 - (int)(width / 64.0f * 32.0f);
                        top  = top  + 32 - (int)(height/64.0f * 32.0f);
                }
                m_dcMem.BitBlt(left, top, 
                        newWidth, 
                        newHeight, 
                        m_pInfo[index].dc, 0, 0, SRCCOPY);
                
                m_stDis[i].SetWindowText((LPTSTR)(LPCTSTR)m_pInfo[index].name);
        }
        
        

       this->InvalidateRect(&m_rcIcon);
        
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

        CDC *dc = this->GetDC();
        HBITMAP hBitmap = CreateCompatibleBitmap(dc->m_hDC, width*cellSize, height*cellSize);
        m_dcView.SelectObject(hBitmap);
        DeleteObject(hBitmap);
        this->ReleaseDC(dc);

        m_rangeHor = width  -  (int)(m_viewWidth/cellSize  + 0.5f);
        m_rangeVer = height -  (int)(m_viewHeight/cellSize + 0.5f);

        if(m_rangeHor>0)
        {
                m_barPosHor = 0;
                m_barHor.SetScrollRange(0, m_rangeHor);
                m_barHor.ShowWindow(SW_SHOW);
        }
        if(m_rangeVer>0)
        {       
                m_barPosVer = 0;
                m_barVer.SetScrollRange(0, m_rangeVer-1);
                m_barVer.ShowWindow(SW_SHOW);
        }

        // 初始化数据索引区
        if(m_pMap)
        {
                delete[] m_pMap;
                m_pMap = NULL;
        }

        m_pMap = new DWORD[width * height];
        memset((char*)m_pMap, 0, sizeof(int)*width*height);
      

        
        m_dcMem.BitBlt(0, 0, m_viewWidth, m_viewHeight, &m_dcView, 0, 0, SRCCOPY);

        this->Invalidate();
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
void CMapEditDlg::ClientToMap(POINT &pt)
{
        pt.x =  (m_barPosHor + (int)(pt.x/m_mapCellSize));
        pt.y =  (m_barPosVer + (int)(pt.y/m_mapCellSize));
}
void CMapEditDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

        if(PtInRect(&m_rcView, point))
        {
                //int x = m_mapCellSize * (m_barPosHor + (int)(point.x/m_mapCellSize));
                //int y = m_mapCellSize * (m_barPosVer + (int)(point.y/m_mapCellSize));

                ClientToMap(point);

                CString str;
                str.Format("地图坐标：[%d], [%d]", point.x, point.y);

                CDC *dc = this->GetDC();
                dc->FillSolidRect(&m_rcText, GetSysColor(COLOR_3DFACE));
                dc->DrawText(str, &m_rcText, DT_LEFT);
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

                ClientToMap(point);

                m_dcView.BitBlt(point.x * m_mapCellSize, point.y * m_mapCellSize, 
                        m_curWidth, m_curHeight, &m_dcChoose, 0, 0, SRCCOPY);

                m_pMap[point.x + point.y * m_mapWidth] = m_indexChoose;
        }
        else if(PtInRect(&m_rcIcon, point))
        {
                // 点中了地图图标区域

                int i=0;
                for(i=0; i<ICON_NUM; i++)
                {
                        if(PtInRect(&m_rcIcons[i], point))
                        {
                                break;
                        }
                }

                if(i == ICON_NUM)
                {
                        // 没有选中相应地图
                        return;
                }

                /*
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
                */

               

                if(i > m_curListEnd)
                {
                        // 此区域空白

                        return;
                }

                m_bChoose = true;

                CellInfo *curCell = &m_pInfo[i + ICON_NUM * m_listPage];

                m_curWidth = curCell->width;
                m_curHeight = curCell->height;

                // 创建临时DC(跟随鼠标)
                CDC *dc = this->GetDC();
                HBITMAP hBitmap = CreateCompatibleBitmap(dc->m_hDC, m_curWidth, m_curHeight);
                m_dcChoose.SelectObject(hBitmap);
                DeleteObject(hBitmap);
                this->ReleaseDC(dc);

                m_indexChoose = curCell->index;
                
                m_dcChoose.BitBlt(0, 0, m_curWidth, m_curHeight, curCell->dc, 0, 0, SRCCOPY);               
        }
        
        CDialog::OnLButtonDown(nFlags, point);
}

BOOL CMapEditDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
        

        // 先把地图画在缓冲上
	m_dcMem.BitBlt(0, 0, m_viewWidth, m_viewHeight, &m_dcView, m_barPosHor*m_mapCellSize, m_barPosVer*m_mapCellSize, SRCCOPY);

        if(m_bChoose)
        {
                // 选择了某个图标

                POINT pt;
                GetCursorPos(&pt);
                this->ScreenToClient(&pt); 
                if(PtInRect(&m_rcView, pt))
                {
                        // 对矩形进行剪裁
                        // 如果边界超过了左边或上边的区域，则x,y值为零
                        // 如果边界超过了右边的区域，则在显示时，将它剪裁掉

                        int width = m_curWidth;
                        int height = m_curHeight;

                        int x = m_mapCellSize*(int)(pt.x / m_mapCellSize);
                        int y = m_mapCellSize*(int)(pt.y / m_mapCellSize);
                       
                        if(x + m_curWidth > m_rcView.right)
                        {
                                width = m_rcView.right - x;
                        }

                        if(y + m_curHeight > m_rcView.bottom)
                        {
                                height = m_rcView.bottom - y;
                        }
                     
                        // 将图标的临时位置显示先缓冲画面上

                        m_dcMem.BitBlt(x, y, width, height, &m_dcChoose, 0, 0, SRCCOPY);
                }
        }

        if(!pDC->IsPrinting())
        {
                pDC->BitBlt(0, 0, m_width, m_height, &m_dcMem, 0, 0, SRCCOPY);
        }
        return TRUE;
}

void CMapEditDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bChoose = FALSE;
        this->InvalidateRect(&m_rcView);
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

                if(m_pInfo)
                {
                        delete[] m_pInfo;
                }

                m_pInfo = new CellInfo[m_bmpNum];

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
                        m_pInfo[i].width    = m_parser.GetInt();
                        m_pInfo[i].height   = m_parser.GetInt();
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
     

       int i=0; // 索引
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

       if(m_bmpNum >ICON_NUM)
       {
               // 显示组合框

               int page = (int)(m_bmpNum/ICON_NUM);
               CString temp;
               for(i=0; i<=page; i++)
               {
                        temp.Format("列表 %d:", i+1);
                        m_combo.AddString((LPTSTR)(LPCTSTR)temp);
               }
               m_combo.SetCurSel(0);
               m_combo.ShowWindow(SW_SHOW);
       }

       m_listPage = 0;
       
       this->ReleaseDC(dc);

       this->UpdateIconList();
       
       


}


void CMapEditDlg::OnLoadMap() 
{
        // TODO: Add your command handler code here
        CFileDialog dlg(TRUE, "*.map",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"地图文件(*.map) | *.map"); 
        if(IDOK == dlg.DoModal())
        {
                CString strPath = dlg.GetPathName();


                if(!m_map.LoadMapFile((LPTSTR)(LPCTSTR)strPath))
                {
                        MessageBox("加载文件失败");
                }
                
                ReleaseCellInfo();
                
                m_bmpNum = m_map.GetSegInfo().segNum;
                
                m_pInfo = new CellInfo[m_bmpNum];

                CDC *dc = this->GetDC();
                
                
                for(int i=0; i<m_bmpNum; i++)
                {
                        m_pInfo[i].width = m_map.GetSegInfo().heads[i].width;
                        m_pInfo[i].height = m_map.GetSegInfo().heads[i].height;
                        m_pInfo[i].index = m_map.GetSegInfo().heads[i].index;
                        m_pInfo[i].name = m_map.GetSegInfo().heads[i].name;


                        HBITMAP hBitmap = GetBmpFromBuffer(m_map.GetSegInfo().buffer[i]);

                        m_pInfo[i].dc = new CDC;

                        m_pInfo[i].dc->CreateCompatibleDC(dc);
                        m_pInfo[i].dc->SelectObject(&hBitmap);
                        
                        // BITMAPINFO bmi;
                        
                     
                        /*
                        
                        BITMAPINFOHEADER         bmih;
                        BYTE                     *pBits;
                        HBITMAP                  hBitmap;

                        bmih.biSize                  = sizeof (BITMAPINFOHEADER);
                        bmih.biWidth                 = m_pInfo[i].width;
                        bmih.biHeight                = m_pInfo[i].height;
                        bmih.biPlanes                = 1;
                        bmih.biBitCount              = 24;
                        bmih.biCompression           = BI_RGB;
                        bmih.biSizeImage             = m_map.GetBitmap(i)->header.byteNum;
                        bmih.biXPelsPerMeter         = 0;
                        bmih.biYPelsPerMeter         = 0;
                        bmih.biClrUsed               = 0;
                        bmih.biClrImportant          = 0;
                        
                        // bmi.bmiHeader = bmih;
                        // bmi.bmiColors = m_map.GetBitmap(i)->buffer;
                        


                        HANDLE hPal = GetStockObject(DEFAULT_PALETTE); 
                        HDC hDC;
                        HANDLE hOldPal;
                        if (hPal) 
                        { 
                                hDC = ::GetDC(NULL); 
                                hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
                                RealizePalette(hDC); 
                        }
                        
                        // 获取该调色板下新的像素值 
                        
                        
                        
                        hBitmap = CreateDIBitmap(hDC, &bmih, 0,NULL, NULL, DIB_PAL_COLORS);
                        
                        
                        
                        //hBitmap = CreateDIBSection (NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS, (void**)&pBits, NULL, 0) ;
                        
                        int k = SetDIBits(hDC, hBitmap, 0, m_pInfo[i].height, (void*)m_map.GetBitmap(i)->buffer, (BITMAPINFO *)&bmih, DIB_PAL_COLORS);
                        
                        //恢复调色板  
                        if (hOldPal) 
                        { 
                                ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
                                RealizePalette(hDC); 
                                ::ReleaseDC(NULL, hDC); 
                        }
                        
                        
                        if(NULL == hBitmap)
                        {
                                MessageBox("创建位图失败");
                        }

                        m_pInfo[i].hBitmap = hBitmap;
                        
                          //memcpy((char*)pBits, (char*)m_map.GetBitmap(i)->buffer, m_map.GetBitmap(i)->header.byteNum);
                        
                        CBitmap bmp;
                        

                        bmp.CreateCompatibleBitmap(&m_dcMem, m_pInfo[i].width, m_pInfo[i].height);

                        BITMAP bm;

                        bmp.GetObject(sizeof(BITMAP), &bm);

                        bmp.SetBitmapBits(m_map.GetBitmap(i)->header.byteNum, m_map.GetBitmap(i)->buffer);

                        m_pInfo[i].dc = new CDC;

                        m_pInfo[i].dc->CreateCompatibleDC(dc);
                        m_pInfo[i].dc->SelectObject(&bmp);  
                        */
                }
                
                   
                ReleaseDC(dc);
      

                UpdateIconList();
        }
	
}

HBITMAP CMapEditDlg::GetBmpFromBuffer(char *buffer)
{
        HBITMAP                hBmp;
        LPSTR                hDIB,lpBuffer = buffer;
        LPVOID                lpDIBBits;
        BITMAPFILEHEADER            bmfHeader;
        DWORD                bmfHeaderLen;
        
        bmfHeaderLen = sizeof(bmfHeader);
        strncpy((LPSTR)&bmfHeader,(LPSTR)lpBuffer,bmfHeaderLen);
        if (bmfHeader.bfType != (*(WORD*)"BM")) return NULL;
        hDIB = lpBuffer + bmfHeaderLen;
        BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
        BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
        
        lpDIBBits=(lpBuffer)+((BITMAPFILEHEADER *)lpBuffer)->bfOffBits;
        CClientDC dc(this);
        hBmp = CreateDIBitmap(dc.m_hDC,&bmiHeader,CBM_INIT,lpDIBBits,&bmInfo,DIB_RGB_COLORS);
        return hBmp;
        
}
void CMapEditDlg::OnSaveMap() 
{
         CFileDialog dlg(FALSE, "*.map",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"地图文件(*.map) | *.map"); 
        if(IDOK == dlg.DoModal())
        {
                CString strPath = dlg.GetPathName();

                /*
                char *buffer = NULL;
                
                SEGMENTHEAD head;
                SaveMyBmp(m_pInfo[1].hBitmap, head, buffer);

                HBITMAP hBitmap = GetBmpFromBuffer(buffer);

                HBITMAP old = (HBITMAP)m_pInfo[0].dc->SelectObject(hBitmap);
                m_pInfo[0].width = 128;
                m_pInfo[0].height = 32;
               
                DeleteObject(old);

                this->Invalidate();

                 */
                



                //this->SaveBmp(CopyDCToBitmap(m_dcMem.m_hDC, &m_rcView), strPath);

                
                // temp保存当前地图含有的元素的索引

                int *temp = new int[m_mapWidth * m_mapHeight];
                memset((char*)temp, 0, m_mapWidth * m_mapHeight * sizeof(int));
                
                temp[0] = m_pMap[0];
                int t=1;        // 元素个数
                
                // 遍历所有的m_pMap数据，并挑选出所有的不重复的索引
                int i=0, j=0;

                for(i=1; i<m_mapWidth*m_mapHeight; i++)
                {
                        for(j=0; j<t; j++)
                        {
                                if(m_pMap[i] == temp[j])
                                {
                                        // 已经存在了，不存
                                        break;
                                }
                        }

                        // 没有相同的
                        if(j == t)
                        {
                                temp[t] = m_pMap[i];
                                t++;
                        }
                }

                // 倒退一格

                t--;

                

                SEGMENTHEAD *head = new SEGMENTHEAD[t];

                char **buffer;

                buffer = new char*[t];

                for(i=0; i<t; i++)
                {
                        buffer[i] = NULL;
                }

                for(i=0; i<t; i++)
                {
                        for(j=0; j<m_bmpNum; j++)
                        {
                                if(m_pInfo[j].index == temp[i])
                                {
                                        // 找到了
                                        strcpy(head[i].name, (LPTSTR)(LPCTSTR)m_pInfo[j].name);
                                        head[i].index = m_pInfo[j].index;
                                        head[i].width = m_pInfo[j].width;
                                        head[i].height = m_pInfo[j].height;

                                        SaveMyBmp(m_pInfo[j].hBitmap, head[i], buffer[i]);

                                }
                        }
                }

                SEGMENTINFO segInfo;

                segInfo.segNum = t;
                segInfo.heads = head;
                segInfo.buffer = buffer;

                MAPINFO mapInfo;
                mapInfo.head.bmpNum = t;
                mapInfo.head.cellSize = m_mapCellSize;
                mapInfo.head.width = m_mapWidth;
                mapInfo.head.height = m_mapHeight;
                mapInfo.pData = m_pMap;

                if(!m_map.SaveMapFile((LPTSTR)(LPCTSTR)strPath, mapInfo, segInfo))
                {
                        MessageBox("保存文件失败");
                }
                

                if(buffer)
                {
                        for(i=0; i<t; i++)
                        {
                                if(buffer[i])
                                {
                                        delete[] buffer[i];
                                }
                        }
                        delete[] buffer;
                }
                

                if(temp)
                {

                        delete[] temp; 
                }
  
                
                /*
                RECT rc = {0, 0, m_mapWidth * m_mapCellSize, m_mapHeight * m_mapCellSize};
                HBITMAP hBitmap = CopyDCToBitmap(m_dcView.m_hDC, &rc);
                if(!SaveBmp(hBitmap, strPath))
                {
                        MessageBox("保存位图失败");
                        return;
                }
                */
        }
	// TODO: Add your command handler code here
	
}


void CMapEditDlg::OnSelchangeCombo() 
{
	// TODO: Add your control notification handler code here
        int cursel = m_combo.GetCurSel();
        m_listPage = cursel;

        this->UpdateIconList();
 
        this->InvalidateRect(&m_rcIcon);
}

HBITMAP CMapEditDlg::CopyDCToBitmap(HDC hSrcDC, LPRECT lpRect)
{
        if(hSrcDC==NULL || lpRect==NULL || IsRectEmpty(lpRect))            
        {        
                AfxMessageBox("参数错误");
                return NULL;          
        }
        
        HDC        hMemDC;      
        // 屏幕和内存设备描述表
        HBITMAP    hBitmap,hOldBitmap;   
        // 位图句柄
        int       nX, nY, nX2, nY2;      
        // 选定区域坐标
        int       nWidth, nHeight;      
        // 位图宽度和高度
        
        // 确保选定区域不为空矩形
        if (IsRectEmpty(lpRect))
                return NULL;
        
        // 获得选定区域坐标
        nX = lpRect->left;
        nY = lpRect->top;
        nX2 = lpRect->right;
        nY2 = lpRect->bottom;
        
        nWidth = nX2 - nX;
        nHeight = nY2 - nY;
        //为指定设备描述表创建兼容的内存设备描述表
        hMemDC = CreateCompatibleDC(hSrcDC);
        // 创建一个与指定设备描述表兼容的位图
        hBitmap = CreateCompatibleBitmap(hSrcDC, nWidth, nHeight);
        // 把新位图选到内存设备描述表中
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
        // 把屏幕设备描述表拷贝到内存设备描述表中
        StretchBlt(hMemDC,0,0,nWidth,nHeight,hSrcDC,nX,nY,nWidth,nHeight,SRCCOPY);
        //BitBlt(hMemDC, 0, 0, nWidth, nHeight,hScrDC, nX, nY, SRCCOPY);
        //得到屏幕位图的句柄
        
        hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
        //清除 
        
        DeleteDC(hMemDC);
        DeleteObject(hOldBitmap);
        // 返回位图句柄
        return hBitmap;
}

//把HBITMAP保存成位图
BOOL CMapEditDlg::SaveBmp(HBITMAP hBitmap, CString FileName)
{     
        if(hBitmap==NULL || FileName.IsEmpty())        
        {          
                AfxMessageBox("参数错误");       
                return false;           
        }
        
        HDC hDC;
        //当前分辨率下每象素所占字节数
        int iBits;
        //位图中每象素所占字节数
        WORD wBitCount;
        //定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数 
        DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0; 
        //位图属性结构 
        BITMAP Bitmap;  
        //位图文件头结构
        BITMAPFILEHEADER bmfHdr;  
        //位图信息头结构 
        BITMAPINFOHEADER bi;  
        //指向位图信息头结构  
        LPBITMAPINFOHEADER lpbi;  
        //定义文件，分配内存句柄，调色板句柄 
        HANDLE fh, hDib, hPal,hOldPal=NULL; 
        
        //计算位图文件每个像素所占字节数 
        hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
        iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES); 
        DeleteDC(hDC); 

    
        if (iBits <= 1)  wBitCount = 1; 
        else if (iBits <= 4)  wBitCount = 4; 
        else if (iBits <= 8)  wBitCount = 8; 
        else      wBitCount = 24; 
      

        GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
        bi.biSize   = sizeof(BITMAPINFOHEADER);
        bi.biWidth   = Bitmap.bmWidth;
        bi.biHeight   = Bitmap.bmHeight;
        bi.biPlanes   = 1;
        bi.biBitCount  = wBitCount;
        bi.biCompression = BI_RGB;
        bi.biSizeImage  = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrImportant = 0;
        bi.biClrUsed  = 0;
        
        dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

        bi.biSizeImage = dwBmBitsSize;
        
        //为位图内容分配内存 
        hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER)); 
        lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
        *lpbi = bi;
        
        // 处理调色板  
        hPal = GetStockObject(DEFAULT_PALETTE); 
        if (hPal) 
        { 
                hDC = ::GetDC(NULL); 
                hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
                RealizePalette(hDC); 
        }
        
        // 获取该调色板下新的像素值 
        GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
                +dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS); 
        
        //恢复调色板  
        if (hOldPal) 
        { 
                ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
                RealizePalette(hDC); 
                ::ReleaseDC(NULL, hDC); 
        }
        
        //创建位图文件  
        fh = CreateFile(FileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS, 
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 
        
        if (fh == INVALID_HANDLE_VALUE)  return FALSE; 
        
        // 设置位图文件头 
        bmfHdr.bfType = 0x4D42; // "BM" 
        dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;  
        bmfHdr.bfSize = dwDIBSize; 
        bmfHdr.bfReserved1 = 0; 
        bmfHdr.bfReserved2 = 0; 
        bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize; 
        // 写入位图文件头 
        WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 
        // 写入位图文件其余内容 
        WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL); 
        //清除  
        GlobalUnlock(hDib); 
        GlobalFree(hDib); 
        CloseHandle(fh);
        
        return TRUE;
}

BOOL CMapEditDlg::SaveMyBmp(HBITMAP hBitmap, SEGMENTHEAD &segHead, char* &buffer)
{
        if(hBitmap==NULL)        
        {          
                AfxMessageBox("参数错误");       
                return false;           
        }
        
        HDC hDC;
        //当前分辨率下每象素所占字节数
        int iBits;
        //位图中每象素所占字节数
        WORD wBitCount;
        //定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数 
        DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0; 
        //位图属性结构 
        BITMAP Bitmap;  
        //位图文件头结构
        BITMAPFILEHEADER bmfHdr;  
        //位图信息头结构 
        BITMAPINFOHEADER bi;  
        //指向位图信息头结构  
        LPBITMAPINFOHEADER lpbi;  
        //定义文件，分配内存句柄，调色板句柄 
        HANDLE hDib, hPal,hOldPal=NULL; 
        
        //计算位图文件每个像素所占字节数 
        hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
        iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES); 
        DeleteDC(hDC); 

    
        if (iBits <= 1)  wBitCount = 1; 
        else if (iBits <= 4)  wBitCount = 4; 
        else if (iBits <= 8)  wBitCount = 8; 
        else      wBitCount = 24; 
      

        GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
        bi.biSize   = sizeof(BITMAPINFOHEADER);
        bi.biWidth   = Bitmap.bmWidth;
        bi.biHeight   = Bitmap.bmHeight;
        bi.biPlanes   = 1;
        bi.biBitCount  = wBitCount;
        bi.biCompression = BI_RGB;
        bi.biSizeImage  = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrImportant = 0;
        bi.biClrUsed  = 0;
        
        dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

        bi.biSizeImage = dwBmBitsSize;
        
        //为位图内容分配内存 
        hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER)); 
        lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
        *lpbi = bi;
        
        // 处理调色板  
        hPal = GetStockObject(DEFAULT_PALETTE); 
        if (hPal) 
        { 
                hDC = ::GetDC(NULL); 
                hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
                RealizePalette(hDC); 
        }
        
        // 获取该调色板下新的像素值 
        GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
                +dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS); 
        
        //恢复调色板  
        if (hOldPal) 
        { 
                ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
                RealizePalette(hDC); 
                ::ReleaseDC(NULL, hDC); 
        }
        
       
        
        // 设置位图文件头 
        bmfHdr.bfType = 0x4D42; // "BM" 
        dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;  
        bmfHdr.bfSize = dwDIBSize; 
        bmfHdr.bfReserved1 = 0; 
        bmfHdr.bfReserved2 = 0; 
        bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize; 

        // 设置结构体信息
        segHead.byteNum = dwDIBSize;
        segHead.bpp = bi.biBitCount;

        if(buffer)
        {
                delete[] buffer;
        }

        buffer = new char[dwDIBSize];

        memcpy((char*)buffer, (char*)&bmfHdr, sizeof(BITMAPFILEHEADER));
        memcpy((char*)(buffer + sizeof(BITMAPFILEHEADER)), (char*)lpbi, dwDIBSize - sizeof(BITMAPFILEHEADER));



        /*
        // 写入位图文件头 
        WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 
        // 写入位图文件其余内容 
        WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL); 

  */

        //清除  
        GlobalUnlock(hDib); 
        GlobalFree(hDib); 
     

        /*
        if(hBitmap==NULL)        
        {          
                AfxMessageBox("参数错误");       
                return false;           
        }
        
        HDC hDC;
        //当前分辨率下每象素所占字节数
        int iBits;
        //位图中每象素所占字节数
        WORD wBitCount;
        //定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数 
        DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0; 
        //位图属性结构 
        BITMAP Bitmap;  

        //位图信息头结构 
        BITMAPINFOHEADER bi;  
        //指向位图信息头结构  
        LPBITMAPINFOHEADER lpbi;  
        //定义文件，分配内存句柄，调色板句柄 
        HANDLE hDib, hPal,hOldPal=NULL; 
        
        //计算位图文件每个像素所占字节数 
        hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
        iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES); 
        DeleteDC(hDC); 

        /*
        if (iBits <= 1)  wBitCount = 1; 
        else if (iBits <= 4)  wBitCount = 4; 
        else if (iBits <= 8)  wBitCount = 8; 
        else      wBitCount = 24; 
        


        
        wBitCount = 24;

        GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
        bi.biSize   = sizeof(BITMAPINFOHEADER);
        bi.biWidth   = Bitmap.bmWidth;
        bi.biHeight   = Bitmap.bmHeight;
        bi.biPlanes   = 1;
        bi.biBitCount  = wBitCount;
        bi.biCompression = BI_RGB;
        bi.biSizeImage  = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrImportant = 0;
        bi.biClrUsed  = 0;
        
        dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

        bi.biSizeImage = dwBmBitsSize;
        
        //为位图内容分配内存 
        hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER)); 
        lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
        *lpbi = bi;
        
        // 处理调色板  
        hPal = GetStockObject(DEFAULT_PALETTE); 
        if (hPal) 
        { 
                hDC = ::GetDC(NULL); 
                hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
                RealizePalette(hDC); 
        }

        bmp.header.width = bi.biWidth;
        bmp.header.height = bi.biHeight;
        bmp.header.byteNum = bi.biSizeImage;
        bmp.header.index = mapIndex;
        bmp.header.bpp = bi.biBitCount;
        strcpy(bmp.header.name, name);
        bmp.buffer = new char[bi.biSizeImage];


        
        // 获取该调色板下新的像素值 
        GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, bmp.buffer, 
                (BITMAPINFO *)lpbi, DIB_RGB_COLORS); 

        if (hOldPal) 
        { 
                ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
                RealizePalette(hDC); 
                ::ReleaseDC(NULL, hDC); 
        }
  
        //清除  
        GlobalUnlock(hDib); 
        GlobalFree(hDib); 
        */

        return TRUE;
}