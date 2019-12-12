// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MyLT.h"

#include "MainFrm.h"
#include "MyLTDoc.h"
#include "MyLTView.h"
#include "MyAnalyseView.h"
#include "MyStatView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZING()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDM_VIEW_BEGIN, IDM_VIEW_END, OnViewChange)
	ON_UPDATE_COMMAND_UI_RANGE(IDM_VIEW_BEGIN, IDM_VIEW_END, OnUpdateViewAll)
	ON_UPDATE_COMMAND_UI_RANGE(IDS_PANEINFO_BEGIN, IDS_PANEINFO_END, OnUpdateStatusPanel)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	IDS_INDICATOR_TOTALNUM,
	IDS_INDICATOR_MSG,
	IDS_INDICATOR_TIME
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_pLTView = NULL;
	m_pAnalyseView = NULL;
	m_pStatView = NULL;
}

CMainFrame::~CMainFrame()
{
	;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!MyCreateToolBar())
		return -1;

	if (!MyCreateStatusBar())
		return -1;

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	return 0;
}

BOOL CMainFrame::MyCreateToolBar()
{
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_AUTOSIZE| TBSTYLE_TRANSPARENT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;     // fail to create
	}
		
	UINT uBmID[] =
	{
		IDB_BMP_NEW, IDB_BMP_OPEN, IDB_BMP_SAVE,
		IDB_BMP_MAIN, IDB_BMP_ANALYSE, IDB_BMP_STAT,
		IDB_BMP_DOWN,
		IDB_BMP_TRANS, IDB_BMP_OPTIONS,
		IDB_BMP_HELP,
	};

	long a;
	CImageList img;
	img.Create(32, 32, ILC_MASK|ILC_COLOR16, 10, 0);
	for (a = 0; a < 10; a++)
	{
		CBitmap bm;
		bm.LoadBitmap((LPCTSTR)uBmID[a]);
		img.Add(&bm, RGB(0, 127, 127));
	}
	CToolBarCtrl &tbc = m_wndToolBar.GetToolBarCtrl();
	tbc.SetImageList(&img);
	img.Detach();

	TOOLBAR_INFO toobarinfo[] =
	{
		{IDM_FILE_NEW,		0, TBSTYLE_BUTTON, "新建"},
		{IDM_FILE_OPEN,		1, TBSTYLE_BUTTON, "打开"},
		{IDM_FILE_SAVE,		2, TBSTYLE_BUTTON, "保存"},
		{0,					0, TBBS_SEPARATOR, ""},
		{IDM_VIEW_MAIN,		3, TBSTYLE_BUTTON, "主图"},
		{IDM_VIEW_ANALYSE,	4, TBSTYLE_BUTTON, "分析"},
		{IDM_VIEW_STAT,		5, TBSTYLE_BUTTON, "统计"},
		{0,					0, TBBS_SEPARATOR, ""},
		{IDM_NET_DOWNLOAD,	6, TBSTYLE_BUTTON, "下载"},
		{0,					0, TBBS_SEPARATOR, ""},
		{IDM_DATA_TRANSDATA,7, TBSTYLE_BUTTON, "转换"},
		{IDM_OPT_SYSSETUP,	8, TBSTYLE_BUTTON, "选项"},
		{0,					0, TBBS_SEPARATOR, ""},
		{IDM_HLP_ABOUT,		9,TBSTYLE_BUTTON, "关于"},
	};

	long lBtnNum = sizeof(toobarinfo) / sizeof(toobarinfo[0]);

	m_wndToolBar.SetButtons(NULL, lBtnNum);
	for (a = 0; a < lBtnNum; a++)
	{
		m_wndToolBar.SetButtonInfo(a, toobarinfo[a].uID, toobarinfo[a].uStyle, toobarinfo[a].lBitmapIndex);
		m_wndToolBar.SetButtonText(a, toobarinfo[a].pName);
	}

	CRect rectToolBar;
	m_wndToolBar.GetItemRect(0, &rectToolBar);
	m_wndToolBar.SetSizes(CSize(200, rectToolBar.Height()), CSize(32, 32));
	m_wndToolBar.SetHeight(rectToolBar.Height() + 6);

	RecalcLayout(FALSE);

	return TRUE;
}

