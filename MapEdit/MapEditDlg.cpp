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

        // ���ڲ���

        // ������
	this->MoveWindow(0, 0, m_width, m_height);  

        // ������
        m_barHor.MoveWindow(0, m_viewHeight, m_viewWidth, 20);  
        m_barVer.MoveWindow(m_viewWidth, 0, 20, m_viewHeight);

        // ���ع������������ͼ����Ĭ����Ļ�ߴ磬����ʾ��������֧�ֵ�ͼ�Ĺ�����ʾ
        m_barVer.ShowWindow(SW_HIDE);
        m_barHor.ShowWindow(SW_HIDE);

         // ��Ͽ�
        m_combo.ShowWindow(SW_HIDE);
        m_combo.MoveWindow(m_width - 120, 5, 100, 20);

        // �༭��
        SetRect(&m_rcView, 0, 0, m_viewWidth, m_viewHeight);

        // ���������
        SetRect(&m_rcText, 0, m_height-80, 300, m_height-60);

         // ѡ��չʾ�б���
        SetRect(&m_rcIcon, m_width-150, 50, m_width - 22, 680);

        CDC *dc = this->GetDC();

        // ˫����DC
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


        // ������̬�ı���

        RECT rcStatic;
        int i=0;
        for(i=0; i<ICON_NUM; i++)
        {
                SetRect(&m_rcIcons[i], m_width - 124, 56  + i*90, m_width - 58, 120 + i*90);
                SetRect(&rcStatic,     m_width - 160 ,125 + i*90, m_width - 20, 140 + i*90); 
                m_stDis[i].Create("��ͼ",WS_CHILD|WS_VISIBLE|SS_CENTER, rcStatic, this);
        }

        
  

        // ��Ĭ�ϼ���һ����ͼ
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

              
                m_stDis[i].SetWindowText("δ����");

        }

   
        
        // ����Ĭ�Ͽհ�ͼ��
}

