#if !defined(AFX_DRAWRESSETUPDLG_H__E1252ED7_5A5B_4E7A_BBB7_80F532786ADD__INCLUDED_)
#define AFX_DRAWRESSETUPDLG_H__E1252ED7_5A5B_4E7A_BBB7_80F532786ADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawResSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawResSetupDlg dialog

class CDrawResSetupDlg : public CDialog
{
// Construction
public:
	CDrawResSetupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDrawResSetupDlg)
	enum { IDD = IDD_DLG_DRAWRES };
	CStatic	m_wndFont;
	CStatic	m_wndColor;
	CComboBox	m_wndFontName;
	CComboBox	m_wndClrName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawResSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDrawResSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCbClrname();
	afx_msg void OnSelchangeCbFontname();
	afx_msg void OnStaColor();
	afx_msg void OnStaFont();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetParam(COLORREF clrParam[3], LOGFONT logFont[6]);

private:
	COLORREF *m_pClrParam;
	LOGFONT *m_pLogFont;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWRESSETUPDLG_H__E1252ED7_5A5B_4E7A_BBB7_80F532786ADD__INCLUDED_)
