#if !defined(AFX_NOFILTERCTRL_H__552D5DD8_1481_4763_B496_950215B61736__INCLUDED_)
#define AFX_NOFILTERCTRL_H__552D5DD8_1481_4763_B496_950215B61736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NOFilterCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNOFilterCtrl window
class CNOFilterCtrl : public CStatic
{
// Construction
public:
	CNOFilterCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNOFilterCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNOFilterCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNOFilterCtrl)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void SetParam(long lSelFrom, long lSelFor, long lHZoneNum, long lVZoneNum);
	long GetHaveNO(long lNo[LT_MAX_SELFOR]);
	long GetNoHaveNO(long lNo[LT_MAX_SELFROM]);

private:
	long m_lSelFrom, m_lSelFor, m_lHZoneNum, m_lVZoneNum;

	void ReDrawGrid(CPaintDC &dc);
	long m_lPanelData[LT_MAX_SELFROM];

	CRect m_rtPanel[2];
	long m_lColWidth, m_lRowHight;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOFILTERCTRL_H__552D5DD8_1481_4763_B496_950215B61736__INCLUDED_)
