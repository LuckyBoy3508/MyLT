// SameRgnTmplEdit.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "SameRgnTmplEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSameRgnTmplEdit

CSameRgnTmplEdit::CSameRgnTmplEdit()
{	
	m_pSameRgnInfo = NULL;
}

CSameRgnTmplEdit::~CSameRgnTmplEdit()
{
}


BEGIN_MESSAGE_MAP(CSameRgnTmplEdit, CStatic)
	//{{AFX_MSG_MAP(CSameRgnTmplEdit)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSameRgnTmplEdit message handlers

void CSameRgnTmplEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here	
	ReDrawGrid(dc);
	FillMap(dc);

	// Do not call CStatic::OnPaint() for painting messages
}

void CSameRgnTmplEdit::SetMap(LTDC_SAMERGN_INFO *pSameRgnInfo)
{
	m_pSameRgnInfo = pSameRgnInfo;

	Invalidate(FALSE);
}

void CSameRgnTmplEdit::ReDrawGrid(CPaintDC &dc)
{
	CRect rtClient;
	GetClientRect(rtClient);

	long lWidth = rtClient.Width();
	long lHeight = rtClient.Height();

	long lCX = lWidth / (LTDC_DWRGNGRID_ROW + 1);
	long lCY = lHeight / (LTDC_DWRGNGRID_COL + 1);

	long lX = (lWidth - lCX * (LTDC_DWRGNGRID_ROW + 1)) / 2;
	long lY = (lHeight - lCY * (LTDC_DWRGNGRID_COL + 1)) /2;

	CPen pen[2];
	pen[0].CreatePen(PS_SOLID, 2, g_DataInfo.GetColor(CDrawRes::CLR_RED));
	pen[1].CreatePen(PS_SOLID, 1, g_DataInfo.GetColor(CDrawRes::CLR_RED));

	CRect rtWork;
	rtWork.left = lX;
	rtWork.right = lWidth - lX;
	rtWork.top = lY;
	rtWork.bottom = lHeight - lY;

	dc.SelectObject(pen[0]);
	//dc.Rectangle(rtWork);
	dc.RoundRect(rtWork, CPoint(5, 5));

	dc.SelectObject(pen[1]);

	long a, b;
	for (a = 1; a <= LTDC_DWRGNGRID_ROW; a++)
	{
		//horizen
		long xs = lX + 1;
		long xe = lWidth - lX - 1;

		long ys, ye; 
		ys = ye = lY + a * lCY;

		dc.MoveTo(xs, ys);
		dc.LineTo(xe, ye);
	}

	for (b = 1; b <= LTDC_DWRGNGRID_COL; b++)
	{
		//vert
		long xs, xe; 
		xs = xe = lX + b * lCX;

		long ys = lY + 1;
		long ye = lHeight - lY - 1;

		dc.MoveTo(xs, ys);
		dc.LineTo(xe, ye);
	}

	for (a = 0; a < LTDC_DWRGNGRID_ROW + 1; a++)
	{
		for (b = 0; b < LTDC_DWRGNGRID_COL + 1; b++)
		{
			long left = lX + b * lCX;
			long right = left + lCX;
			long top = lY + a * lCY;
			long bottom = top + lCY;

			m_rtMap[a][b].SetRect(left, top, right, bottom);
		}
	}

	int nOldMode = dc.SetBkMode(TRANSPARENT);	
	for (a = 1; a < LTDC_DWRGNGRID_ROW + 1; a++)
	{
		char c = (char)('A' + a - 1);
		CString strT = c;

		dc.DrawText(strT, 1, m_rtMap[0][a], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	for (b = 1; b < LTDC_DWRGNGRID_COL + 1; b++)
	{
		char c = (char)('0' + b - 1);
		CString strT = c;
		dc.DrawText(strT, 1, m_rtMap[b][0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	dc.SetBkMode(nOldMode);
}

void CSameRgnTmplEdit::FillMap(CPaintDC &dc)
{
	if (m_pSameRgnInfo != NULL)
	{
		for (long a = 0; a < LTDC_DWRGNGRID_ROW; a++)
		{
			for (long b = 0; b < LTDC_DWRGNGRID_COL; b++)
			{
				if (m_pSameRgnInfo->lRgnMap[a][b] > 0)
					dc.FillSolidRect(m_rtMap[a + 1][b + 1], g_DataInfo.GetColor(CDrawRes::CLR_RED));
			}
		}
	}
}

void CSameRgnTmplEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_pSameRgnInfo)
		return;

	BOOL bIn = FALSE;
	for (long a = 0; a < LTDC_DWRGNGRID_ROW; a++)
	{
		for (long b = 0; b < LTDC_DWRGNGRID_COL; b++)
		{
			if (m_rtMap[a + 1][b + 1].PtInRect(point))
			{
				m_pSameRgnInfo->lRgnMap[a][b] ^= 1;

				bIn = TRUE;
				break;
			}
		}
	}

	if (bIn)
		Invalidate(FALSE);

	CStatic::OnLButtonDown(nFlags, point);
}
