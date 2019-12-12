// RgnTmplManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "RgnTmplManageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRgnTmplManageDlg dialog


CRgnTmplManageDlg::CRgnTmplManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRgnTmplManageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRgnTmplManageDlg)
	m_strTmplName = _T("");
	m_b90 = TRUE;
	m_b180 = TRUE;
	m_b270 = TRUE;
	m_bHMirror = TRUE;
	m_bVMirror = TRUE;
	//}}AFX_DATA_INIT
	m_lSelIndex = -1;
	m_pLstSameRgn = NULL;
}


void CRgnTmplManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRgnTmplManageDlg)
	DDX_Control(pDX, IDC_STA_TMPEDIT, m_wndTmplEdit);
	DDX_Control(pDX, IDC_LST_TMPLIST, m_wndTmplLst);
	DDX_Text(pDX, IDC_EDT_TMPNAME, m_strTmplName);
	DDX_Check(pDX, IDC_CK_90, m_b90);
	DDX_Check(pDX, IDC_CK_180, m_b180);
	DDX_Check(pDX, IDC_CK_270, m_b270);
	DDX_Check(pDX, IDC_CK_HMIRROR, m_bHMirror);
	DDX_Check(pDX, IDC_CK_VMIRROR, m_bVMirror);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRgnTmplManageDlg, CDialog)
	//{{AFX_MSG_MAP(CRgnTmplManageDlg)
	ON_LBN_SELCHANGE(IDC_LST_TMPLIST, OnSelchangeLstTmplist)
	ON_BN_CLICKED(IDC_BTN_TMPADD, OnBtnTmpadd)
	ON_BN_CLICKED(IDC_BTN_TMPDEL, OnBtnTmpdel)
	ON_BN_CLICKED(IDC_CK_90, OnCk90)
	ON_BN_CLICKED(IDC_CK_180, OnCk180)
	ON_BN_CLICKED(IDC_CK_270, OnCk270)
	ON_BN_CLICKED(IDC_CK_HMIRROR, OnCkHmirror)
	ON_BN_CLICKED(IDC_CK_VMIRROR, OnCkVmirror)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRgnTmplManageDlg message handlers

BOOL CRgnTmplManageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	DoInit();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRgnTmplManageDlg::SetSameRgnList(SAMERGNLIST *pLstSameRgn, long lCurrIndex)
{
	m_pLstSameRgn = pLstSameRgn;
	m_lSelIndex = lCurrIndex;	
}

void CRgnTmplManageDlg::DoInit()
{
	RWSameRgnlInfo(TRUE);

	POSITION pos = m_lstSameRgnInfo.GetHeadPosition();
	while(pos)
	{
		LTDC_SAMERGN_INFO *pTmp = m_lstSameRgnInfo.GetNext(pos);		
		m_wndTmplLst.SetItemData(m_wndTmplLst.AddString(pTmp->szName), (DWORD)(pTmp));			
	}

	if (m_wndTmplLst.GetCount() > m_lSelIndex)
	{
		m_wndTmplLst.SetCurSel(m_lSelIndex);
		OnSelchangeLstTmplist();
	}
}

void CRgnTmplManageDlg::RWSameRgnlInfo(BOOL bIsRead)
{
	if (bIsRead)
	{
		FreeTmplInfoList(m_pLstSameRgn);

		POSITION pos = m_pLstSameRgn->GetHeadPosition();
		while(pos)
		{
			LTDC_SAMERGN_INFO *pTmp = m_pLstSameRgn->GetNext(pos);

			LTDC_SAMERGN_INFO *pNew = new LTDC_SAMERGN_INFO;
			memcpy(pNew, pTmp, sizeof(LTDC_SAMERGN_INFO));

			m_lstSameRgnInfo.AddTail(pNew);
		}
	}
	else
	{
		FreeTmplInfoList(m_pLstSameRgn);

		POSITION pos = m_lstSameRgnInfo.GetHeadPosition();
		while(pos)
		{
			LTDC_SAMERGN_INFO *pTmp = m_lstSameRgnInfo.GetNext(pos);
			m_pLstSameRgn->AddTail(pTmp);
		}

		m_lstSameRgnInfo.RemoveAll();
	}
}

void CRgnTmplManageDlg::FreeTmplInfoList(SAMERGNLIST *pLstSameRgn)
{
	POSITION pos = pLstSameRgn->GetHeadPosition();
	while(pos)
	{
		LTDC_SAMERGN_INFO *pTmp = pLstSameRgn->GetNext(pos);
		delete pTmp;
	}

	pLstSameRgn->RemoveAll();
}

void CRgnTmplManageDlg::OnSelchangeLstTmplist() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndTmplLst.GetCurSel();
	if (nIndex != LB_ERR)
	{
		LTDC_SAMERGN_INFO *pTmp = (LTDC_SAMERGN_INFO *)m_wndTmplLst.GetItemData(nIndex);

		m_wndTmplEdit.SetMap(pTmp);
		m_strTmplName = pTmp->szName;
		m_b90 = pTmp->b90;
		m_b180 = pTmp->b180;
		m_b270 = pTmp->b270;
		m_bHMirror = pTmp->bHMirror;
		m_bVMirror = pTmp->bVMirror;
	}
	else
	{
		m_wndTmplEdit.SetMap(NULL);
		m_strTmplName = "";
	}

	UpdateData(FALSE);
}

