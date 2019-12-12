// HyperLinkCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "HyperLinkCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperLinkCtrl

CHyperLinkCtrl::CHyperLinkCtrl()
{
	LOGFONT logfont[2] = 
	{	
		{-12, 0, 0, 0, 400, 0, 0, 0, 134, 3, 2, 1, 2, "ËÎÌו"},
		{-12, 0, 0, 0, 700, 0, 1, 0, 134, 3, 2, 1, 2, "ËÎÌו"},
	};
	
	m_FontNomal.CreateFontIndirect(&logfont[0]);
	m_FontFocus.CreateFontIndirect(&logfont[1]);

	m_bHaveClicked = FALSE;
}

CHyperLinkCtrl::~CHyperLinkCtrl()
{
}


BEGIN_MESSAGE_MAP(CHyperLinkCtrl, CStatic)
	//{{AFX_MSG_MAP(CHyperLinkCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	//ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperLinkCtrl message handlers
void CHyperLinkCtrl::SetHyperLink(LPCTSTR lpszHyperLink)
{
	if (lpszHyperLink)
		m_strHyperLink = lpszHyperLink;
}

void CHyperLinkCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (m_strHyperLink.IsEmpty())
		GetWindowText(m_strHyperLink);

	int nOldMode = dc.SetBkMode(TRANSPARENT);
	
	CFont *pOldFont = dc.SelectObject(&m_FontNomal);
	COLORREF clrUse = RGB(0, 0, 255);
	if (m_bHaveClicked)
		clrUse = RGB(128, 0, 128);

	dc.SetTextColor(clrUse);

	CRect rt;
	GetClientRect(rt);
	dc.DrawText(m_strHyperLink, rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	dc.SetBkMode(nOldMode);
	dc.SelectObject(pOldFont);
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CHyperLinkCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bHaveClicked = TRUE;	
	ShellExecute(GetSafeHwnd(), "open", m_strHyperLink, NULL, NULL, SW_SHOW);
	Invalidate();
	//CStatic::OnLButtonDown(nFlags, point);
}

BOOL CHyperLinkCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
#if(WINVER >= 0x0500)
	::SetCursor(::LoadCursor(NULL, IDC_HAND));
#else
	::SetCursor(::LoadCursor(NULL, IDC_CROSS));
#endif

	return TRUE;
}
