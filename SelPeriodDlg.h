#if !defined(AFX_SELPERIODDLG_H__94E2DE8A_2510_441E_818F_814FCE09E950__INCLUDED_)
#define AFX_SELPERIODDLG_H__94E2DE8A_2510_441E_818F_814FCE09E950__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelPeriodDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelPeriodDlg dialog

class CSelPeriodDlg : public CDialog
{
// Construction
public:
	CSelPeriodDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelPeriodDlg)
	enum { IDD = IDD_DLG_SELPERIOD };
	CComboBox	m_wndTo;
	CComboBox	m_wndFrom;
	BOOL	m_bAllowSel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelPeriodDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelPeriodDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCkAllowsel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetParam(long lDataNum, long lFromPeriod, long lToPeriod, BOOL bIsApply);
	long m_lDataNum, m_lFromPeriod, m_lToPeriod;
	BOOL m_bIsApply;

private:
	void EnableCtrls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELPERIODDLG_H__94E2DE8A_2510_441E_818F_814FCE09E950__INCLUDED_)
