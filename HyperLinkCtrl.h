#if !defined(AFX_HYPERLINKCTRL_H__EE588059_D6BC_4EDB_AC82_67823417C975__INCLUDED_)
#define AFX_HYPERLINKCTRL_H__EE588059_D6BC_4EDB_AC82_67823417C975__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HyperLinkCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHyperLinkCtrl window

class CHyperLinkCtrl : public CStatic
{
// Construction
public:
	CHyperLinkCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHyperLinkCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHyperLinkCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHyperLinkCtrl)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()

public:
	void SetHyperLink(LPCTSTR lpszHyperLink);

private:
	CString m_strHyperLink;
	BOOL m_bHaveClicked;
	CFont m_FontNomal, m_FontFocus;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYPERLINKCTRL_H__EE588059_D6BC_4EDB_AC82_67823417C975__INCLUDED_)
