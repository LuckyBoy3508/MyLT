// NOFilterCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "NOFilterCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNOFilterCtrl

CNOFilterCtrl::CNOFilterCtrl()
{
	m_lSelFrom = m_lSelFor = m_lHZoneNum = m_lVZoneNum = 0;

	memset(m_lPanelData, 0, sizeof(long) * LT_MAX_SELFROM);	

	m_rtPanel[0].SetRectEmpty();
	m_rtPanel[1].SetRectEmpty();

	m_lColWidth = m_lRowHight = 0;
}

CNOFilterCtrl::~CNOFilterCtrl()
{
	;
}


BEGIN_MESSAGE_MAP(CNOFilterCtrl, CStatic)
	//{{AFX_MSG_MAP(CNOFilterCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNOFilterCtrl message handlers
void CNOFilterCtrl::SetParam(long lSelFrom, long lSelFor, long lHZoneNum, long lVZoneNum)
{
	m_lSelFrom = lSelFrom;
	m_lSelFor = lSelFor;
	m_lHZoneNum = lHZoneNum;
	m_lVZoneNum = lVZoneNum;

	memset(m_lPanelData, 0, sizeof(long) * LT_MAX_SELFROM);	
}

void CNOFilterCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here	
	int nOldMode = dc.SetBkMode(TRANSPARENT);

	ReDrawGrid(dc);

	dc.SetBkMode(nOldMode);
	// Do not call CStatic::OnPaint() for painting messages
}

