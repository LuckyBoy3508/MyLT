// MyLTDoc.h : interface of the CMyLTDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLTDOC_H__16AAD0F3_4E43_4B44_88FA_90EBE81B14D2__INCLUDED_)
#define AFX_MYLTDOC_H__16AAD0F3_4E43_4B44_88FA_90EBE81B14D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyLTView;
class CMyLTDoc : public CDocument
{
protected: // create from serialization only
	CMyLTDoc();
	DECLARE_DYNCREATE(CMyLTDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyLTDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyLTDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyLTDoc)
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnDataSelect();
	afx_msg void OnDataTransdata();
	afx_msg void OnNetDownload();	
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDataSelect(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//history data function.	
	FILELIST *GetFileList();
	BOOL IsHaveFile(LPCTSTR lpszFileName);
	LT_FILE_INFO *GetFile(LPCTSTR lpszFileName);
	void DeleteFile(LPCTSTR lpszFileName);	
	
	//current data
	void OnGetData(LPCTSTR lpszFileName);
	LT_DATA_INFO *GetDataInfo(long &lDataNum);
	static LPCTSTR IsDataValid(LT_DATA_INFO *pData, long lSelFrom, long lSelFor);

	//others
	void SetWindowTitle();

	//property.
	CString m_strFileName;
	LT_DATA_INFO *m_pDataInfo;
	long m_lDataNum;
	long m_lSelFrom, m_lSelFor;
	BOOL m_bHaveSpec;
	BOOL m_bIsDataChanged;

private:
	//file	
	void InitFileList();
	void FreeFileList();
	FILELIST m_lstFile;

	//current data
	void SaveDataInfo();
	BOOL RWData(BOOL bIsRead = TRUE);
	long m_lVersion;
	CString m_strDesp;
	long m_lProvince;
	
	//option
	long m_lFromPeriod, m_lToPeriod;
	BOOL m_bIsApply;

	//others
	CMyLTView *GetLtView();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLTDOC_H__16AAD0F3_4E43_4B44_88FA_90EBE81B14D2__INCLUDED_)
