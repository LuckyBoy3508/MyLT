#if !defined(AFX_MYDWVIEW_H__9F43DE4B_F0A2_4831_9F4D_4E8DA41F7A29__INCLUDED_)
#define AFX_MYDWVIEW_H__9F43DE4B_F0A2_4831_9F4D_4E8DA41F7A29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDWView.h : header file
//

#include "DrawCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CMyAnalyseView view
#define LT_BTN_HEIGHT	20
#define LT_MAINBTN_WD	70
#define LT_SUBBTN_WD	50

class CMyLTDoc;
class CMyAnalyseView : public CView
{
protected:
	CMyAnalyseView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyAnalyseView)

// Attributes
public:
	BOOL RWCfg(BOOL bIsRead);
	CMyLTDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyAnalyseView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyAnalyseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyAnalyseView)
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnBtns(UINT nID);
	afx_msg void OnMenus(UINT nID);
	afx_msg LRESULT OnScroll(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void GetAllPanelRect();
	void CreateCtrls();
	void RedrawAll(CDC *pDC);
	void SetCtrlsState();
	void ScrollView(long lType, long lNewPos);

private:
	CDrawProxy m_DrawProxy;
	CDvMemDC m_MemDC;

	//0:frame 1:work 2:v-scrollbar 3:bottom-ctrls-rect
	//4:analyse-btn 5:stat-btn 6:filter-btn 7:command-btn
	//8:<- 9:-> 10:up 11:Down 12:Pageup 13: Pagedown
	CRect m_rtCtrls[14];

	//0:analyse 1:stat 2:filter 3:command
	//4:<- 5:-> 6:up 7:down 8:pageup 9:pagedown
	CButton m_wndBtns[10];	
	//CSpinButtonCtrl m_wndVScroll;
	CSpinButtonCtrl m_wndVScroll;

	//Range occupy by button, that not need redraw.
	CRgn m_rgnButton;

	//current ctrl index.
	long m_lCurrCtrl;

	//curr ctrl status property	
	long m_lCtrlStartPos;
	long m_lCtrlCurrPos;
	long m_lCtrlOnePageNum;
	long m_lCtrlScrollRange;
	long m_lCtrlTotalNum;

	//curr obj status property
	long m_lObjStartPos;
	long m_lObjCurrPos;
	long m_lObjOnePageNum;
	long m_lObjScrollRange;
	long m_lObjTotalNum;

	//data info
	LT_DATA_INFO *m_pDataInfo;
	long m_lDataNum;
	long m_lSelFrom, m_lSelFor;
	BOOL m_bHaveSpec;
};

inline CMyLTDoc* CMyAnalyseView::GetDocument()
   { return (CMyLTDoc*)m_pDocument; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDWVIEW_H__9F43DE4B_F0A2_4831_9F4D_4E8DA41F7A29__INCLUDED_)
