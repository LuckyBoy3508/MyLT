#if !defined(AFX_RGNTMPLMANAGEDLG_H__07ABF8EC_A1CC_4A61_BAC1_4B621E204EB6__INCLUDED_)
#define AFX_RGNTMPLMANAGEDLG_H__07ABF8EC_A1CC_4A61_BAC1_4B621E204EB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RgnTmplManageDlg.h : header file
//

#include "SameRgnTmplEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CRgnTmplManageDlg dialog

class CRgnTmplManageDlg : public CDialog
{
// Construction
public:
	CRgnTmplManageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRgnTmplManageDlg)
	enum { IDD = IDD_DLG_RGNEDIT };
	CSameRgnTmplEdit	m_wndTmplEdit;
	CListBox	m_wndTmplLst;
	CString	m_strTmplName;
	BOOL	m_b90;
	BOOL	m_b180;
	BOOL	m_b270;
	BOOL	m_bHMirror;
	BOOL	m_bVMirror;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRgnTmplManageDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRgnTmplManageDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeLstTmplist();
	afx_msg void OnBtnTmpadd();
	afx_msg void OnBtnTmpdel();
	afx_msg void OnCk90();
	afx_msg void OnCk180();
	afx_msg void OnCk270();
	afx_msg void OnCkHmirror();
	afx_msg void OnCkVmirror();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetSameRgnList(SAMERGNLIST *pLstSameRgn, long lCurrIndex);

	void DoInit();
	void RWSameRgnlInfo(BOOL bIsRead);
	void FreeTmplInfoList(SAMERGNLIST *pLstSameRgn);
	BOOL HaveSameNameTmpl(CString strTmplName);
	
	long m_lSelIndex;

private:
	SAMERGNLIST *m_pLstSameRgn;
	SAMERGNLIST m_lstSameRgnInfo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RGNTMPLMANAGEDLG_H__07ABF8EC_A1CC_4A61_BAC1_4B621E204EB6__INCLUDED_)
