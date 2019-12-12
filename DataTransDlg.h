#if !defined(AFX_DATATRANSDLG_H__6C48C3B4_4CC8_4672_8CF9_421F121DBAAD__INCLUDED_)
#define AFX_DATATRANSDLG_H__6C48C3B4_4CC8_4672_8CF9_421F121DBAAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataTransDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataTransDlg dialog
#define LT_DT_MAXFIELD		50
#define LT_DT_USEDFIELD		3 + LT_MAX_SELFOR
#define LT_DT_FILEDLEN		50

class CDataTransDlg : public CDialog
{
// Construction
public:
	CDataTransDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataTransDlg)
	enum { IDD = IDD_DLG_DATATRANS };
	CComboBox	m_wndProvince;
	CStatic	m_wndStaNo8;
	CStatic	m_wndStaNo7;
	CStatic	m_wndStaNo6;
	CStatic	m_wndStaNo5;
	CStatic	m_wndStaNo4;
	CStatic	m_wndStaNo3;
	CStatic	m_wndStaNo2;
	CStatic	m_wndStaNo1;
	CComboBox	m_wndTo;
	CComboBox	m_wndSepa;
	CComboBox	m_wndPeriod;
	CComboBox	m_wndNo8;
	CComboBox	m_wndNo7;
	CComboBox	m_wndNo6;
	CComboBox	m_wndNo5;
	CComboBox	m_wndNo4;
	CComboBox	m_wndNo3;
	CComboBox	m_wndNo2;
	CComboBox	m_wndNo1;
	CComboBox	m_wndFrom;
	CComboBox	m_wndDevote;
	CComboBox	m_wndDate;
	BOOL	m_bHaveSpec;
	CString	m_strDesp;
	CString	m_strFileName;
	BOOL	m_bUpOrder;
	CString	m_strDataFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataTransDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataTransDlg)
	afx_msg void OnBtnStart();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnBroser();
	afx_msg void OnSelchangeCbTo();
	afx_msg void OnCkHavespec();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	struct LT_DT_DATAINFO
	{
		char szField[LT_DT_MAXFIELD][LT_DT_FILEDLEN];
	};

	BOOL IsDataValid();
	BOOL GetInputParam();
	BOOL GetDataIndex(long lIndex[LT_DT_USEDFIELD]);
	long GetMaybeDataNum();
	void GetMaybeData(LT_DT_DATAINFO *pDtDataInfo, long lDataNum);
	void TransData(LT_DT_DATAINFO *pDtDataInfo, LT_DATA_INFO *pDataInfo, 
				long lMaybeNum, long lIndex[LT_DT_USEDFIELD]);
	void ChangeSpecNoPos(LT_DATA_INFO *pDataInfo, long lMaybeNum);
	BOOL IsDateValid(LT_DATA_INFO *pDataInfo, long lMaybeNum);

	BOOL CreateTempFile();
	void ReplaceChinaSpace(char *pBuf, long lBufLen);
	void DeleteTempFile();

	void MyTrim(CString &strItem);
	long m_lSelFrom, m_lSelFor;
	char m_szSepa[10];
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATATRANSDLG_H__6C48C3B4_4CC8_4672_8CF9_421F121DBAAD__INCLUDED_)
