#if !defined(AFX_SAMERGNTMPLEDIT_H__A82D73A4_E9B5_43E5_BEA4_77D0378866DD__INCLUDED_)
#define AFX_SAMERGNTMPLEDIT_H__A82D73A4_E9B5_43E5_BEA4_77D0378866DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SameRgnTmplEdit.h : header file
//

#include "DrawCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CSameRgnTmplEdit window

class CSameRgnTmplEdit : public CStatic
{
// Construction
public:
	CSameRgnTmplEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSameRgnTmplEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSameRgnTmplEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSameRgnTmplEdit)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void SetMap(LTDC_SAMERGN_INFO *pSameRgnInfo);

	void ReDrawGrid(CPaintDC &dc);
	void FillMap(CPaintDC &dc);

	LTDC_SAMERGN_INFO *m_pSameRgnInfo;
	CRect m_rtMap[LTDC_DWRGNGRID_ROW + 1][LTDC_DWRGNGRID_COL + 1];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMERGNTMPLEDIT_H__A82D73A4_E9B5_43E5_BEA4_77D0378866DD__INCLUDED_)
