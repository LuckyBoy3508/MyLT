#if !defined(AFX_FILEDOWNLOADDLG_H__89376381_690B_4A56_8658_DD0AF36B2108__INCLUDED_)
#define AFX_FILEDOWNLOADDLG_H__89376381_690B_4A56_8658_DD0AF36B2108__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileDownLoadDlg.h : header file
//

#include "HyperLinkCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CFileDownLoadDlg dialog

class CFileDownLoadDlg : public CDialog
{
// Construction
public:
	CFileDownLoadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileDownLoadDlg)
	enum { IDD = IDD_DLG_DOWNLOAD };
	CHyperLinkCtrl	m_wndHomepage;
	CAnimateCtrl	m_wndAniDownLoad;
	CProgressCtrl	m_wndDLProcess;
	CComboBox	m_wndProvince;
	CListCtrl	m_wndListProvince;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDownLoadDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileDownLoadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCbProvince();
	afx_msg void OnBtnGetfilelist();
	virtual void OnOK();
	afx_msg void OnBtnDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL DownLoadFile();
	void ReadFileList();
	long GetProvinceIndex(LPCTSTR lpszProvince);
	void DeleteAllTmpFile();
	
	CString m_strLocalFile, m_strRemoteFile;

	LT_FILE_INFO *m_pFileInfo;
	long m_lFileInfoNum;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDOWNLOADDLG_H__89376381_690B_4A56_8658_DD0AF36B2108__INCLUDED_)
