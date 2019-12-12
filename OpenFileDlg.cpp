// OpenFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "OpenFileDlg.h"

#include "MyLTDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenFileDlg dialog


COpenFileDlg::COpenFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenFileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLstFile = NULL;
	m_RetFileInfo = NULL;
}


void COpenFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenFileDlg)
	DDX_Control(pDX, IDC_LST_PROVINCE, m_wndListProvince);
	DDX_Control(pDX, IDC_CB_PROVINCE, m_wndProvince);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenFileDlg, CDialog)
	//{{AFX_MSG_MAP(COpenFileDlg)
	ON_CBN_SELCHANGE(IDC_CB_PROVINCE, OnSelchangeCbProvince)
	ON_BN_CLICKED(IDC_BTN_DELFILE, OnBtnDelfile)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_PROVINCE, OnDblclkLstProvince)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenFileDlg message handlers

BOOL COpenFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMyLTDoc *pDoc = (CMyLTDoc *)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();
	m_pLstFile = pDoc->GetFileList();

	m_wndListProvince.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); 
	m_wndListProvince.InsertColumn(0, "序号", LVCFMT_LEFT, 50, 0);
	m_wndListProvince.InsertColumn(1, "省份", LVCFMT_LEFT, 50, 1);
	m_wndListProvince.InsertColumn(2, "描述", LVCFMT_LEFT, 240, 2);
	m_wndListProvince.InsertColumn(3, "文件名", LVCFMT_LEFT, 100, 3);

	m_wndProvince.AddString("所有");
	for (long a = 0; a < g_DataInfo.GetProvinceNum(); a++)
	{
		m_wndProvince.AddString(g_DataInfo.GetProvinceName(a));
	}

	m_wndProvince.SetCurSel(theApp.m_SysCfgInfo.lDefProvince + 1);
	OnSelchangeCbProvince();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COpenFileDlg::OnSelchangeCbProvince() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndProvince.GetCurSel();

	m_wndListProvince.DeleteAllItems();

	CString strItem;
	long lItem = 0;

	CMyLTDoc *pDoc = (CMyLTDoc *)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();
	CString strCurrFileName = pDoc->m_strFileName;

	POSITION pos = m_pLstFile->GetHeadPosition();
	while (pos)
	{
		LT_FILE_INFO *pFileInfo = m_pLstFile->GetNext(pos);

		if ((nIndex == 0 || (nIndex == pFileInfo->lProvince + 1))
			 && strCurrFileName.Compare(pFileInfo->szName) != 0)				
		{
			strItem.Format("%d", lItem);

			m_wndListProvince.InsertItem(lItem, strItem);
			m_wndListProvince.SetItemText(lItem, 1, g_DataInfo.GetProvinceName(pFileInfo->lProvince));
			m_wndListProvince.SetItemText(lItem, 2, pFileInfo->szDesp);
			m_wndListProvince.SetItemText(lItem, 3, pFileInfo->szName);

			m_wndListProvince.SetItemData(lItem, (DWORD)pFileInfo);

			lItem++;
		}
	}
}

void COpenFileDlg::OnBtnDelfile() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_wndListProvince.GetFirstSelectedItemPosition();
	if (pos)
	{
		if (IDOK == theApp.MsgBox("您是否确定要删除所选择的文件?", MB_OKCANCEL | MB_ICONINFORMATION))
		{
			int nIndex = m_wndListProvince.GetNextSelectedItem(pos);
			LT_FILE_INFO *pFileInfo = (LT_FILE_INFO *)m_wndListProvince.GetItemData(nIndex);

			CMyLTDoc *pDoc = (CMyLTDoc *)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();
			pDoc->DeleteFile(pFileInfo->szName);

			OnSelchangeCbProvince();
		}
	}
	else
		theApp.MsgBox("请选择要删除的数据文件!");
}

void COpenFileDlg::OnDblclkLstProvince(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (m_wndListProvince.GetFirstSelectedItemPosition() != NULL)
	{
		COpenFileDlg::OnOK();
	}
}

void COpenFileDlg::OnOK() 
{
	// TODO: Add extra validation here
	POSITION pos = m_wndListProvince.GetFirstSelectedItemPosition();
	if (pos)
	{
		int nIndex = m_wndListProvince.GetNextSelectedItem(pos);
		m_RetFileInfo = (LT_FILE_INFO *)m_wndListProvince.GetItemData(nIndex);
	}
	else
	{
		theApp.MsgBox("请选择要打开的数据文件!");
		return;
	}

	CDialog::OnOK();
}

BOOL COpenFileDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DestroyWindow();
}

