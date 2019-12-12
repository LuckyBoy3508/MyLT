// MyLTView.cpp : implementation of the CMyLTView class
//

#include "stdafx.h"
#include "MyLT.h"

#include "MyLTDoc.h"
#include "MainFrm.h"
#include "MyLTView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyLTView

IMPLEMENT_DYNCREATE(CMyLTView, CListView)

BEGIN_MESSAGE_MAP(CMyLTView, CListView)
	//{{AFX_MSG_MAP(CMyLTView)
	ON_COMMAND(IDM_DATA_MANAGER, OnDataManager)
	ON_UPDATE_COMMAND_UI(IDM_DATA_MANAGER, OnUpdateDataManager)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyLTView construction/destruction

CMyLTView::CMyLTView()
{
	// TODO: add construction code here
	m_pDataInfo = NULL;
	m_lDataNum = 0;
	m_lSelFrom = m_lSelFor = 0;
	m_bHaveSpec = FALSE;
	m_lCurrSel = -1;
	m_bCurrShow = FALSE;
}

CMyLTView::~CMyLTView()
{
	if (m_pDataInfo)
		delete m_pDataInfo;
}

BOOL CMyLTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | LVS_SINGLESEL;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyLTView drawing

void CMyLTView::OnDraw(CDC* pDC)
{
	CMyLTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CMyLTView::OnInitialUpdate()
{
	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
	CListCtrl &lc = GetListCtrl();
	lc.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_DataManager.Create(CDataManager::IDD, this);
	m_DataManager.CenterWindow(this);

	CListView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CMyLTView printing

BOOL CMyLTView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyLTView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyLTView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyLTView diagnostics

#ifdef _DEBUG
void CMyLTView::AssertValid() const
{
	CListView::AssertValid();
}

void CMyLTView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CMyLTDoc* CMyLTView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyLTDoc)));
	return (CMyLTDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyLTView message handlers

void CMyLTView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pSender == this) return;

	CMyLTDoc* pDoc = GetDocument();

	m_lDataNum = pDoc->m_lDataNum;
	if (m_pDataInfo)
		delete m_pDataInfo;
	m_pDataInfo = new LT_DATA_INFO[m_lDataNum];
	memcpy(m_pDataInfo, pDoc->m_pDataInfo, sizeof(LT_DATA_INFO) * m_lDataNum);
	m_lSelFrom = pDoc->m_lSelFrom;
	m_lSelFor = pDoc->m_lSelFor;
	m_bHaveSpec = pDoc->m_bHaveSpec;

	CListCtrl &lc = GetListCtrl();
	lc.DeleteAllItems();
	UpdateCtlHeader();

	if (m_lDataNum)
	{
		CString strItem;
		long a;

		LT_DATA_INFO *pDataInfoTemp = m_pDataInfo;
		for(a = 0; a < m_lDataNum; a++, pDataInfoTemp++)
			MyInsertItem(a, pDataInfoTemp);

		lc.EnsureVisible(m_lDataNum - 1, FALSE);
	}

	m_lCurrSel = -1;
	m_DataManager.SetCurrDoc(pDoc);
}

void CMyLTView::MyInsertItem(long lIndex, LT_DATA_INFO *pDataInfo)
{
	CListCtrl &lc = GetListCtrl();

	CString strItem;
	strItem.Format("%03d", pDataInfo->lPeriodID);
	lc.InsertItem(lIndex, strItem);

	UpdateSubItem(lIndex, pDataInfo);
}

void CMyLTView::UpdateSubItem(long lIndex, LT_DATA_INFO *pDataInfo)
{
	CListCtrl &lc = GetListCtrl();
	CString strItem;

	strItem.Format("%s", pDataInfo->szPeriod);
	lc.SetItemText(lIndex, 1, strItem);

	strItem.Format("%s", pDataInfo->szDate);
	lc.SetItemText(lIndex, 2, strItem);
	
	strItem.Format("%d", pDataInfo->lTotalDev);
	lc.SetItemText(lIndex, 3, strItem);

	for (long a = 0; a < m_lSelFor; a++)
	{
		strItem.Format("%02d", pDataInfo->lData[a]);
		lc.SetItemText(lIndex, 4 + a, strItem);					
	}
}

