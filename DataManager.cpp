// DataManager.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "DataManager.h"

#include "MyLTDoc.h"
#include "MyLTView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataManager dialog


CDataManager::CDataManager(CWnd* pParent /*=NULL*/)
	: CDialog(CDataManager::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataManager)
	m_strDevote = _T("");
	m_strPeriod = _T("");
	m_strDate = _T("");
	//}}AFX_DATA_INIT

	//Create(CDataManager::IDD);
	m_pDoc = NULL;
	m_lSelFrom = m_lSelFor = 0;
	m_bHaveSpec = FALSE;
	
	m_pView = NULL;
	m_lCurrSel = -1;

	memset(&m_NewData, 0, sizeof(LT_DATA_INFO));
}


void CDataManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataManager)
	DDX_Control(pDX, IDC_CB_NO_8, m_wndNo8);
	DDX_Control(pDX, IDC_CB_NO_7, m_wndNo7);
	DDX_Control(pDX, IDC_CB_NO_6, m_wndNo6);
	DDX_Control(pDX, IDC_CB_NO_5, m_wndNo5);
	DDX_Control(pDX, IDC_CB_NO_4, m_wndNo4);
	DDX_Control(pDX, IDC_CB_NO_3, m_wndNo3);
	DDX_Control(pDX, IDC_CB_NO_2, m_wndNo2);
	DDX_Control(pDX, IDC_CB_NO_1, m_wndNo1);
	DDX_Text(pDX, IDC_EDT_DEVOTE, m_strDevote);
	DDX_Text(pDX, IDC_EDT_PERIOD, m_strPeriod);
	DDX_Text(pDX, IDC_EDT_DATE, m_strDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataManager, CDialog)
	//{{AFX_MSG_MAP(CDataManager)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_INSERT, OnBtnInsert)
	ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataManager message handlers
void CDataManager::SetCurrDoc(CMyLTDoc *pDoc)
{
	m_pDoc = pDoc;
	m_lSelFrom = pDoc->m_lSelFrom;
	m_lSelFor = pDoc->m_lSelFor;
	m_bHaveSpec = pDoc->m_bHaveSpec;

	m_strDevote = _T("");
	m_strPeriod = _T("");
	m_strDate = _T("");

	m_wndNo1.ResetContent();
	m_wndNo2.ResetContent();
	m_wndNo3.ResetContent();
	m_wndNo4.ResetContent();
	m_wndNo5.ResetContent();
	m_wndNo6.ResetContent();
	m_wndNo7.ResetContent();
	m_wndNo8.ResetContent();
	CString strItem;
	for (long a = 0; a < m_lSelFrom; a++)
	{
		strItem.Format("%d", a + 1);
		m_wndNo1.AddString(strItem);
		m_wndNo2.AddString(strItem);
		m_wndNo3.AddString(strItem);
		m_wndNo4.AddString(strItem);
		m_wndNo5.AddString(strItem);
		m_wndNo6.AddString(strItem);
		m_wndNo7.AddString(strItem);
		m_wndNo8.AddString(strItem);
	}

	EnableStatics();

	m_lCurrSel = -1;
	EnableBtns(m_lCurrSel != -1);

	UpdateData(FALSE);
}

void CDataManager::EnableStatics()
{
	UINT uIDs[2][8] = 
	{
		{
			IDC_STA_NO1, IDC_STA_NO2, IDC_STA_NO3, IDC_STA_NO4,
			IDC_STA_NO5, IDC_STA_NO6, IDC_STA_NO7, IDC_STA_NO8,
		},
		{
			IDC_CB_NO_1, IDC_CB_NO_2, IDC_CB_NO_3, IDC_CB_NO_4,
			IDC_CB_NO_5, IDC_CB_NO_6, IDC_CB_NO_7, IDC_CB_NO_8,
		},
	};

	long lShowFlag[LT_MAX_SELFOR] = {0};

	long lNormalNum = m_lSelFor;
	if (m_bHaveSpec)
	{
		lShowFlag[LT_MAX_SELFOR - 1] = 1;
		lNormalNum--;
	}
	
	for (long a = 0; a < lNormalNum; a++)
		lShowFlag[a] = 1;

	for (long a = 0; a < LT_MAX_SELFOR; a++)
	{
		GetDlgItem(uIDs[0][a])->ShowWindow(lShowFlag[a] ? SW_SHOW : SW_HIDE);
		GetDlgItem(uIDs[1][a])->ShowWindow(lShowFlag[a] ? SW_SHOW : SW_HIDE);
	}
}

