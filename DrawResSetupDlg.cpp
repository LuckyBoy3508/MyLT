// DrawResSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "DrawResSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawResSetupDlg dialog


CDrawResSetupDlg::CDrawResSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawResSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawResSetupDlg)
	//}}AFX_DATA_INIT
	m_pClrParam = NULL;
	m_pLogFont = NULL;
}


void CDrawResSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawResSetupDlg)
	DDX_Control(pDX, IDC_STA_FONT, m_wndFont);
	DDX_Control(pDX, IDC_STA_COLOR, m_wndColor);
	DDX_Control(pDX, IDC_CB_FONTNAME, m_wndFontName);
	DDX_Control(pDX, IDC_CB_CLRNAME, m_wndClrName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawResSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CDrawResSetupDlg)
	ON_CBN_SELCHANGE(IDC_CB_CLRNAME, OnSelchangeCbClrname)
	ON_CBN_SELCHANGE(IDC_CB_FONTNAME, OnSelchangeCbFontname)
	ON_BN_CLICKED(IDC_STA_COLOR, OnStaColor)
	ON_BN_CLICKED(IDC_STA_FONT, OnStaFont)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawResSetupDlg message handlers
void CDrawResSetupDlg::SetParam(COLORREF clrParam[3], LOGFONT logFont[6])
{
	m_pClrParam = clrParam;
	m_pLogFont = logFont;
}

BOOL CDrawResSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndClrName.AddString("背景");	
	m_wndClrName.AddString("外框");	
	m_wndClrName.AddString("内框");	
	m_wndClrName.SetCurSel(0);
	OnSelchangeCbClrname();

	m_wndFontName.AddString("大字体(中文)");
	m_wndFontName.AddString("中字体(中文)");
	m_wndFontName.AddString("小字体(中文)");
	m_wndFontName.AddString("大字体(英文)");
	m_wndFontName.AddString("中字体(英文)");
	m_wndFontName.AddString("小字体(英文)");
	m_wndFontName.SetCurSel(0);
	OnSelchangeCbFontname();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDrawResSetupDlg::OnSelchangeCbClrname() 
{
	// TODO: Add your control notification handler code here	
	int nIndex = m_wndClrName.GetCurSel();
	if (nIndex != CB_ERR)
	{
		m_wndColor.RedrawWindow();
	}
}

void CDrawResSetupDlg::OnSelchangeCbFontname() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndFontName.GetCurSel();

	CFont ft;
	ft.CreateFontIndirect(&m_pLogFont[nIndex]);
	m_wndFont.SetFont(&ft);
}

void CDrawResSetupDlg::OnStaColor() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;

	int nIndex = m_wndClrName.GetCurSel();
	dlg.m_cc.rgbResult = m_pClrParam[nIndex];

	if (dlg.DoModal() == IDOK)
	{
		m_pClrParam[nIndex] = dlg.m_cc.rgbResult;
		OnSelchangeCbClrname();
	}
}

void CDrawResSetupDlg::OnStaFont() 
{
	// TODO: Add your control notification handler code here
	CFontDialog dlg;
	dlg.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;

	int nIndex = m_wndFontName.GetCurSel();
	dlg.m_cf.lpLogFont = &m_pLogFont[nIndex];

	if (dlg.DoModal() == IDOK)
	{
		CFont ft;
		ft.CreateFontIndirect(&m_pLogFont[nIndex]);
		m_wndFont.SetFont(&ft);		
	}
}

HBRUSH CDrawResSetupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	static CBrush brs;
	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STA_COLOR)
		{
			int nIndex = m_wndClrName.GetCurSel();

			brs.DeleteObject();
			brs.CreateSolidBrush(m_pClrParam[nIndex]);
			hbr = (HBRUSH)brs.GetSafeHandle();

			pDC->SetBkMode(TRANSPARENT);
		}
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