void CMyLTView::UpdateCtlHeader()
{
	static long lOldSelFor = -1;

	if (lOldSelFor != m_lSelFor)
	{
		lOldSelFor = m_lSelFor;

		CListCtrl &lc = GetListCtrl();
		lc.DeleteAllItems();

		long lCol = 0;
		while (lc.DeleteColumn(lCol))
		{
			;//每删除一次列数变少，所以lCol不用加1
		}

		//总期数","开奖期数", "开奖日期","总投注额"
		lc.InsertColumn(0, "总期数", LVCFMT_CENTER, 70, 0);
		lc.InsertColumn(1, "开奖期数", LVCFMT_CENTER, 70, 1);
		lc.InsertColumn(2, "开奖日期", LVCFMT_LEFT, 90, 2);
		lc.InsertColumn(3, "总投注额", LVCFMT_RIGHT, 70, 3);

		CString strCol;
		for (long a = 0; a < m_lSelFor; a++)
		{
			if (a == m_lSelFor - 1 && m_bHaveSpec)
				strCol.Format("特别号码", a + 1);
			else
				strCol.Format("第%d个号码", a + 1);
			lc.InsertColumn(4 + a, strCol, LVCFMT_CENTER, 80, 4 + a);		
		}
	}
}

void CMyLTView::ChangeData(LT_CHAGE_TYPE eType, LT_DATA_INFO *pNewData)
{
	CListCtrl &lc = GetListCtrl();
	CString strItem;

	switch (eType)
	{
	case LCT_ADD:
		{
			m_lCurrSel = m_lDataNum;

			pNewData->lPeriodID = m_lCurrSel;
			long lDataNum = m_lDataNum + 1;
			LT_DATA_INFO *pNewDataInfo = new LT_DATA_INFO[lDataNum];
			memcpy(pNewDataInfo, m_pDataInfo, sizeof(LT_DATA_INFO) * m_lDataNum);
			memcpy(pNewDataInfo + m_lDataNum, pNewData, sizeof(LT_DATA_INFO));
			delete m_pDataInfo;
			m_pDataInfo = pNewDataInfo;
			m_lDataNum = lDataNum;

			MyInsertItem(m_lCurrSel, pNewData);
		}
		break;
	case LCT_INSERT:
		{
			pNewData->lPeriodID = m_lCurrSel;

			long lDataNum = m_lDataNum + 1;
			LT_DATA_INFO *pNewDataInfo = new LT_DATA_INFO[lDataNum];
			memcpy(pNewDataInfo, m_pDataInfo, sizeof(LT_DATA_INFO) * m_lCurrSel);
			memcpy(pNewDataInfo + m_lCurrSel, pNewData, sizeof(LT_DATA_INFO));
			memcpy(pNewDataInfo + m_lCurrSel + 1, m_pDataInfo + m_lCurrSel, 
							sizeof(LT_DATA_INFO) * (m_lDataNum - m_lCurrSel));
			delete m_pDataInfo;
			m_pDataInfo = pNewDataInfo;
			m_lDataNum = lDataNum;

			MyInsertItem(m_lCurrSel, pNewData);

			ChangePeriod();
			UpdateItemTitle();
		}
		break;
	case LCT_MODIFY:
		{
			pNewData->lPeriodID = m_lCurrSel;
			memcpy(m_pDataInfo + m_lCurrSel, pNewData, sizeof(LT_DATA_INFO));

			UpdateSubItem(m_lCurrSel, pNewData);
		}
		break;
	case LCT_DELETE:
		{
			long lDataNum = m_lDataNum - 1;
			LT_DATA_INFO *pNewDataInfo = new LT_DATA_INFO[lDataNum];
			memcpy(pNewDataInfo, m_pDataInfo, sizeof(LT_DATA_INFO) * m_lCurrSel);
			memcpy(pNewDataInfo + m_lCurrSel, m_pDataInfo + m_lCurrSel + 1, 
							sizeof(LT_DATA_INFO) * (m_lDataNum - m_lCurrSel - 1));
			delete m_pDataInfo;
			m_pDataInfo = pNewDataInfo;
			m_lDataNum = lDataNum;
			
			//bug: Notify message will be send when delete current select item. 
			//     so m_lCurrSel variable will be changed. 
			long lOldSel = m_lCurrSel;
			lc.DeleteItem(m_lCurrSel);
			ChangePeriod();
			UpdateItemTitle();
			m_lCurrSel = lOldSel;

			if (m_lDataNum)
			{
				if (m_lCurrSel >= m_lDataNum)
					m_lCurrSel--;
			}
			else
				m_lCurrSel = -1;
		}
		break;
	default:
		break;
	}

	if (m_lCurrSel != -1)
		lc.SetItemState(m_lCurrSel, LVIS_SELECTED, LVIS_SELECTED);
	m_DataManager.SetParam(this, m_lCurrSel);
 
	GetDocument()->m_bIsDataChanged = TRUE;
}

