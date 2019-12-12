// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D5B2C7D9_A46F_41FD_AF3D_BFE460E6EFBB__INCLUDED_)
#define AFX_MAINFRM_H__D5B2C7D9_A46F_41FD_AF3D_BFE460E6EFBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyLTView;
class CMyAnalyseView;
class CMyStatView;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	afx_msg void OnViewChange(UINT uCmdID);
	afx_msg void OnUpdateViewAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStatusPanel(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	void AddAllView();
	CMyStatView *GetStatView() {return m_pStatView;}

private:
	struct TOOLBAR_INFO
	{
		UINT uID;
		long lBitmapIndex;
		UINT uStyle;		
		char *pName;
	};

	BOOL MyCreateToolBar();
	BOOL MyCreateStatusBar();

	CMyLTView *m_pLTView;
	CMyAnalyseView *m_pAnalyseView;
	CMyStatView *m_pStatView;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D5B2C7D9_A46F_41FD_AF3D_BFE460E6EFBB__INCLUDED_)
