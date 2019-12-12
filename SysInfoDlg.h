#if !defined(AFX_SYSINFODLG_H__8E4C777C_9C97_4F28_97F8_20C02DBF50A7__INCLUDED_)
#define AFX_SYSINFODLG_H__8E4C777C_9C97_4F28_97F8_20C02DBF50A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysInfoDlg dialog

class CSysInfoDlg : public CDialog
{
// Construction
public:
	CSysInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSysInfoDlg)
	enum { IDD = IDD_DLG_SYSINFO };
	CComboBox	m_wndProvince;
	CComboBox	m_wndFileInfo;
	BOOL	m_bAutoLoad;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysInfoDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSysInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCkAutoload();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	FILELIST m_FileListInfo;
	void SetParam(LT_SYSCFG_INFO *pSysInfo);
	LT_SYSCFG_INFO m_SysCfgInfo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSINFODLG_H__8E4C777C_9C97_4F28_97F8_20C02DBF50A7__INCLUDED_)
