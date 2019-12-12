// ReginsterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "ReginsterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReginsterDlg dialog


CReginsterDlg::CReginsterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReginsterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReginsterDlg)
	//}}AFX_DATA_INIT
	m_bIsTotal = TRUE;
}


void CReginsterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReginsterDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReginsterDlg, CDialog)
	//{{AFX_MSG_MAP(CReginsterDlg)
	ON_BN_CLICKED(IDC_BTN_MOREINFO, OnBtnMoreinfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReginsterDlg message handlers

BOOL CReginsterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetWindowRect(m_rtTotal);
	m_rtNormal.left = m_rtTotal.left;
	m_rtNormal.right = m_rtTotal.left + m_rtTotal.Width() / 2;
	m_rtNormal.top = m_rtTotal.top;
	m_rtNormal.bottom = m_rtTotal.bottom;

	CString strNormalInfo =
		"1: ���ܼ��\n"
		"   ����ͼ  �����ݲ鿴������\n"
		"   ������ͼ��ͼ������ҳ��������ݹ�ϵ\n"
		"   ͳ����ͼ��ѡ�š����š���ˮ\n\n"
		"2: ��ݼ�\n"
		"   �ڷ�����ͼ�п������ü��̿��Ƽ�\n"
		"   �����������������PageUp��PageDown��Home��End��Tab��Ctrl+����Ctrl+����Ctrl+Tab\n\n"
		"3: ��ʷ���ݸ�����ѡ��˵�������->��������\n\n"
		"4: ��Ϊ������ʹ�õ��Ƿ�ע��棬���һЩ�����ܵ����ƣ�����ȫ�����ܣ�����ע������\n\n"
		"5: �����Դ������ʲô��������뷢��E-mail��dfliu@163.com\n\n"
		"6: ��л���Թ��������֧��!\n\n"
		;
	SetDlgItemText(IDC_STA_NORMAL, strNormalInfo);

	CString strMoreInfo = 
		"1: ע���\n"
		"   ���汾ע��ѣ�68Ԫ\n\n"
		"2: ע�᷽��\n"
		"   (1) ί���й��������ע�����Ĵ���\n"
		"       ��ѡ��˵�������->���ע�ᣬ��½�й��������ע���������ո���վ��ʾ����ע�ᡣ\n"
				"ͬʱ����÷���E-mail��dfliu@163.com���Ա��ܼ�ʱ����Ϣ\n"
		"   (2) ֱ����������ϵ����\n"
		"       ����E-mail��dfliu@163.com��˵��Ҫע���������ƺ���ϵ��ʽ��\n"
		"       �����յ�ȷ�϶����ʼ���Ὣ��ʽ������ṩ��ʽ����������\n"
		;
	SetDlgItemText(IDC_STA_MOREINFO, strMoreInfo);

	//SetWindowPos(NULL, m_rtNormal.left, m_rtNormal.top, m_rtNormal.Width(), m_rtNormal.Height(),
	//				SWP_NOZORDER);
	CenterWindow();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReginsterDlg::OnBtnMoreinfo() 
{
	// TODO: Add your control notification handler code here
	CRect rtNow;
	CString strTxt;

	if (m_bIsTotal)
	{
		strTxt = "ע����Ϣ >>";
		rtNow = m_rtNormal;
	}
	else
	{
		strTxt = "<< ע����Ϣ";
		rtNow = m_rtTotal;
	}

	m_bIsTotal ^= 1;

	SetDlgItemText(IDC_BTN_MOREINFO, strTxt);
	SetWindowPos(NULL, rtNow.left, rtNow.top, rtNow.Width(), rtNow.Height(), SWP_NOZORDER);
	CenterWindow();
}
