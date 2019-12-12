// MyDWView.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "MyLTDoc.h"
#include "MainFrm.h"
#include "MyAnalyseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyAnalyseView

IMPLEMENT_DYNCREATE(CMyAnalyseView, CView)

CMyAnalyseView::CMyAnalyseView()
{
	for (long a = 0; a < 14; a++)
		m_rtCtrls[a].SetRectEmpty();

	m_lCurrCtrl = 0;

	m_lCtrlStartPos = 0;
	m_lCtrlCurrPos = 0;
	m_lCtrlOnePageNum = 0;
	m_lCtrlScrollRange = 0;
	m_lCtrlTotalNum = 0;

	m_lObjStartPos = 0;
	m_lObjCurrPos = 0;
	m_lObjOnePageNum = 0;
	m_lObjScrollRange = 0;
	m_lObjTotalNum = 0;

	m_pDataInfo = NULL;
	m_lDataNum = 0;
	m_lSelFrom = m_lSelFor = 0;
	m_bHaveSpec = FALSE;


}

CMyAnalyseView::~CMyAnalyseView()
{
}

BEGIN_MESSAGE_MAP(CMyAnalyseView, CView)
	//{{AFX_MSG_MAP(CMyAnalyseView)
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_DYNAMIC_BEGIN, IDC_DYNAMIC_END, OnBtns)
	ON_COMMAND_RANGE(IDM_DYNAMIC_MENU_BEGIN, IDM_DYNAMIC_MENU_END, OnMenus)
	ON_MESSAGE(WM_VSCROLL, OnScroll)
END_MESSAGE_MAP()

BOOL CMyAnalyseView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class	
	//cs.style |= WS_VSCROLL;
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyAnalyseView drawing
void CMyAnalyseView::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
	if (theApp.m_pMainWnd && 
		((CFrameWnd *)theApp.m_pMainWnd)->GetActiveView() == this &&
		m_MemDC.GetSafeHdc())
	{
		pDC->SelectClipRgn(&m_rgnButton, RGN_XOR);

		RedrawAll(pDC);
		SetCtrlsState();

		pDC->SelectClipRgn(NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyAnalyseView diagnostics

#ifdef _DEBUG
void CMyAnalyseView::AssertValid() const
{
	CView::AssertValid();
}

void CMyAnalyseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyAnalyseView message handlers
BOOL CMyAnalyseView::RWCfg(BOOL bIsRead)
{
	if (m_DrawProxy.RWCfg(bIsRead))
	{
		m_lCurrCtrl = m_DrawProxy.m_lCurrCtrl;
		return TRUE;
	}

	return FALSE;
}

void CMyAnalyseView::OnInitialUpdate() 
{
	CreateCtrls();
	
	CDC *pDC = GetDC();
	if (pDC)
	{
		m_MemDC.CreateMemDC(pDC, g_DataInfo.GetColor(CDrawRes::CLR_BKGROUND));
		m_MemDC.SetBkMode(TRANSPARENT);
	}

	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class	
}

void CMyAnalyseView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMyLTDoc *pDoc = GetDocument();

	m_pDataInfo = pDoc->GetDataInfo(m_lDataNum);
	m_lSelFrom = pDoc->m_lSelFrom;
	m_lSelFor = pDoc->m_lSelFor;
	m_bHaveSpec = pDoc->m_bHaveSpec;

	m_DrawProxy.SetData(m_pDataInfo, m_lDataNum, m_lSelFrom, m_lSelFor, m_bHaveSpec);	
	if (((CMainFrame *)GetParent())->GetActiveView() == this)
		Invalidate(FALSE);
}

void CMyAnalyseView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (bActivate)
		Invalidate(FALSE);

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

LRESULT CMyAnalyseView::OnScroll(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default	
	if (m_lCtrlScrollRange)
	{
		int nSBCode = (int)LOWORD(wParam);
		int nPos = (short int)HIWORD(wParam);

		long lNewPos = m_lCtrlCurrPos;
		switch (nSBCode)
		{
		case SB_LINEUP:
			lNewPos--;
			break;
		case SB_LINEDOWN:
			lNewPos++;
			break;
		case SB_PAGEUP:
			lNewPos -= m_lCtrlOnePageNum;
			break;
		case SB_PAGEDOWN:
			lNewPos += m_lCtrlOnePageNum;
			break;
		case SB_TOP:
			lNewPos = 0;
			break;
		case SB_BOTTOM:
			lNewPos = m_lCtrlScrollRange;
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			lNewPos = nPos;
			break;
		default:
			break;
		}

		ScrollView(0, lNewPos);
	}

	return FALSE;
}

BOOL CMyAnalyseView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_lObjScrollRange)
	{
		long lNewPos = m_lObjCurrPos;
		lNewPos -= zDelta / WHEEL_DELTA;

		ScrollView(1, lNewPos);
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CMyAnalyseView::ScrollView(long lType, long lNewPos)
{
	if (lType == 0)
	{//ctrl type change.
		if (IDrawCtrl::AdjustStartPosByCurrPos(lNewPos, m_lCtrlCurrPos, 
						m_lCtrlStartPos, m_lCtrlOnePageNum, m_lCtrlTotalNum))
		{
			m_DrawProxy.SetScrollPos(0, m_lCtrlStartPos, m_lCtrlCurrPos);
			Invalidate(FALSE);
		}
	}
	else
	{//obj type change
		if (IDrawCtrl::AdjustStartPosByCurrPos(lNewPos, m_lObjCurrPos, 
						m_lObjStartPos, m_lObjOnePageNum, m_lObjTotalNum))
		{
			m_DrawProxy.SetScrollPos(1, m_lObjStartPos, m_lObjCurrPos);
			Invalidate(FALSE);
		}
	}
}

void CMyAnalyseView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (cx > 0 && cy > 0)
	{
		GetAllPanelRect();

		m_DrawProxy.SetParam(m_rtCtrls[1]);

		if (m_wndVScroll.GetSafeHwnd())
		{
			for (long a = 0; a < 10; a++)
			{
				m_wndBtns[a].SetWindowPos(NULL, m_rtCtrls[4 + a].left, m_rtCtrls[4 + a].top,
										m_rtCtrls[4 + a].Width(), m_rtCtrls[4 + a].Height(),
					SWP_NOZORDER);
			}

			m_wndVScroll.SetWindowPos(NULL, m_rtCtrls[2].left, 
					m_rtCtrls[2].top, m_rtCtrls[2].Width(), m_rtCtrls[2].Height(),
					SWP_NOZORDER);
		}

		Invalidate(FALSE);
	}
}

