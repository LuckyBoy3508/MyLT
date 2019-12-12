#if !defined(AFX_DATAMANAGER_H__22E14F08_BCB8_4BB3_B1AF_20B5F664B397__INCLUDED_)
#define AFX_DATAMANAGER_H__22E14F08_BCB8_4BB3_B1AF_20B5F664B397__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataManager.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataManager dialog
class CMyLTDoc;
class CMyLTView;
class CDataManager : public CDialog
{
// Construction
public:
	CDataManager(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataManager)
	enum { IDD = IDD_DLG_DATAMANAGER };
	CComboBox	m_wndNo8;
	CComboBox	m_wndNo7;
	CComboBox	m_wndNo6;
	CComboBox	m_wndNo5;
	CComboBox	m_wndNo4;
	CComboBox	m_wndNo3;
	CComboBox	m_wndNo2;
	CComboBox	m_wndNo1;
	CString	m_strDevote;
	CString	m_strPeriod;
	CString	m_strDate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataManager)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataManager)
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnInsert();
	afx_msg void OnBtnModify();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnExit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetCurrDoc(CMyLTDoc *pDoc);
	void SetParam(CMyLTView *pView, long lCurrSel);
		
private:
	void EnableStatics();
	void EnableBtns(BOOL bEnable);
	LPCTSTR GetNewData();

	CMyLTDoc* m_pDoc;
	long m_lSelFrom, m_lSelFor;
	BOOL m_bHaveSpec;

	CMyLTView *m_pView;
	long m_lCurrSel;

	LT_DATA_INFO m_NewData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMANAGER_H__22E14F08_BCB8_4BB3_B1AF_20B5F664B397__INCLUDED_)