void CDataManager::SetParam(CMyLTView *pView, long lCurrSel)
{
	m_pView = pView;
	m_lCurrSel = lCurrSel;

	CComboBox *pWndNo[] = 
	{
		&m_wndNo1, &m_wndNo2, &m_wndNo3, &m_wndNo4,
		&m_wndNo5, &m_wndNo6, &m_wndNo7, &m_wndNo8,
	};

	if (m_lCurrSel != -1)
	{
		long lDataNum = 0;
		LT_DATA_INFO *pDataInfo = pView->GetNewDataInfo(lDataNum);
		ASSERT(m_lCurrSel < lDataNum);

		pDataInfo += m_lCurrSel;
		m_strPeriod = pDataInfo->szPeriod;
		m_strDate = pDataInfo->szDate;
		m_strDevote.Format("%d", pDataInfo->lTotalDev);

		for (long a = 0; a < m_lSelFor; a++)
		{
			if (a == m_lSelFor - 1 && m_bHaveSpec)
				pWndNo[7]->SetCurSel(pDataInfo->lData[a] - 1);
			else
				pWndNo[a]->SetCurSel(pDataInfo->lData[a] - 1);
		}
	}
	else 
	{	
		m_strPeriod = "";
		m_strDate = "";
		m_strDevote = "";

		for (long a = 0; a < m_lSelFor; a++)
		{
			pWndNo[a]->SetCurSel(a);
		}
	}

	EnableBtns(m_lCurrSel != -1);

	UpdateData(FALSE);
}

void CDataManager::EnableBtns(BOOL bEnable)
{
	UINT uIDs[] = {IDC_BTN_INSERT, IDC_BTN_MODIFY, IDC_BTN_DEL};

	for (long a = 0; a < sizeof(uIDs) / sizeof(uIDs[0]); a++)
	{
		GetDlgItem(uIDs[a])->EnableWindow(bEnable);
	}
}

void CDataManager::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
	LPCTSTR lpszErr = GetNewData();
	if (!lpszErr)
		m_pView->ChangeData(CMyLTView::LCT_ADD, &m_NewData);
	else
		theApp.MsgBox(lpszErr, MB_OK | MB_ICONSTOP, this);
}

void CDataManager::OnBtnInsert() 
{
	// TODO: Add your control notification handler code here
	LPCTSTR lpszErr = GetNewData();
	if (!lpszErr)
		m_pView->ChangeData(CMyLTView::LCT_INSERT, &m_NewData);
	else
		theApp.MsgBox(lpszErr, MB_OK | MB_ICONSTOP, this);
}

void CDataManager::OnBtnModify() 
{
	// TODO: Add your control notification handler code here
	LPCTSTR lpszErr = GetNewData();
	if (!lpszErr)
		m_pView->ChangeData(CMyLTView::LCT_MODIFY, &m_NewData);
	else
		theApp.MsgBox(lpszErr, MB_OK | MB_ICONSTOP, this);
}

void CDataManager::OnBtnDel() 
{
	// TODO: Add your control notification handler code here
	ASSERT(m_lCurrSel != -1);
	m_pView->ChangeData(CMyLTView::LCT_DELETE, NULL);
}

void CDataManager::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	if (m_pView)
		m_pView->m_bCurrShow = FALSE;
	ShowWindow(SW_HIDE);
}

LPCTSTR CDataManager::GetNewData()
{
	UpdateData();
	memset(&m_NewData, 0, sizeof(LT_DATA_INFO));

	strcpy(m_NewData.szPeriod, m_strPeriod);
	strcpy(m_NewData.szDate, m_strDate);
	m_NewData.lTotalDev = atol(m_strDevote);

	CComboBox *pWndNo[] = 
	{
		&m_wndNo1, &m_wndNo2, &m_wndNo3, &m_wndNo4,
		&m_wndNo5, &m_wndNo6, &m_wndNo7, &m_wndNo8,
	};

	for (long a = 0; a < m_lSelFor; a++)
	{
		if (a == m_lSelFor - 1 && m_bHaveSpec)
			m_NewData.lData[a] = pWndNo[7]->GetCurSel() + 1;
		else
			m_NewData.lData[a] = pWndNo[a]->GetCurSel() + 1;
	}

	return CMyLTDoc::IsDataValid(&m_NewData, m_lSelFrom, m_lSelFor);
}


BOOL CDataManager::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