BOOL CMainFrame::MyCreateStatusBar()
{
	/*
	IDS_INDICATOR_PROCESS,
	IDS_INDICATOR_TOTALNUM,
	IDS_INDICATOR_MSG,
	IDS_INDICATOR_TIME
	*/

	long a, lIdNum, lIndex;
	UINT uStyle;
	long lPaneWidth[] = {100, 200, 110};

	lIdNum = sizeof(indicators)/sizeof(UINT);
	if (m_wndStatusBar.Create(this) && m_wndStatusBar.SetIndicators(indicators, lIdNum))
	{
		m_wndStatusBar.SetPaneInfo(0, indicators[0], SBPS_STRETCH | SBPS_NOBORDERS, 0); 

		for (a = 1; a < lIdNum; a++)
		{
			lIndex = m_wndStatusBar.CommandToIndex(indicators[a]);
		//	注意：取得的缺省属性会有SBPS_DISABLED，造成状态条panel不能被更新
		//	uStyle = m_wndStatusBar.GetStyle();
			uStyle = SBPS_NORMAL;
			m_wndStatusBar.SetPaneInfo(lIndex, indicators[a], uStyle, lPaneWidth[a - 1]);
		}
	}
	else
	{
		TRACE0("Failed to create StatusBar\n");
		return FALSE;
	}

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style &= ~FWS_ADDTOTITLE;	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::AddAllView()
{
	CDocument *pDoc = GetActiveDocument();

	//1. create view
	m_pAnalyseView = (CMyAnalyseView *)CMyAnalyseView::CreateObject();
	((CWnd *)m_pAnalyseView)->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), this, AFX_IDW_PANE_FIRST + 1, NULL);

	m_pStatView = (CMyStatView *)CMyStatView::CreateObject();
	((CWnd *)m_pStatView)->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
		CRect(0,0,0,0), this, AFX_IDW_PANE_FIRST + 2, NULL);
	
	pDoc->AddView(m_pAnalyseView);
	pDoc->AddView(m_pStatView);
	
	m_pAnalyseView->OnInitialUpdate();
	m_pStatView->OnInitialUpdate();
	
	m_pAnalyseView->ShowWindow(SW_HIDE);
	m_pStatView->ShowWindow(SW_HIDE);
	
	m_pLTView = (CMyLTView *)GetActiveView();

	//2. read config
	m_pAnalyseView->RWCfg(TRUE);
}

void CMainFrame::OnViewChange(UINT uCmdID)
{
	CView *pWillShow = NULL;
	CView *pActive = GetActiveView();
	
	if (uCmdID == IDM_VIEW_MAIN)
		pWillShow = (CView *)m_pLTView;
	else if(uCmdID == IDM_VIEW_ANALYSE)
		pWillShow = (CView *)m_pAnalyseView;
	else
		pWillShow = (CView *)m_pStatView;
		
	if (pWillShow != pActive)
	{
		long lCtrlId = pWillShow->GetDlgCtrlID();
		pActive->SetDlgCtrlID(lCtrlId);
		pWillShow->SetDlgCtrlID(AFX_IDW_PANE_FIRST);

		SetActiveView(pWillShow);
		pActive->ShowWindow(SW_HIDE);
		pWillShow->ShowWindow(SW_SHOW);

		RecalcLayout();
	}
}

void CMainFrame::OnUpdateViewAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	switch (pCmdUI->m_nID)
	{
	case IDM_VIEW_MAIN:
		pCmdUI->SetRadio((CView *)m_pLTView == GetActiveView());
		break;
	case IDM_VIEW_ANALYSE:
		pCmdUI->SetRadio((CView *)m_pAnalyseView == GetActiveView());
		break;
	case IDM_VIEW_STAT:
		pCmdUI->SetRadio((CView *)m_pStatView == GetActiveView());
		break;
	default:
		break;
	}
}

void CMainFrame::OnUpdateStatusPanel(CCmdUI* pCmdUI)
{
	if (pCmdUI->m_nID == IDS_INDICATOR_TOTALNUM)
	{
		if (m_pLTView)
		{
			CString strNum;
			long lNum;
			LT_DATA_INFO *p = ((CMyLTView *)m_pLTView)->GetNewDataInfo(lNum);
			strNum.Format("总记录数:%d", lNum);
			m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(IDS_INDICATOR_TOTALNUM), strNum);
		}
	}
	else if (pCmdUI->m_nID == IDS_INDICATOR_TIME)
	{
		CString strTime, strAM;
		CTime tm = CTime::GetCurrentTime();
		
		long lHour = tm.GetHour();
		if (lHour >= 0 && lHour < 6)
			strAM = "凌晨";
		else if (lHour >= 6 && lHour < 12)
			strAM = "上午";
		else if (lHour >= 12 && lHour < 18)
		{
			strAM = "下午";
			lHour -= 12;
		}
		else
		{
			strAM = "晚上";
			lHour -= 12;
		}

		strTime.Format("%d月%d日 %s%02d:%02d", tm.GetMonth(), tm.GetDay(), 
				strAM, lHour, tm.GetMinute());

		m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(IDS_INDICATOR_TIME), strTime);
	}
	pCmdUI->Enable();
}
		
void CMainFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	nCmdShow = SW_MAXIMIZE;
	CFrameWnd::ActivateFrame(nCmdShow);
}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CFrameWnd::OnSizing(fwSide, pRect);
	
	// TODO: Add your message handler code here
	if (pRect->right - pRect->left < 400)
		pRect->right = pRect->left + 400;

	if (pRect->bottom - pRect->top < 300)
		pRect->bottom = pRect->top + 300;
}

BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pAnalyseView->RWCfg(FALSE);
	return CFrameWnd::DestroyWindow();
}
