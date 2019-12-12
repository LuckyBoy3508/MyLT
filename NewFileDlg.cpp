// NewFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "NewFileDlg.h"

#include "MyLTDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewFileDlg dialog


CNewFileDlg::CNewFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewFileDlg)
	m_bIsHaveSpec = FALSE;
	m_strDesp = _T("");
	m_strPathName = _T("");
	//}}AFX_DATA_INIT
	m_lProvince = 0;
	m_lSelFrom = m_lSelFor = 0;
}


void CNewFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewFileDlg)
	DDX_Control(pDX, IDC_CB_PROVINCE, m_wndProvince);
	DDX_Control(pDX, IDC_CB_TO, m_wndTo);
	DDX_Control(pDX, IDC_CB_FROM, m_wndFrom);
	DDX_Check(pDX, IDC_CK_HAVESPEC, m_bIsHaveSpec);
	DDX_Text(pDX, IDC_EDT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDT_FILENAME, m_strPathName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewFileDlg, CDialog)
	//{{AFX_MSG_MAP(CNewFileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewFileDlg message handlers
BOOL CNewFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	long a;
	CString strItem;

	for (a = 0; a < g_DataInfo.GetProvinceNum(); a++)
	{
		m_wndProvince.AddString(g_DataInfo.GetProvinceName(a));
	}
	long lCurSel = (theApp.m_SysCfgInfo.lDefProvince == -1) ? 1 : theApp.m_SysCfgInfo.lDefProvince;
	m_wndProvince.SetCurSel(lCurSel);

	for (a = LT_MIN_SELFROM; a <= LT_MAX_SELFROM; a++)
	{
		strItem.Format("%d", a);
		m_wndFrom.AddString(strItem);
	}
	m_wndFrom.SetCurSel(LT_DEF_SELFROM - LT_MIN_SELFROM);

	for (a = LT_MIN_SELFOR; a <= LT_MAX_SELFOR; a++)
	{
		strItem.Format("%d", a - 1);
		m_wndTo.AddString(strItem);
	}
	m_wndTo.SetCurSel(LT_DEF_SELFOR - LT_MIN_SELFOR);

	m_bIsHaveSpec = LT_DEF_HAVESPECNO;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewFileDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if (m_strPathName.IsEmpty())
	{
		theApp.MsgBox("请输入文件名称!");
		return;
	}
	else
	{
		m_strPathName += ".xml";

		CMyLTDoc *pDoc = (CMyLTDoc *)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();
		if (pDoc->IsHaveFile(m_strPathName))
		{
			if (IDOK == theApp.MsgBox("已经存在该文件，是否覆盖?", MB_OKCANCEL | MB_ICONINFORMATION))
			{
				pDoc->DeleteFile(m_strPathName);
			}
			else
				return;
		}
	}

	if (m_strDesp.IsEmpty())
	{
		theApp.MsgBox("请输入文件描述信息!");
		return;
	}

	m_lProvince = m_wndProvince.GetCurSel();
	m_lSelFrom = m_wndFrom.GetCurSel() + LT_MIN_SELFROM;
	m_lSelFor = m_wndTo.GetCurSel() + LT_MIN_SELFOR;
	if (!m_bIsHaveSpec)
		m_lSelFor--;

	//不能用m_strPathName，因为OnOK会重新调用UpdataData()函数，结果会使
	//刚才加上的".xml"又去掉了
	m_strRetName = m_strPathName;

	CDialog::OnOK();
}