void CNOFilterCtrl::ReDrawGrid(CPaintDC &dc)
{
	CRect rtClient;
	GetClientRect(rtClient);

	long lWidth = rtClient.Width();
	long lHeight = rtClient.Height();	
	long lMargn = lHeight / 6;

	m_rtPanel[0].SetRect(0, 0, lWidth, lHeight - lMargn);
	m_rtPanel[1].SetRect(0, lHeight - lMargn, lWidth, lHeight);

	long lHZoneNum = m_lHZoneNum + 1;
	long lVZoneNum = m_lVZoneNum + 1;
	long lCX = m_rtPanel[0].Width() / lVZoneNum;
	long lCY = m_rtPanel[0].Height() / lHZoneNum;
	m_lColWidth = lCX;
	m_lRowHight = lCY;

	long lDtx = (m_rtPanel[0].Width() - lCX * lVZoneNum) / 2;
	long lDty = (m_rtPanel[0].Height() - lCY * lHZoneNum) / 2;
	m_rtPanel[0].DeflateRect(lDtx, lDty);
	m_rtPanel[1].DeflateRect(lDtx, lDty);

	CPen pen[3], *plOldPen;
	pen[0].CreatePen(PS_SOLID, 2, g_DataInfo.GetColor(CDrawRes::CLR_RED));
	pen[1].CreatePen(PS_SOLID, 1, g_DataInfo.GetColor(CDrawRes::CLR_RED));
	pen[2].CreatePen(PS_SOLID, 1, g_DataInfo.GetColor(CDrawRes::CLR_BLUE));
	plOldPen = dc.SelectObject(&pen[0]);
	
	CBrush brs[3], *plOldBrush;
	brs[0].CreateSolidBrush(g_DataInfo.GetColor(CDrawRes::CLR_RED));
	brs[1].CreateSolidBrush(g_DataInfo.GetColor(CDrawRes::CLR_BLUE));
	brs[2].CreateSolidBrush(g_DataInfo.GetColor(CDrawRes::CLR_GRAY));
	plOldBrush = dc.SelectObject(&brs[2]);

	CFont *pFont, *pOldFont;
	pFont = g_DataInfo.GetFont(CDrawRes::FONT_CH_SMALL);
	pOldFont = dc.SelectObject(pFont);
	
	//draw frame
	dc.RoundRect(m_rtPanel[0], CPoint(5, 5));
	
	//draw inner frame/title/content.
	CRect rtText;
	CString strText, strTmp;
	dc.SelectObject(&pen[1]);
	long a, b;
	
	for (a = 0; a < lHZoneNum; a++)
	{
		//horizen
		long xs = m_rtPanel[0].left + 1;
		long xe = m_rtPanel[0].right - 1;

		long ys, ye; 
		ys = ye = m_rtPanel[0].top + a * lCY;

		//inner frame
		if (a)
		{
			dc.MoveTo(xs, ys);
			dc.LineTo(xe, ye);
		}

		for (b = 0; b < lVZoneNum; b++)
		{
			rtText.left = m_rtPanel[0].left + b * lCX;
			rtText.top = m_rtPanel[0].top + a * lCY;
			rtText.right = rtText.left + lCX;
			rtText.bottom = rtText.top + lCY;

			strText = "";
			dc.SelectObject(&brs[2]);
			if (a == 0)
			{
				if (b == 0)
				{
					dc.MoveTo(rtText.left + 1, rtText.top + 1);
					dc.LineTo(rtText.right - 1, rtText.bottom - 1);
				}
				else
				{					
					char c = (char)('A' + b - 1);
					strTmp = c;
					strText.Format("V%s", strTmp);
				}
			}
			else
			{
				if (b == 0)
				{
					char c = (char)('A' + a - 1);
					strTmp = c;
					strText.Format("H%s", strTmp);
				}
				else
				{
					long lNO = (a - 1) * m_lVZoneNum + (b - 1);
					if (lNO < m_lSelFrom)
					{
						strText.Format("%02d", lNO + 1);

						if (m_lPanelData[lNO] == 1)
							dc.SelectObject(&brs[0]);
						else if (m_lPanelData[lNO] == 2)
							dc.SelectObject(&brs[1]);
					}
				}
			}

			if (strText.GetLength() > 0)
			{
				dc.Rectangle(rtText);
				dc.DrawText(strText, rtText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}		
	}

	for (b = 0; b < lVZoneNum; b++)
	{
		//vert
		long xs, xe; 
		xs = xe = m_rtPanel[0].left + b * lCX;

		long ys = m_rtPanel[0].top + 1;
		long ye = m_rtPanel[0].bottom - 1;

		if (b)
		{
			dc.MoveTo(xs, ys);
			dc.LineTo(xe, ye);
		}
	}

	//draw flag
	{
		CRect rtFlag[6];
		long lMargn = m_rtPanel[1].Width() / 2;
		rtFlag[0].SetRect(m_rtPanel[1].left, m_rtPanel[1].top, m_rtPanel[1].left + lMargn, m_rtPanel[1].bottom);
		rtFlag[1].SetRect(m_rtPanel[1].left + lMargn, m_rtPanel[1].top, m_rtPanel[1].right, m_rtPanel[1].bottom);
		
		lMargn = rtFlag[0].Width() / 3;
		rtFlag[2] = rtFlag[0];
		rtFlag[2].right = lMargn;
		rtFlag[3] = rtFlag[0];
		rtFlag[3].left += lMargn;
		rtFlag[4] = rtFlag[1];
		rtFlag[4].right = rtFlag[1].left + lMargn;
		rtFlag[5] = rtFlag[1];
		rtFlag[5].left += lMargn;
		
		rtText = rtFlag[2];
		lMargn = rtFlag[2].Width() / 2;
		rtText.left += lMargn - 2;
		rtText.right -= 2;
		lMargn = rtFlag[2].Height() / 4;
		rtText.top += lMargn;
		rtText.bottom -= lMargn;
		dc.SelectObject(&pen[1]);
		dc.SelectObject(&brs[0]);
		dc.Rectangle(rtText);
		rtText = rtFlag[3];
		rtText.left += 2;
		dc.DrawText("必有号码", rtText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		rtText = rtFlag[4];
		lMargn = rtFlag[4].Width() / 2;
		rtText.left += lMargn - 2;
		rtText.right -= 2;
		lMargn = rtFlag[4].Height() / 4;
		rtText.top += lMargn;
		rtText.bottom -= lMargn;
		dc.SelectObject(&pen[2]);
		dc.SelectObject(&brs[1]);
		dc.Rectangle(rtText);
		rtText = rtFlag[5];
		rtText.left += 2;
		dc.DrawText("必无号码", rtText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}

	dc.SelectObject(pOldFont);
	dc.SelectObject(plOldBrush);
	dc.SelectObject(plOldPen);
}

void CNOFilterCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_rtPanel[0].PtInRect(point))
	{
		//1. backup panel data.
		long lPanelDataOld[LT_MAX_SELFROM];
		memcpy(lPanelDataOld, m_lPanelData, sizeof(long) * LT_MAX_SELFROM);

		long a;
		//2. cal new panel data.
		long lRow = (point.y - m_rtPanel[0].top) / m_lRowHight;
		long lCol = (point.x - m_rtPanel[0].left) / m_lColWidth;
		
		long lNO = 0;
		if (lRow == 0)
		{
			if (lCol == 0)
			{
				;
			}
			else
			{//hit v zone
				BOOL bIsAllBlue = TRUE;
				for (a = 0; a < m_lHZoneNum; a++)
				{
					lNO = a * m_lVZoneNum + (lCol - 1);
					if (lNO < m_lSelFrom)
					{
						if (m_lPanelData[lNO] != 2)
						{
							bIsAllBlue = FALSE;
							break;
						}
					}
				}

				for (a = 0; a < m_lHZoneNum; a++)
				{
					lNO = a * m_lVZoneNum + (lCol - 1);
					if (lNO < m_lSelFrom)
					{
						if (bIsAllBlue)
							m_lPanelData[lNO] = 0;
						else
							m_lPanelData[lNO] = 2;
					}
				}
			}
		}
		else
		{
			if (lCol == 0)
			{//hit h zone
				BOOL bIsAllBlue = TRUE;
				for (a = 0; a < m_lVZoneNum; a++)
				{
					lNO = (lRow - 1) * m_lVZoneNum + a;
					if (lNO < m_lSelFrom)
					{
						if (m_lPanelData[lNO] != 2)
						{
							bIsAllBlue = FALSE;
							break;			
						}
					}
				}

				for (a = 0; a < m_lVZoneNum; a++)
				{
					lNO = (lRow - 1) * m_lVZoneNum + a;
					if (lNO < m_lSelFrom)
					{
						if (bIsAllBlue)
							m_lPanelData[lNO] = 0;
						else
							m_lPanelData[lNO] = 2;
					}
				}
			}
			else
			{
				long lNO = (lRow - 1) * m_lVZoneNum + (lCol - 1);

				if (lNO < m_lSelFrom)
				{
					if (m_lPanelData[lNO] == 0)
						m_lPanelData[lNO] = 1;
					else if (m_lPanelData[lNO] == 1)
						m_lPanelData[lNO] = 2;
					else
						m_lPanelData[lNO] = 0;
				}
			}			
		}

		//3. check if data is validate
		BOOL bValidate = TRUE;
		long lHaveNO[LT_MAX_SELFOR];
		long lHaveNum = GetHaveNO(lHaveNO);
		long lNoHaveNO[LT_MAX_SELFROM];
		long lNoHaveNum = GetNoHaveNO(lNoHaveNO);
		if (lHaveNum == -1 || lNoHaveNum == -1)
		{
			bValidate = FALSE;

			CString strErr;
			if (lHaveNum == -1)
				strErr.Format("必有号码不能超过%d个！", m_lSelFor);
			else
				strErr.Format("必无号码不能超过%d个！", m_lSelFrom - m_lSelFor);

			AfxMessageBox(strErr);			
		}

		//4. re-draw data.
		if (bValidate)
			Invalidate();
		else
		{
			memcpy(m_lPanelData, lPanelDataOld, sizeof(long) * LT_MAX_SELFROM);
		}
	}

	CStatic::OnLButtonDown(nFlags, point);
}

long CNOFilterCtrl::GetHaveNO(long lNo[LT_MAX_SELFOR])
{
	long lRet = 0;

	memset(lNo, 0, sizeof(long) * LT_MAX_SELFOR);

	long lCnt = 0;
	for (long a = 0; a < m_lSelFrom; a++)
	{
		if (m_lPanelData[a] == 1)
		{//have
			lNo[lCnt++] = a + 1;

			if (lCnt >= m_lSelFor)
			{
				lRet = -1;
				break;
			}
		}
	}

	if (lRet != -1)
		lRet = lCnt;

	return lRet;
}

long CNOFilterCtrl::GetNoHaveNO(long lNo[LT_MAX_SELFROM])
{
	long lRet = 0;

	memset(lNo, 0, sizeof(long) * LT_MAX_SELFROM);

	long lCnt = 0;
	for (long a = 0; a < m_lSelFrom; a++)
	{
		if (m_lPanelData[a] == 2)
		{//no have
			lNo[lCnt++] = a + 1;

			if (lCnt >= m_lSelFrom - m_lSelFor)
			{
				lRet = -1;
				break;
			}
		}
	}

	if (lRet != -1)
		lRet = lCnt;

	return lRet;
}
