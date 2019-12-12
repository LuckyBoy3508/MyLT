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
		"1: 功能简介\n"
		"   主视图  －数据查看、管理\n"
		"   分析视图－图表分析找出各期数据关系\n"
		"   统计视图－选号、缩号、缩水\n\n"
		"2: 快捷键\n"
		"   在分析视图中可以试用键盘控制键\n"
		"   如←、→、↑、↓、PageUp、PageDown、Home、End、Tab、Ctrl+←、Ctrl+→，Ctrl+Tab\n\n"
		"3: 历史数据更新请选择菜单：网络->数据下载\n\n"
		"4: 因为您现在使用的是非注册版，因此一些功能受到限制，如需全部功能，请您注册此软件\n\n"
		"5: 如您对此软件有什么意见或建议请发送E-mail到dfliu@163.com\n\n"
		"6: 感谢您对共享软件的支持!\n\n"
		;
	SetDlgItemText(IDC_STA_NORMAL, strNormalInfo);

	CString strMoreInfo = 
		"1: 注册费\n"
		"   本版本注册费￥68元\n\n"
		"2: 注册方法\n"
		"   (1) 委托中国共享软件注册中心代理\n"
		"       请选择菜单：网络->软件注册，登陆中国共享软件注册中心依照该网站提示进行注册。\n"
				"同时您最好发送E-mail到dfliu@163.com，以便能及时回信息\n"
		"   (2) 直接与作者联系购买\n"
		"       发送E-mail到dfliu@163.com，说明要注册的软件名称和联系方式。\n"
		"       作者收到确认定购邮件后会将汇款方式和软件提供方式回馈给您。\n"
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
		strTxt = "注册信息 >>";
		rtNow = m_rtNormal;
	}
	else
	{
		strTxt = "<< 注册信息";
		rtNow = m_rtTotal;
	}

	m_bIsTotal ^= 1;

	SetDlgItemText(IDC_BTN_MOREINFO, strTxt);
	SetWindowPos(NULL, rtNow.left, rtNow.top, rtNow.Width(), rtNow.Height(), SWP_NOZORDER);
	CenterWindow();
}