void CMyLTView::ChangePeriod()
{
	LT_DATA_INFO *pDataInfoTemp = m_pDataInfo;

	for (long a = 0; a < m_lDataNum; a++, pDataInfoTemp++)
	{
		pDataInfoTemp->lPeriodID = a;
	}
}

void CMyLTView::UpdateItemTitle()
{
	CListCtrl &lc = GetListCtrl();
	CString strItem;
	
	LT_DATA_INFO *pDataInfoTemp = m_pDataInfo;
	for (long a = 0; a < m_lDataNum; a++, pDataInfoTemp++)
	{
		strItem.Format("%03d", pDataInfoTemp->lPeriodID);
		lc.SetItemText(a, 0, strItem);
	}
}

LT_DATA_INFO *CMyLTView::GetNewDataInfo(long &lNewDataNum)
{
	lNewDataNum = m_lDataNum;
	return m_pDataInfo;
}

void CMyLTView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_lSelFrom)
		OnDataManager();
	CListView::OnLButtonDblClk(nFlags, point);
}

void CMyLTView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CListCtrl &lc = GetListCtrl();
	POSITION pos = lc.GetFirstSelectedItemPosition();
	if (pos)
		m_lCurrSel = lc.GetNextSelectedItem(pos);
	else
		m_lCurrSel = -1;
	
	if (m_DataManager.m_hWnd && m_DataManager.IsWindowVisible())
	{		
		m_DataManager.SetParam(this, m_lCurrSel);
	}	
	*pResult = 0;
}

void CMyLTView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pActivateView != pDeactiveView)
	{
		if (bActivate)
		{
			if (m_bCurrShow)
			{
				m_DataManager.ShowWindow(SW_SHOW);
			}
		}
		else
		{
			if (m_bCurrShow)
			{
				m_DataManager.ShowWindow(SW_HIDE);
			}
		}
	}
	CListView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CMyLTView::OnDestroy() 
{
	CListView::OnDestroy();
	
	// TODO: Add your message handler code here
	m_DataManager.DestroyWindow();
}

void CMyLTView::OnDataManager() 
{
	// TODO: Add your command handler code here
	if (!m_bCurrShow)
	{
		m_bCurrShow = TRUE;
		m_DataManager.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	}
	
	m_DataManager.SetParam(this, m_lCurrSel);
}

void CMyLTView::OnUpdateDataManager(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CView *pView = ((CFrameWnd *)AfxGetMainWnd())->GetActiveView();
	pCmdUI->Enable(pView == this && m_lSelFrom);
}