void CMyAnalyseView::GetAllPanelRect()
{
	CRect rtClient;
	GetClientRect(rtClient);

	//Margn: Left:25 Right:25 Up:20 Bottom:30 
	//get frame
	m_rtCtrls[0].SetRect(rtClient.left, rtClient.top, rtClient.right, rtClient.bottom);
	m_rtCtrls[1].SetRect(m_rtCtrls[0].left + 5, m_rtCtrls[0].top + 25, m_rtCtrls[0].right - 30, m_rtCtrls[0].bottom - 30);
	long lMiddle = m_rtCtrls[1].top + m_rtCtrls[1].Height() / 2;
	m_rtCtrls[2].SetRect(m_rtCtrls[1].right + 3, lMiddle - 20, m_rtCtrls[1].right + 3 + GetSystemMetrics(SM_CXVSCROLL) + 5, lMiddle + 20);
	m_rtCtrls[3].SetRect(m_rtCtrls[1].left, m_rtCtrls[1].bottom + 2, m_rtCtrls[1].right, m_rtCtrls[0].bottom);

	//btn'height = 14
	{
		// ||_MainView_RateView_FilterView_|_Command_|__|_<-_->_Up_Down_PageUp_PageDown_||
		// || 
		long a, lWidth, lHeight, lGap, lMargn, lWdA, lWdB, lWdC, lWdD, lWdE, lX, lTop, lBottom;
		lWidth = lHeight = lGap = lMargn = lWdA = lWdB = lWdC = lWdD = lWdE = lX = lTop = lBottom = 0;
		lMargn = 5;

		lWidth = m_rtCtrls[3].Width() - 2 * lMargn;
		lHeight = m_rtCtrls[3].Height();

		if (lHeight > LT_BTN_HEIGHT)
			lGap = (lHeight - LT_BTN_HEIGHT) / 2;

		lTop = m_rtCtrls[3].top + lGap;
		lBottom = m_rtCtrls[3].bottom - lGap;

		lWdA = lWidth / 2;		//half
		lWdB = lWdA * 2 / 3;	//left three button.
		lWdC = lWdA - lWdB;		//command
		lWdD = lWdB / 3;		//each mainview...btn.
		lWdE = lWdA / 6;		//each ->...btn
		
		lGap = 0;
		if (lWdD > LT_MAINBTN_WD)
			lGap = (lWdD - LT_MAINBTN_WD) / 2;
		lX = m_rtCtrls[3].left + lMargn;
		for (a = 0; a < 3; a++)
		{
			m_rtCtrls[4 + a].SetRect(lX + lGap, lTop, lX + lWdD - lGap, lBottom);
			lX += lWdD;			
		}

		lGap = 0;
		if (lWdA - lWdB > LT_SUBBTN_WD)
			lGap = (lWdA - lWdB - LT_SUBBTN_WD) / 2;
		lX = m_rtCtrls[3].left + lMargn + lWdB;
		m_rtCtrls[7].SetRect(lX + lGap, lTop, lX + lWdA - lWdB - lGap, lBottom);


		lGap = 0;
		if (lWdE > LT_SUBBTN_WD)
			lGap = (lWdE - LT_SUBBTN_WD) / 2;
		lX = m_rtCtrls[3].left + lMargn + lWdA;
		for (a = 0; a < 6; a++)
		{
			m_rtCtrls[8 + a].SetRect(lX + lGap, lTop, lX + lWdE - lGap, lBottom);
			lX += lWdE;
		}
	}

	{//Create buttons range.
		m_rgnButton.DeleteObject();
		m_rgnButton.CreateRectRgnIndirect(m_rtCtrls[2]);

		for (long a = 4; a < 14; a++)
		{
			CRgn rgnTmp;			
			rgnTmp.CreateRectRgnIndirect(m_rtCtrls[a]);
			m_rgnButton.CombineRgn(&m_rgnButton, &rgnTmp, RGN_OR);
		}
	}
}

