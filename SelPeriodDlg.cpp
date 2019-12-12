// SelPeriodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "SelPeriodDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelPeriodDlg dialog


CSelPeriodDlg::CSelPeriodDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelPeriodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelPeriodDlg)
	m_bAllowSel = FALSE;
	//}}AFX_DATA_INIT
	m_lDataNum = m_lFromPeriod = m_lToPeriod = 0;
	m_bIsApply = FALSE;
}


void CSelPeriodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelPeriodDlg)
	DDX_Control(pDX, IDC_CB_ENDPERIOD, m_wndTo);
	DDX_Control(pDX, IDC_CB_BEGINPERIOD, m_wndFrom);
	DDX_Check(pDX, IDC_CK_ALLOWSEL, m_bAllowSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelPeriodDlg, CDialog)
	//{{AFX_MSG_MAP(CSelPeriodDlg)
	ON_BN_CLICKED(IDC_CK_ALLOWSEL, OnCkAllowsel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelPeriodDlg message handlers

void CSelPeriodDlg::SetParam(long lDataNum, long lFromPeriod, long lToPeriod, BOOL bIsApply)
{
	m_lDataNum = lDataNum;
	m_lFromPeriod = lFromPeriod;
	m_lToPeriod = lToPeriod;
	m_bIsApply = bIsApply;
}


BOOL CSelPeriodDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bAllowSel = m_bIsApply;

	CString strItem;
	for (long a = 0; a < m_lDataNum; a++)
	{
		strItem.Format("%d", a);
		m_wndFrom.AddString(strItem);
		m_wndTo.AddString(strItem);
	}

	m_wndFrom.SetCurSel(m_lFromPeriod);
	m_wndTo.SetCurSel(m_lToPeriod);

	EnableCtrls();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelPeriodDlg::EnableCtrls()
{
	m_wndFrom.EnableWindow(m_bAllowSel);
	m_wndTo.EnableWindow(m_bAllowSel);
}

void CSelPeriodDlg::OnCkAllowsel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	EnableCtrls();
}

void CSelPeriodDlg::OnOK() 
{
	// TODO: Add extra validation here
#ifndef LT_REGISTERED_VERSION
	theApp.CheckReg();
#else
	UpdateData();

	m_lFromPeriod = m_wndFrom.GetCurSel();
	m_lToPeriod = m_wndTo.GetCurSel();
	m_bIsApply = m_bAllowSel;

	if (m_lFromPeriod > m_lToPeriod)
	{
		theApp.MsgBox("统计起始周期不能大于终止周期!");
		return;
	}

	CDialog::OnOK();
#endif
}

