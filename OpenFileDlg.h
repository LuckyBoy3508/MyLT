#if !defined(AFX_OPENFILEDLG_H__50776BA5_2080_42F0_A481_EA1FDBBB8EA9__INCLUDED_)
#define AFX_OPENFILEDLG_H__50776BA5_2080_42F0_A481_EA1FDBBB8EA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenFileDlg dialog

class COpenFileDlg : public CDialog
{
// Construction
public:
	COpenFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COpenFileDlg)
	enum { IDD = IDD_DLG_OPENFILE };
	CListCtrl	m_wndListProvince;
	CComboBox	m_wndProvince;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenFileDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenFileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCbProvince();
	virtual void OnOK();
	afx_msg void OnBtnDelfile();
	afx_msg void OnDblclkLstProvince(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	FILELIST *m_pLstFile;
	LT_FILE_INFO *m_RetFileInfo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENFILEDLG_H__50776BA5_2080_42F0_A481_EA1FDBBB8EA9__INCLUDED_)