void CMyAnalyseView::CreateCtrls()
{
	//create btns & scroll bar
	char *pBtnName[] = 
	{
		"基本分析", "统计分析", "走势分析", "设置", 
		"←", "→", "↑", "↓", "上页", "下页"
	};

	for (long a = 0; a < sizeof(pBtnName) / sizeof (pBtnName[0]); a++)
	{
		m_wndBtns[a].Create(pBtnName[a], WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
			CRect(0, 0, 0, 0), this, IDC_DYNAMIC_BEGIN + a);
		
		m_wndBtns[a].SetFont(g_DataInfo.GetFont(CDrawRes::FONT_CH_SMALL), FALSE);
	}

	m_wndVScroll.Create(WS_CHILD | WS_VISIBLE | UDS_ARROWKEYS, CRect(0, 0, 0, 0), this, IDC_DYNAMIC_SCROLL);
}

void CMyAnalyseView::RedrawAll(CDC *pDC)
{
	//1:draw background
	CDvMemDC *pMemDC = &m_MemDC;
	pMemDC->ClearBkGround();

	//2:draw title.	
	char *pCtrlName = m_DrawProxy.GetCtrlName(m_lCurrCtrl);

	//use big font draw title [CtrlName]
	CRect rtTitle;
	rtTitle.SetRect(m_rtCtrls[1].left + 20, m_rtCtrls[0].top, m_rtCtrls[1].right, m_rtCtrls[1].top - 2);
	CFont *pOldFont = (CFont *)pMemDC->SelectObject(g_DataInfo.GetFont(CDrawRes::FONT_CH_BIG));
	CString strTitle;
	strTitle.Format("%s", pCtrlName);
	pMemDC->SetTextColor(g_DataInfo.GetColor(CDrawRes::CLR_FRAME));
	pMemDC->DrawText(strTitle, rtTitle, DT_LEFT | DT_BOTTOM | DT_SINGLELINE);
	pMemDC->SelectObject(pOldFont);

	//3:draw interface.
	m_DrawProxy.Draw(pMemDC->GetSafeHdc());

	//4:copy mem dc to realize dc.
	pDC->BitBlt(m_rtCtrls[0].left, m_rtCtrls[0].top, m_rtCtrls[0].Width(), m_rtCtrls[0].Height(),
		pMemDC, m_rtCtrls[0].left, m_rtCtrls[0].top, SRCCOPY);
}

void CMyAnalyseView::SetCtrlsState()
{
	//1. vert scroll bar
	m_DrawProxy.GetCtrlState(m_lCtrlStartPos, m_lCtrlCurrPos, m_lCtrlOnePageNum, 
						m_lCtrlScrollRange, m_lCtrlTotalNum);
	if (m_lCtrlScrollRange)
	{
		m_wndVScroll.EnableWindow();
		m_wndVScroll.SetRange32(m_lCtrlTotalNum, 0);
		m_wndVScroll.SetPos(m_lCtrlCurrPos);
	}
	else
		m_wndVScroll.EnableWindow(FALSE);

	//2. buttons
	BOOL bBtns[6] = {FALSE};
	m_DrawProxy.GetObjState(bBtns, m_lObjStartPos, m_lObjCurrPos, 
						m_lObjOnePageNum, m_lObjScrollRange, m_lObjTotalNum);
	for (long a = 0; a < 6; a++)
	{
		m_wndBtns[4 + a].EnableWindow(bBtns[a]);
	}

	//一定要设置，否则当按钮无效后，这个视图将逝去焦点，无法响应消息
	SetFocus();
}

