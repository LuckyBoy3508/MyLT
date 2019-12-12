// SysInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "SysInfoDlg.h"

#include "MyLTDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysInfoDlg dialog


CSysInfoDlg::CSysInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSysInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysInfoDlg)
	m_bAutoLoad = FALSE;
	//}}AFX_DATA_INIT
	memset(&m_SysCfgInfo, 0, sizeof(LT_SYSCFG_INFO));
}


void CSysInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysInfoDlg)
	DDX_Control(pDX, IDC_CB_PROVINCE, m_wndProvince);
	DDX_Control(pDX, IDC_CB_FILELIST, m_wndFileInfo);
	DDX_Check(pDX, IDC_CK_AUTOLOAD, m_bAutoLoad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CSysInfoDlg)
	ON_BN_CLICKED(IDC_CK_AUTOLOAD, OnCkAutoload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysInfoDlg message handlers
void CSysInfoDlg::SetParam(LT_SYSCFG_INFO *pSysInfo)
{
	memcpy(&m_SysCfgInfo, pSysInfo, sizeof(LT_SYSCFG_INFO));
}

BOOL CSysInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMyLTDoc *pDoc = (CMyLTDoc *)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();
	FILELIST *pLstFile = pDoc->GetFileList();

	POSITION pos = pLstFile->GetHeadPosition();
	while (pos)
	{
		LT_FILE_INFO *p = pLstFile->GetNext(pos);

		CString strItem;
		strItem.Format("%s-%s-%s", g_DataInfo.GetProvinceName(p->lProvince), p->szDesp, p->szName);
		m_wndFileInfo.SetItemData(m_wndFileInfo.AddString(strItem), (DWORD)p);
	}

	m_wndProvince.AddString("Ыљга");
	for (long a = 0; a < g_DataInfo.GetProvinceNum(); a++)
	{
		m_wndProvince.AddString(g_DataInfo.GetProvinceName(a));
	}
	
	m_bAutoLoad = m_SysCfgInfo.bIsAutoLoad;
	if (m_bAutoLoad)
	{
		for (long a = 0; a < m_wndFileInfo.GetCount(); a++)
		{
			LT_FILE_INFO *p = (LT_FILE_INFO *)m_wndFileInfo.GetItemData(a);
			if (strcmp(p->szName, m_SysCfgInfo.szFileName) == 0)
			{
				m_wndFileInfo.SetCurSel(a);
				break;
			}
		}
	}
	else
		m_wndFileInfo.EnableWindow(m_bAutoLoad);

	m_wndProvince.SetCurSel(m_SysCfgInfo.lDefProvince + 1);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSysInfoDlg::OnCkAutoload() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_wndFileInfo.EnableWindow(m_bAutoLoad);
}

void CSysInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	m_SysCfgInfo.bIsAutoLoad = m_bAutoLoad;
	if (m_bAutoLoad)
	{
		int nIndex = m_wndFileInfo.GetCurSel();

		LT_FILE_INFO *p = (LT_FILE_INFO *)m_wndFileInfo.GetItemData(nIndex);
		strcpy(m_SysCfgInfo.szFileName, p->szName);
	}

	m_SysCfgInfo.lDefProvince = m_wndProvince.GetCurSel() - 1;
	CDialog::OnOK();
}

BOOL CSysInfoDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DestroyWindow();
}
