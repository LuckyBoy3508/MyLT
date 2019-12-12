#if !defined(AFX_NEWFILEDLG_H__C9E60411_AB4A_4BCB_856B_02E58791126A__INCLUDED_)
#define AFX_NEWFILEDLG_H__C9E60411_AB4A_4BCB_856B_02E58791126A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewFileDlg dialog

class CNewFileDlg : public CDialog
{
// Construction
public:
	CNewFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewFileDlg)
	enum { IDD = IDD_DLG_NEWFILE };
	CComboBox	m_wndProvince;
	CComboBox	m_wndTo;
	CComboBox	m_wndFrom;
	BOOL	m_bIsHaveSpec;
	CString	m_strDesp;
	CString	m_strPathName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewFileDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	long m_lProvince;
	long m_lSelFrom, m_lSelFor;
	CString m_strRetName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWFILEDLG_H__C9E60411_AB4A_4BCB_856B_02E58791126A__INCLUDED_)