void CMyAnalyseView::OnBtns(UINT nID)
{
	CRect rtBtn;

	switch (nID)
	{
	case IDC_DYNAMIC_ANALYSE:
	case IDC_DYNAMIC_STAT:
	case IDC_DYNAMIC_FILTER:
		{
			long lCtrl = 0;
			UINT nMenuID = 0;
			if (nID == IDC_DYNAMIC_STAT)
			{
				lCtrl = 1;
				nMenuID = 20;
			}
			else if (nID == IDC_DYNAMIC_FILTER)
			{
				lCtrl = 2;
				nMenuID = 40;
			}
			
			CMenu menu;
			menu.CreatePopupMenu();

			for (long a = 0; a < m_DrawProxy.GetObjNum(lCtrl); a++, nMenuID++)
			{
				UINT nTrueID = nMenuID + IDM_DYNAMIC_MENU_BEGIN;				

				if (m_lCurrCtrl == lCtrl && m_lCtrlCurrPos == a)
					menu.AppendMenu(MF_STRING | MF_CHECKED, nTrueID, m_DrawProxy.GetObjName(lCtrl, a));
				else
					menu.AppendMenu(MF_STRING, nTrueID, m_DrawProxy.GetObjName(lCtrl, a));
			}

			m_wndBtns[lCtrl].GetWindowRect(rtBtn);
			menu.TrackPopupMenu(TPM_LEFTALIGN, rtBtn.right, rtBtn.top, this);
		}
		break;
	case IDC_DYNAMIC_SETUP:
		{
			UINT nMenuID = IDM_DYNAMIC_MENU_BEGIN + 60;

			CMenu menu, *pCtrlMenu;
			menu.CreatePopupMenu();

			menu.AppendMenu(MF_STRING, nMenuID, "画图信息");
			menu.AppendMenu(MF_SEPARATOR);

			pCtrlMenu = m_DrawProxy.GetPopupMenu(&menu, nMenuID + 1);
			
			m_wndBtns[3].GetWindowRect(rtBtn);
			pCtrlMenu->TrackPopupMenu(TPM_LEFTALIGN, rtBtn.right, rtBtn.top, this);
		}
		break;
	default:
		{
			UINT nChar;

			if (nID == IDC_DYNAMIC_LEFT)
				nChar = VK_LEFT;
			else if (nID == IDC_DYNAMIC_RIGHT)
				nChar = VK_RIGHT;
			else if (nID == IDC_DYNAMIC_UP)
				nChar = VK_UP;
			else if (nID == IDC_DYNAMIC_BOTTON)
				nChar = VK_DOWN;
			else if (nID == IDC_DYNAMIC_PAGEUP)
				nChar = VK_PRIOR;
			else if (nID == IDC_DYNAMIC_PAGEDOWN)
				nChar = VK_NEXT;

			SHORT sKeyState = (SHORT)::GetAsyncKeyState(VK_CONTROL);
			BOOL bIsCtrlKeyDown = sKeyState >> 15;
			
			if (m_DrawProxy.OnKeyDown(nChar, bIsCtrlKeyDown))
				Invalidate(FALSE);
		}
		break;
	}
}

void CMyAnalyseView::OnMenus(UINT nID)
{
	nID -= IDM_DYNAMIC_MENU_BEGIN;

	if (nID >= 60)
	{//command
		nID -= 60;

		if (nID == 0)
		{//base draw setup
			g_DataInfo.DrawResSetUp();
		}
		else
		{//ctrl command
			//max 40 menu-item-id can used.
			m_DrawProxy.OnCommand(nID - 1);
		}
	}
	else
	{
		long lNewCtrl = nID / 20;
		long lNewObj = nID % 20;

		if (lNewCtrl != m_lCurrCtrl || lNewObj != m_lCtrlCurrPos)
		{
			m_lCurrCtrl = lNewCtrl;
			m_lCtrlCurrPos = lNewObj;

			m_DrawProxy.SetCurrCtrlObj(m_lCurrCtrl, m_lCtrlCurrPos);			
		}
	}

	Invalidate(FALSE);
}

BOOL CMyAnalyseView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (((CMainFrame *)GetParent())->GetActiveView() == this)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			SHORT sKeyState = (SHORT)::GetAsyncKeyState(VK_CONTROL);
			BOOL bIsCtrlKeyDown = sKeyState >> 15;

			int nChar = (int)pMsg->wParam;
			if (m_DrawProxy.OnKeyDown(nChar, bIsCtrlKeyDown))
				Invalidate(FALSE);
		}
	}

	return CView::PreTranslateMessage(pMsg);
}

void CMyAnalyseView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = { point.x, point.y };

	if (m_DrawProxy.OnLButtonDown(nFlags, pt))
		Invalidate(FALSE);

	CView::OnLButtonDown(nFlags, point);
}

void CMyAnalyseView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = { point.x, point.y };

	if (m_DrawProxy.OnMouseMove(nFlags, pt))
		Invalidate(FALSE);

	CView::OnMouseMove(nFlags, point);
}
