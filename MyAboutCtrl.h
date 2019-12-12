
#ifndef _MYABOUTCTRL_H
#define _MYABOUTCTRL_H

/* Usage:
	1: Define variable in dialog class.
		CMyAboutCtrl myAboutCtrl;
	2: Call differenc function.
		myAboutCtrl.SetStarSpeed(30);
		myAboutCtrl.SetScrollSpeed(2);
		myAboutCtrl.AddTextLine("...");
*/

#define NUM_STARS 100

class CMyAboutCtrl : public CStatic
{
public:
	CMyAboutCtrl();
	virtual ~CMyAboutCtrl();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyAboutCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddTextLine(LPCTSTR lpszText);
	void SetStarSpeed(int nSpeed);
	void SetScrollSpeed(int nSpeed);
	
protected:
	//{{AFX_MSG(CMyAboutCtrl)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int m_nStarsSpeed;
	int m_nScrollSpeed;
	int m_nScrollPos;
	void DoScrollText(CDC *pDC);
	void DoStars(CDC *pDC);
	void InvalidateCtrl();

	CFont m_Font;
	CRect  m_rectClient;
	CDC     m_MainDC;
	CBitmap m_MainBitmap;
	CBitmap *m_pOldBitmap;
	CStringArray m_TextLines;

	struct CStar
	{
		int x;
		int y;
		int z;
	};
	CStar m_StarArray[NUM_STARS];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARWARSCTRL_H__B75B0B9E_367D_11D6_AB38_00D0B70C3D79__INCLUDED_)
