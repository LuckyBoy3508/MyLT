// MyLT.h : main header file for the MYLT application
//

#if !defined(AFX_MYLT_H__B4880F86_4214_42D7_BF92_10C87F0F4F91__INCLUDED_)
#define AFX_MYLT_H__B4880F86_4214_42D7_BF92_10C87F0F4F91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "Global.h"
/////////////////////////////////////////////////////////////////////////////
// CMyLTApp:
// See MyLT.cpp for the implementation of this class
class CMyLTApp : public CWinApp
{
public:
	CMyLTApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyLTApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMyLTApp)
	afx_msg void OnFileExit();
	afx_msg void OnOptSyssetup();
	afx_msg void OnNetUpdate();
	afx_msg void OnNetRegister();
	afx_msg void OnHlpContent();
	afx_msg void OnHlpAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//main-path e.g. C:\MyLT
	CString m_strMainPath;
	CString m_strHistDataPath;
	CString m_strHistTextPath;
	CString m_strUserDataPath;
	CString m_strLogFilePath;
	CString m_strCfgFilePath;
	CString m_strDownLoadPath;

	//xml
	CDvXML m_DvXml;

	//system-option
	BOOL RWCfg(BOOL bIsRead = TRUE);
	LT_SYSCFG_INFO m_SysCfgInfo;

	//register-relate
	void ShowRegMsg();
	BOOL CheckReg();
	
	//helper-function.
	BOOL MsgBox(LPCTSTR lpszMsg, UINT uType = MB_OK | MB_ICONINFORMATION, CWnd *pParent = NULL);
	char *OpenFileDlg(BOOL bIsOpen, const char *pFilter, const char *pInitDir,
						const char *pDefExt = NULL);
};

extern CMyLTApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLT_H__B4880F86_4214_42D7_BF92_10C87F0F4F91__INCLUDED_)