void CMapEditDlg::UpdateIconList()
{
        // ���õ�ǰ�б�����һ��ͼ��
        m_curListEnd =  m_bmpNum - ICON_NUM * m_listPage;
        if(m_curListEnd > ICON_NUM)
                m_curListEnd = ICON_NUM;
        
        // ���õ�ǰ����ʾ���б�
        
        
        // ���Ƶ�ǰ�б�
        int i=0;

        for(i=0; i<ICON_NUM; i++)
        {
                m_dcMem.BitBlt(m_rcIcons[i].left, m_rcIcons[i].top, 
                        m_rcIcons[i].right - m_rcIcons[i].left, 
                        m_rcIcons[i].bottom - m_rcIcons[i].top, 
                        &m_dcDef, 0, 0, SRCCOPY); 
                m_stDis[i].SetWindowText("δ����");
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
                        // ��Ҫ����
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

        // ����һ���µ�ͼ��new��Դ�ɶԻ��������ͷ�

        CNewMapDlg *dlg = new CNewMapDlg;
        dlg->Create(IDD_DLG_NEW);
        dlg->GetMainDlg(this);
        dlg->ShowWindow(SW_SHOW);

	
}

void CMapEditDlg::CreateNewMap(CString strName, int width, int height, int cellSize)
{
        // �˺����ɴ����µ�ͼ�ĶԻ��������ã������е�ͼ�༭���ĳ�ʼ��

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

        // ��ʼ������������
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
                str.Format("��ͼ���꣺[%d], [%d]", point.x, point.y);

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
                // �ڵ�ͼ�༭��
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
                // �����˵�ͼͼ������

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
                        // û��ѡ����Ӧ��ͼ
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
                        // ������հ�

                        return;
                }

                m_bChoose = true;

                CellInfo *curCell = &m_pInfo[i + ICON_NUM * m_listPage];

                m_curWidth = curCell->width;
                m_curHeight = curCell->height;

                // ������ʱDC(�������)
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
        

        // �Ȱѵ�ͼ���ڻ�����
	m_dcMem.BitBlt(0, 0, m_viewWidth, m_viewHeight, &m_dcView, m_barPosHor*m_mapCellSize, m_barPosVer*m_mapCellSize, SRCCOPY);

        if(m_bChoose)
        {
                // ѡ����ĳ��ͼ��

                POINT pt;
                GetCursorPos(&pt);
                this->ScreenToClient(&pt); 
                if(PtInRect(&m_rcView, pt))
                {
                        // �Ծ��ν��м���
                        // ����߽糬������߻��ϱߵ�������x,yֵΪ��
                        // ����߽糬�����ұߵ�����������ʾʱ���������õ�

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
                     
                        // ��ͼ�����ʱλ����ʾ�Ȼ��廭����

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
        CFileDialog dlg(TRUE, "*.cfg",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"��ͼ�����ļ�(*.cfg) | *.cfg"); 
        if(IDOK == dlg.DoModal())
        {
                CString path = dlg.GetPathName();

                if(!m_parser.Open((LPTSTR)(LPCTSTR)path))
                {
                        MessageBox("�ļ���ʧ��");
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
                        MessageBox("�ļ�������");
                }
                
                if(!m_parser.PatternMath(m_parser.strLine.c_str(), "['<NUM>']"))
                {       
                        MessageBox("�ļ���ʽ����ȷ");
                        return;
                }
                
                if(!m_parser.GetLine(PARSER_STRIP_EMPTY_LINES|PARSER_STRIP_COMMENTS))
                {
                        MessageBox("�ļ�������");
                        return;
                }

                if(!m_parser.PatternMath(m_parser.strLine.c_str(), "[i]"))
                {       
                        MessageBox("�ļ���ʽ����ȷ");
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
                        MessageBox("�ļ�������");
                        return;
                }

                if(!m_parser.PatternMath(m_parser.strLine.c_str(), "['<INFO>']"))
                {       
                        MessageBox("�ļ���ʽ����ȷ");
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
     

       int i=0; // ����
       CDC *dc = this->GetDC();

       for(i=0; i<m_bmpNum; i++)
       {
                CString path = m_bmpPath +'\\'+ m_pInfo[i].fileName;
                //MessageBox(path);

                m_pInfo[i].hBitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
                        (LPTSTR)(LPCTSTR)path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

                if(NULL == m_pInfo[i].hBitmap)
                {
                        MessageBox("����λͼʧ��");
                }
                m_pInfo[i].bLoaded = TRUE;

                m_pInfo[i].dc = new CDC;
                m_pInfo[i].dc->CreateCompatibleDC(dc);
                m_pInfo[i].dc->SelectObject(m_pInfo[i].hBitmap);
       }

       if(m_bmpNum >ICON_NUM)
       {
               // ��ʾ��Ͽ�

               int page = (int)(m_bmpNum/ICON_NUM);
               CString temp;
               for(i=0; i<=page; i++)
               {
                        temp.Format("�б� %d:", i+1);
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
        CFileDialog dlg(TRUE, "*.map",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"��ͼ�ļ�(*.map) | *.map"); 
        if(IDOK == dlg.DoModal())
        {
                CString strPath = dlg.GetPathName();


                if(!m_map.LoadMapFile((LPTSTR)(LPCTSTR)strPath))
                {
                        MessageBox("�����ļ�ʧ��");
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
                        
                        // ��ȡ�õ�ɫ�����µ�����ֵ 
                        
                        
                        
                        hBitmap = CreateDIBitmap(hDC, &bmih, 0,NULL, NULL, DIB_PAL_COLORS);
                        
                        
                        
                        //hBitmap = CreateDIBSection (NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS, (void**)&pBits, NULL, 0) ;
                        
                        int k = SetDIBits(hDC, hBitmap, 0, m_pInfo[i].height, (void*)m_map.GetBitmap(i)->buffer, (BITMAPINFO *)&bmih, DIB_PAL_COLORS);
                        
                        //�ָ���ɫ��  
                        if (hOldPal) 
                        { 
                                ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
                                RealizePalette(hDC); 
                                ::ReleaseDC(NULL, hDC); 
                        }
                        
                        
                        if(NULL == hBitmap)
                        {
                                MessageBox("����λͼʧ��");
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
         CFileDialog dlg(FALSE, "*.map",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"��ͼ�ļ�(*.map) | *.map"); 
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

                
                // temp���浱ǰ��ͼ���е�Ԫ�ص�����

                int *temp = new int[m_mapWidth * m_mapHeight];
                memset((char*)temp, 0, m_mapWidth * m_mapHeight * sizeof(int));
                
                temp[0] = m_pMap[0];
                int t=1;        // Ԫ�ظ���
                
                // �������е�m_pMap���ݣ�����ѡ�����еĲ��ظ�������
                int i=0, j=0;

                for(i=1; i<m_mapWidth*m_mapHeight; i++)
                {
                        for(j=0; j<t; j++)
                        {
                                if(m_pMap[i] == temp[j])
                                {
                                        // �Ѿ������ˣ�����
                                        break;
                                }
                        }

                        // û����ͬ��
                        if(j == t)
                        {
                                temp[t] = m_pMap[i];
                                t++;
                        }
                }

                // ����һ��

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
                                        // �ҵ���
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
                        MessageBox("�����ļ�ʧ��");
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
                        MessageBox("����λͼʧ��");
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
                AfxMessageBox("��������");
                return NULL;          
        }
        
        HDC        hMemDC;      
        // ��Ļ���ڴ��豸������
        HBITMAP    hBitmap,hOldBitmap;   
        // λͼ���
        int       nX, nY, nX2, nY2;      
        // ѡ����������
        int       nWidth, nHeight;      
        // λͼ��Ⱥ͸߶�
        
        // ȷ��ѡ������Ϊ�վ���
        if (IsRectEmpty(lpRect))
                return NULL;
        
        // ���ѡ����������
        nX = lpRect->left;
        nY = lpRect->top;
        nX2 = lpRect->right;
        nY2 = lpRect->bottom;
        
        nWidth = nX2 - nX;
        nHeight = nY2 - nY;
        //Ϊָ���豸�����������ݵ��ڴ��豸������
        hMemDC = CreateCompatibleDC(hSrcDC);
        // ����һ����ָ���豸��������ݵ�λͼ
        hBitmap = CreateCompatibleBitmap(hSrcDC, nWidth, nHeight);
        // ����λͼѡ���ڴ��豸��������
        hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
        // ����Ļ�豸�����������ڴ��豸��������
        StretchBlt(hMemDC,0,0,nWidth,nHeight,hSrcDC,nX,nY,nWidth,nHeight,SRCCOPY);
        //BitBlt(hMemDC, 0, 0, nWidth, nHeight,hScrDC, nX, nY, SRCCOPY);
        //�õ���Ļλͼ�ľ��
        
        hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
        //��� 
        
        DeleteDC(hMemDC);
        DeleteObject(hOldBitmap);
        // ����λͼ���
        return hBitmap;
}

//��HBITMAP�����λͼ
BOOL CMapEditDlg::SaveBmp(HBITMAP hBitmap, CString FileName)
{     
        if(hBitmap==NULL || FileName.IsEmpty())        
        {          
                AfxMessageBox("��������");       
                return false;           
        }
        
        HDC hDC;
        //��ǰ�ֱ�����ÿ������ռ�ֽ���
        int iBits;
        //λͼ��ÿ������ռ�ֽ���
        WORD wBitCount;
        //�����ɫ���С�� λͼ�������ֽڴ�С ��λͼ�ļ���С �� д���ļ��ֽ��� 
        DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0; 
        //λͼ���Խṹ 
        BITMAP Bitmap;  
        //λͼ�ļ�ͷ�ṹ
        BITMAPFILEHEADER bmfHdr;  
        //λͼ��Ϣͷ�ṹ 
        BITMAPINFOHEADER bi;  
        //ָ��λͼ��Ϣͷ�ṹ  
        LPBITMAPINFOHEADER lpbi;  
        //�����ļ��������ڴ�������ɫ���� 
        HANDLE fh, hDib, hPal,hOldPal=NULL; 
        
        //����λͼ�ļ�ÿ��������ռ�ֽ��� 
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
        
        //Ϊλͼ���ݷ����ڴ� 
        hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER)); 
        lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
        *lpbi = bi;
        
        // �����ɫ��  
        hPal = GetStockObject(DEFAULT_PALETTE); 
        if (hPal) 
        { 
                hDC = ::GetDC(NULL); 
                hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
                RealizePalette(hDC); 
        }
        
        // ��ȡ�õ�ɫ�����µ�����ֵ 
        GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
                +dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS); 
        
        //�ָ���ɫ��  
        if (hOldPal) 
        { 
                ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
                RealizePalette(hDC); 
                ::ReleaseDC(NULL, hDC); 
        }
        
        //����λͼ�ļ�  
        fh = CreateFile(FileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS, 
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 
        
        if (fh == INVALID_HANDLE_VALUE)  return FALSE; 
        
        // ����λͼ�ļ�ͷ 
        bmfHdr.bfType = 0x4D42; // "BM" 
        dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;  
        bmfHdr.bfSize = dwDIBSize; 
        bmfHdr.bfReserved1 = 0; 
        bmfHdr.bfReserved2 = 0; 
        bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize; 
        // д��λͼ�ļ�ͷ 
        WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 
        // д��λͼ�ļ��������� 
        WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL); 
        //���  
        GlobalUnlock(hDib); 
        GlobalFree(hDib); 
        CloseHandle(fh);
        
        return TRUE;
}

BOOL CMapEditDlg::SaveMyBmp(HBITMAP hBitmap, SEGMENTHEAD &segHead, char* &buffer)
{
        if(hBitmap==NULL)        
        {          
                AfxMessageBox("��������");       
                return false;           
        }
        
        HDC hDC;
        //��ǰ�ֱ�����ÿ������ռ�ֽ���
        int iBits;
        //λͼ��ÿ������ռ�ֽ���
        WORD wBitCount;
        //�����ɫ���С�� λͼ�������ֽڴ�С ��λͼ�ļ���С �� д���ļ��ֽ��� 
        DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0; 
        //λͼ���Խṹ 
        BITMAP Bitmap;  
        //λͼ�ļ�ͷ�ṹ
        BITMAPFILEHEADER bmfHdr;  
        //λͼ��Ϣͷ�ṹ 
        BITMAPINFOHEADER bi;  
        //ָ��λͼ��Ϣͷ�ṹ  
        LPBITMAPINFOHEADER lpbi;  
        //�����ļ��������ڴ�������ɫ���� 
        HANDLE hDib, hPal,hOldPal=NULL; 
        
        //����λͼ�ļ�ÿ��������ռ�ֽ��� 
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
        
        //Ϊλͼ���ݷ����ڴ� 
        hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER)); 
        lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
        *lpbi = bi;
        
        // �����ɫ��  
        hPal = GetStockObject(DEFAULT_PALETTE); 
        if (hPal) 
        { 
                hDC = ::GetDC(NULL); 
                hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
                RealizePalette(hDC); 
        }
        
        // ��ȡ�õ�ɫ�����µ�����ֵ 
        GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
                +dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS); 
        
        //�ָ���ɫ��  
        if (hOldPal) 
        { 
                ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
                RealizePalette(hDC); 
                ::ReleaseDC(NULL, hDC); 
        }
        
       
        
        // ����λͼ�ļ�ͷ 
        bmfHdr.bfType = 0x4D42; // "BM" 
        dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;  
        bmfHdr.bfSize = dwDIBSize; 
        bmfHdr.bfReserved1 = 0; 
        bmfHdr.bfReserved2 = 0; 
        bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize; 

        // ���ýṹ����Ϣ
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
        // д��λͼ�ļ�ͷ 
        WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 
        // д��λͼ�ļ��������� 
        WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL); 

  */

        //���  
        GlobalUnlock(hDib); 
        GlobalFree(hDib); 
     

        /*
        if(hBitmap==NULL)        
        {          
                AfxMessageBox("��������");       
                return false;           
        }
        
        HDC hDC;
        //��ǰ�ֱ�����ÿ������ռ�ֽ���
        int iBits;
        //λͼ��ÿ������ռ�ֽ���
        WORD wBitCount;
        //�����ɫ���С�� λͼ�������ֽڴ�С ��λͼ�ļ���С �� д���ļ��ֽ��� 
        DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0; 
        //λͼ���Խṹ 
        BITMAP Bitmap;  

        //λͼ��Ϣͷ�ṹ 
        BITMAPINFOHEADER bi;  
        //ָ��λͼ��Ϣͷ�ṹ  
        LPBITMAPINFOHEADER lpbi;  
        //�����ļ��������ڴ�������ɫ���� 
        HANDLE hDib, hPal,hOldPal=NULL; 
        
        //����λͼ�ļ�ÿ��������ռ�ֽ��� 
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
        
        //Ϊλͼ���ݷ����ڴ� 
        hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER)); 
        lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
        *lpbi = bi;
        
        // �����ɫ��  
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


        
        // ��ȡ�õ�ɫ�����µ�����ֵ 
        GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, bmp.buffer, 
                (BITMAPINFO *)lpbi, DIB_RGB_COLORS); 

        if (hOldPal) 
        { 
                ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
                RealizePalette(hDC); 
                ::ReleaseDC(NULL, hDC); 
        }
  
        //���  
        GlobalUnlock(hDib); 
        GlobalFree(hDib); 
        */

        return TRUE;
}