void CRgnTmplManageDlg::OnBtnTmpadd() 
{
	// TODO: Add your control notification handler code here
	CString strErr = "";

	UpdateData();

	if (!m_strTmplName.IsEmpty())
	{
		if (!HaveSameNameTmpl(m_strTmplName))
		{
			LTDC_SAMERGN_INFO *pTmp = new LTDC_SAMERGN_INFO;
			memset(pTmp, 0, sizeof(LTDC_SAMERGN_INFO));
			m_lstSameRgnInfo.AddHead(pTmp);

			strcpy(pTmp->szName, m_strTmplName);
			pTmp->b90 = m_b90;
			pTmp->b180 = m_b180;
			pTmp->b270 = m_b270;
			pTmp->bHMirror = m_bHMirror;
			pTmp->bVMirror= m_bVMirror;

			int nIndex = m_wndTmplLst.AddString(m_strTmplName);

			m_wndTmplLst.SetItemData(nIndex, (DWORD)(pTmp));
			m_wndTmplLst.SetCurSel(nIndex);

			OnSelchangeLstTmplist();
		}
		else
			strErr = "模板名称已经存在！";
	}
	else
		strErr = "模板名称不能为空！";

	if (strErr.GetLength() > 0)
	{
		AfxMessageBox(strErr);
	}
}

BOOL CRgnTmplManageDlg::HaveSameNameTmpl(CString strTmplName)
{
	BOOL bRet = FALSE;

	POSITION pos = m_lstSameRgnInfo.GetHeadPosition();
	while(pos)
	{
		LTDC_SAMERGN_INFO *pTmp = m_lstSameRgnInfo.GetNext(pos);
		
		if (strTmplName.CompareNoCase(pTmp->szName) == 0)
		{
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

void CRgnTmplManageDlg::OnBtnTmpdel() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndTmplLst.GetCurSel();

	BOOL bDeleteOK = FALSE;
	LTDC_SAMERGN_INFO *pTmp = NULL;
	CString strName;
	if (nIndex != LB_ERR)
	{
		pTmp = (LTDC_SAMERGN_INFO *)m_wndTmplLst.GetItemData(nIndex);

		CString strMsg;
		strMsg.Format("确定要删除模板<%s>吗？", pTmp->szName);
		if (AfxMessageBox(strMsg) == IDOK)
		{
			m_wndTmplLst.DeleteString(nIndex);

			strName = pTmp->szName;
			bDeleteOK = TRUE;
		}
	}

	if (bDeleteOK)
	{
		//1. delete it.
		POSITION pos = m_lstSameRgnInfo.GetHeadPosition();
		while(pos)
		{
			POSITION posOld = pos;

			pTmp = m_lstSameRgnInfo.GetNext(pos);			
			if (strName.CompareNoCase(pTmp->szName) == 0)
			{
				delete pTmp;

				m_lstSameRgnInfo.RemoveAt(posOld);
				break;
			}
		}

		//2. change current selection.
		int nCount = m_wndTmplLst.GetCount();

		if (nCount < nIndex + 1)
			nIndex--;

		m_wndTmplLst.SetCurSel(nIndex);
		OnSelchangeLstTmplist();
	}
}


BOOL CRgnTmplManageDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	FreeTmplInfoList(&m_lstSameRgnInfo);

	return CDialog::DestroyWindow();
}

void CRgnTmplManageDlg::OnOK() 
{
	// TODO: Add extra validation here
	RWSameRgnlInfo(FALSE);
	m_lSelIndex = m_wndTmplLst.GetCurSel();

	CDialog::OnOK();
}

void CRgnTmplManageDlg::OnCk90() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int nIndex = m_wndTmplLst.GetCurSel();
	if (nIndex != LB_ERR)
	{
		LTDC_SAMERGN_INFO *pTmp = (LTDC_SAMERGN_INFO *)m_wndTmplLst.GetItemData(nIndex);
		pTmp->b90 = m_b90;
	}
}

void CRgnTmplManageDlg::OnCk180() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int nIndex = m_wndTmplLst.GetCurSel();
	if (nIndex != LB_ERR)
	{
		LTDC_SAMERGN_INFO *pTmp = (LTDC_SAMERGN_INFO *)m_wndTmplLst.GetItemData(nIndex);
		pTmp->b180 = m_b180;
	}	
}

void CRgnTmplManageDlg::OnCk270() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int nIndex = m_wndTmplLst.GetCurSel();
	if (nIndex != LB_ERR)
	{
		LTDC_SAMERGN_INFO *pTmp = (LTDC_SAMERGN_INFO *)m_wndTmplLst.GetItemData(nIndex);
		pTmp->b270 = m_b270;
	}		
}

void CRgnTmplManageDlg::OnCkHmirror() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int nIndex = m_wndTmplLst.GetCurSel();
	if (nIndex != LB_ERR)
	{
		LTDC_SAMERGN_INFO *pTmp = (LTDC_SAMERGN_INFO *)m_wndTmplLst.GetItemData(nIndex);
		pTmp->bHMirror = m_bHMirror;
	}		
}

void CRgnTmplManageDlg::OnCkVmirror() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int nIndex = m_wndTmplLst.GetCurSel();
	if (nIndex != LB_ERR)
	{
		LTDC_SAMERGN_INFO *pTmp = (LTDC_SAMERGN_INFO *)m_wndTmplLst.GetItemData(nIndex);
		pTmp->bVMirror = m_bVMirror;
	}		
}
