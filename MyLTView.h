// MyLTView.h : interface of the CMyLTView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLTVIEW_H__0DBC4D7E_0590_4F6E_8A64_462077FDB2C2__INCLUDED_)
#define AFX_MYLTVIEW_H__0DBC4D7E_0590_4F6E_8A64_462077FDB2C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataManager.h"

class CMyLTView : public CListView
{
protected: // create from serialization only
	CMyLTView();
	DECLARE_DYNCREATE(CMyLTView)

// Attributes
public:
	CMyLTDoc* GetDocument();
	
	enum LT_CHAGE_TYPE
	{
		LCT_ADD = 1, LCT_INSERT, LCT_MODIFY, LCT_DELETE,
	};
	void ChangeData(LT_CHAGE_TYPE eType, LT_DATA_INFO *pNewData);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyLTView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyLTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyLTView)
	afx_msg void OnDataManager();
	afx_msg void OnUpdateDataManager(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	LT_DATA_INFO *GetNewDataInfo(long &lNewDataNum);
	BOOL m_bCurrShow;

private:
	void UpdateCtlHeader();
	void MyInsertItem(long lIndex, LT_DATA_INFO *pDataInfo);
	void UpdateSubItem(long lIndex, LT_DATA_INFO *pDataInfo);
	void ChangePeriod();
	void UpdateItemTitle();

	LT_DATA_INFO *m_pDataInfo;
	long m_lDataNum;
	long m_lSelFrom, m_lSelFor;
	BOOL m_bHaveSpec;

	CDataManager m_DataManager;
	long m_lCurrSel;
};

#ifndef _DEBUG  // debug version in MyLTView.cpp
inline CMyLTDoc* CMyLTView::GetDocument()
   { return (CMyLTDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLTVIEW_H__0DBC4D7E_0590_4F6E_8A64_462077FDB2C2__INCLUDED_)
