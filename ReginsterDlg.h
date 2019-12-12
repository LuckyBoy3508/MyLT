#if !defined(AFX_REGINSTERDLG_H__6AF0132B_27DE_49C9_BAFD_7D0E21FA9FC3__INCLUDED_)
#define AFX_REGINSTERDLG_H__6AF0132B_27DE_49C9_BAFD_7D0E21FA9FC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReginsterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReginsterDlg dialog

class CReginsterDlg : public CDialog
{
// Construction
public:
	CReginsterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReginsterDlg)
	enum { IDD = IDD_DLG_REGISTER };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReginsterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReginsterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnMoreinfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CRect m_rtNormal, m_rtTotal;
	BOOL m_bIsTotal;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGINSTERDLG_H__6AF0132B_27DE_49C9_BAFD_7D0E21FA9FC3__INCLUDED_)
