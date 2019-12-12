
#include "stdafx.h"
#include "DvChart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDvChart
CDvChart::CDvChart()
{
	memset(&m_BaseInfo, 0, sizeof(DVCT_BASE_INFO));	
	memset(&m_DataInfo, 0, sizeof(DVCT_DATA_INFO));	
	m_lCurrLine = 0;
	m_rtShow.SetRectEmpty();

	m_lStartPos = 0;
	m_lCurrPos = 0;
	m_lOnePageNum = 0;
	m_lScrollRange = 0;
	m_lTotalNum = 0;
	m_lCellWidth = DVCT_DEFCELL_WIDTH;
	m_bIsColumn = FALSE;

	m_lShowType = 0;
	for (long a = 0; a < 8; a++)
		m_rtPanel[a].SetRectEmpty();

	memset(&m_VMarkInfo, 0, sizeof(DVCT_VMARK_INFO));

	m_bArrowFrm = m_bArrowLeft = m_bArrowRight = FALSE;
	m_bColumnFrm = FALSE;
}

CDvChart::~CDvChart()
{
	;
}

void CDvChart::SetBaseInfo(DVCT_BASE_INFO *pBaseInfo)
{
	memcpy(&m_BaseInfo, pBaseInfo, sizeof(DVCT_BASE_INFO));
}

void CDvChart::SetData(DVCT_DATA_INFO *pData)
{
	memcpy(&m_DataInfo, pData, sizeof(DVCT_DATA_INFO));
	m_lCurrLine = m_DataInfo.lCurrDataIndex;

	ResetDrawInfo();
}

void CDvChart::SetParam(RECT *pRect)
{
	if (pRect)
		m_rtShow.SetRect(pRect->left, pRect->top, pRect->right, pRect->bottom);
	else
		m_rtShow.SetRectEmpty();

	//get all panels rect.
	CalcPanelsRect();

	//reset draw info
	ResetDrawInfo();
}

void CDvChart::CalcPanelsRect()
{
	if (!m_rtShow.IsRectEmpty())
	{
		//wk
		m_rtPanel[0].SetRect(m_rtShow.left, m_rtShow.top + 20, m_rtShow.right, m_rtShow.bottom);
		m_rtPanel[1].SetRect(m_rtPanel[0].left + 50, m_rtPanel[0].top + 10,
							m_rtPanel[0].right - 30, m_rtPanel[0].bottom - 20);

		//title
		m_rtPanel[2].SetRect(m_rtShow.left, m_rtShow.top, m_rtShow.right, m_rtShow.top + 20);
		m_rtPanel[3].SetRect(m_rtPanel[2].left + 40, m_rtPanel[2].top,
							m_rtPanel[2].right - 20, m_rtPanel[2].bottom);

		//title-others
		long lCX = m_rtPanel[3].Width() / 10;
		long lTop = m_rtPanel[3].top;
		long lBottom = m_rtPanel[3].bottom;
		m_rtPanel[4].SetRect(m_rtPanel[3].left, lTop, m_rtPanel[3].left + lCX * 2, lBottom);
		m_rtPanel[5].SetRect(m_rtPanel[4].right, lTop, m_rtPanel[4].right + lCX * 6, lBottom);
		m_rtPanel[6].SetRect(m_rtPanel[5].right, lTop, m_rtPanel[5].right + lCX * 1, lBottom);
		m_rtPanel[7].SetRect(m_rtPanel[6].right, lTop, m_rtPanel[6].right + lCX * 1, lBottom);
	}
	else
	{
		for (long a = 0; a < 8; a++)
			m_rtPanel[a].SetRectEmpty();
	}
}

void CDvChart::ResetDrawInfo()
{	
	if (IsHaveData() && !m_rtShow.IsRectEmpty())
	{
		long lDrawDataNum = m_DataInfo.pLineData->lUnitNum;
		if (m_DataInfo.bIsFixWidth)
		{
			m_lStartPos = 0;
			m_lCurrPos = lDrawDataNum - 1;
			m_lOnePageNum = lDrawDataNum;
			m_lScrollRange = 0;
			m_lTotalNum = lDrawDataNum;
			m_lCellWidth = m_rtPanel[1].Width() / m_lOnePageNum;		
		}
		else
		{
			m_lOnePageNum = m_rtPanel[1].Width() / m_lCellWidth;
			m_lTotalNum = lDrawDataNum;
			m_lScrollRange = lDrawDataNum - m_lOnePageNum;
			if (m_lScrollRange < 0)
				m_lScrollRange = 0;
			m_lStartPos = m_lScrollRange;
			m_lCurrPos = lDrawDataNum - 1;
		}
	}
	else
	{
		m_lStartPos = 0;
		m_lCurrPos = 0;
		m_lOnePageNum = 0;
		m_lScrollRange = 0;
		m_lTotalNum = 0;
		m_lCellWidth = DVCT_DEFCELL_WIDTH;	
	}
}

BOOL CDvChart::IsHaveData()
{
	if (m_DataInfo.pLineData && m_DataInfo.lLineNum > 0 && 
		m_DataInfo.pLineData->pUnitData && m_DataInfo.pLineData->lUnitNum > 0)
		return TRUE;

	return FALSE;
}

void CDvChart::SetPosInfo(long lStartPos, long lCurrPos)
{
	m_lStartPos = lStartPos;
	m_lCurrPos = lCurrPos;
}

void CDvChart::SetIsColumn(BOOL bIsColumn)
{
	m_bIsColumn = bIsColumn;
}

long CDvChart::SetCellWidth(long lNewWidth)
{
	if (lNewWidth < DVCT_MINCELL_WIDTH)
		lNewWidth = DVCT_MINCELL_WIDTH;

	if (lNewWidth > DVCT_MAXCELL_WIDTH)
		lNewWidth = DVCT_MAXCELL_WIDTH;

	if (m_DataInfo.bIsFixWidth)
	{
		m_lCellWidth = m_rtPanel[1].Width() / m_lTotalNum;
		m_lOnePageNum = m_lTotalNum;
	}
	else
	{
		m_lOnePageNum = m_rtPanel[1].Width() / lNewWidth;
		m_lCellWidth = lNewWidth;
	}
	
	return m_lOnePageNum;
}

void CDvChart::GetDrawInfo(long &lStartPos, long &lCurrPos, 
				long &lOnePageNum, long &lScrollRange, long &lTotalNum, 
				long &lCellWidth, BOOL &bIsColumn)
{
	lStartPos = m_lStartPos;
	lCurrPos = m_lCurrPos;
	lOnePageNum = m_lOnePageNum;
	lScrollRange = m_lScrollRange;
	lTotalNum = m_lTotalNum;
	lCellWidth = m_lCellWidth;
	bIsColumn = m_bIsColumn;
}

BOOL CDvChart::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bRet = FALSE;

	if (m_rtPanel[1].PtInRect(point))
	{
		long lOldPos = m_lCurrPos;

		m_lCurrPos = (point.x - m_rtPanel[1].left - m_lCellWidth / 2) / m_lCellWidth;
		if (m_lCurrPos < 0)
			m_lCurrPos = 0;

		m_lCurrPos += m_lStartPos;

		if (m_lCurrPos > m_lTotalNum - 1)
			m_lCurrPos = m_lTotalNum - 1;
		
		if (m_lCurrPos != lOldPos)
			bRet = TRUE;
	}
	else if (m_bArrowLeft || m_bArrowRight)
	{
		long lOld = m_lCurrLine;
		m_lCurrLine = m_bArrowLeft ? (m_lCurrLine - 1) : (m_lCurrLine + 1);
		
		if (m_lCurrLine < 0)
			m_lCurrLine = 0;
		else if (m_lCurrLine > m_DataInfo.lLineNum - 1)
			m_lCurrLine = m_DataInfo.lLineNum - 1;

		if (m_lCurrLine != lOld)
			bRet = TRUE;
	}
	else if (m_bColumnFrm)
	{
		m_bIsColumn ^= 1;
		bRet = TRUE;
	}

	return bRet;
}

BOOL CDvChart::OnMouseMove(UINT nFlags, POINT point)
{
	BOOL bRet = FALSE;

	if (m_lShowType == 1)
	{//active.
		if (DrawScrollBar(TRUE, &point) || DrawColumnFlag(TRUE, &point))
			bRet = TRUE;
	}
		
	return bRet;
}

void CDvChart::Draw(HDC hDC, long lShowType)
{
	m_lShowType = lShowType;

	m_MemDC.Attach(hDC);
	int nOldMode = m_MemDC.SetBkMode(TRANSPARENT);

	if (DrawOutlook()) //draw outlook(frame/title/scroll/colum)
	{
		//draw line(ruler/line)			
		DrawLine();

		//draw cursor	
		DrawCursor();
	}

	m_MemDC.SetBkMode(nOldMode);
	m_MemDC.Detach();
}

BOOL CDvChart::DrawOutlook()
{
	COLORREF clrUse = (m_lShowType == 1) ? m_BaseInfo.clrFrame : m_BaseInfo.clrGray;	

	CPen pen[3], *pOldPen;
	pen[0].CreatePen(PS_SOLID, 2, clrUse);
	pen[1].CreatePen(PS_SOLID, 1, clrUse);
	pen[2].CreatePen(PS_SOLID, 2, m_BaseInfo.clrFrame);
	pOldPen = m_MemDC.SelectObject(&pen[0]);
	
	CBrush brsBK(m_BaseInfo.clrBkGround);
	CBrush *pOldBrh = m_MemDC.SelectObject(&brsBK);
	
	//1. draw frame
	if (m_lShowType == 2)
		m_MemDC.SelectObject(&pen[2]);
	m_MemDC.RoundRect(m_rtShow, CPoint(m_BaseInfo.lCornerRadius, m_BaseInfo.lCornerRadius));

	//2. draw content
	if (IsHaveData())
	{
		//draw name.
		ShowText(m_BaseInfo.szName, m_rtPanel[4], clrUse, 9, 0);
		
		//draw val's bottom line
		m_MemDC.SelectObject(&pen[1]);
		m_MemDC.MoveTo(m_rtPanel[5].left + 5, m_rtPanel[5].bottom - 1);
		m_MemDC.LineTo(m_rtPanel[5].right - 5, m_rtPanel[5].bottom - 1);
		
		//draw scroll
		DrawScrollBar();
		
		//draw column flag.
		DrawColumnFlag();
	}
	else
	{
		ShowText("没有数据！", m_rtPanel[0], clrUse, 5, 0);
		return FALSE;
	}
		
	m_MemDC.SelectObject(pOldBrh);
	m_MemDC.SelectObject(pOldPen);

	return TRUE;
}

void CDvChart::ShowText(LPCTSTR lpszText, RECT *pRect, COLORREF clrText, long lPos, long lFontIndex)
{
	if (lpszText && pRect)
	{
		COLORREF clrOld = m_MemDC.SetTextColor(clrText);
				
		CFont ft, *pOldFont;
		ft.Attach(m_BaseInfo.hFont[lFontIndex]);
		pOldFont = m_MemDC.SelectObject(&ft);

		UINT uPos = DT_SINGLELINE;
		if (lPos % 3 == 1)
			uPos |= DT_LEFT;
		else if (lPos % 3 == 2)
			uPos |= DT_CENTER;
		else
			uPos |= DT_RIGHT;

		if ((lPos - 1) / 3 == 0)
			uPos |= DT_TOP;
		else if ((lPos - 1) / 3 == 1)
			uPos |= DT_VCENTER;
		else
			uPos |= DT_BOTTOM;

		CString strText(lpszText);
		m_MemDC.DrawText(strText, pRect, uPos);

		m_MemDC.SetTextColor(clrOld);
		m_MemDC.SelectObject(pOldFont);
		ft.Detach();
	}
}

BOOL CDvChart::DrawScrollBar(BOOL bTest /*= FALSE*/, POINT *pPoint /*= NULL*/)
{
	BOOL bRet = FALSE;

	CRect rtBtn[3];
	//1. get arrow button rects
	{
		long lMargn, lLeft, lTop, lRight, lBottom, lWidth;

		lMargn = m_rtPanel[6].Width() / 5;
		lLeft = m_rtPanel[6].left + lMargn;
		lTop = m_rtPanel[6].top + 3;
		lRight = m_rtPanel[6].right - lMargn;
		lBottom = m_rtPanel[6].bottom;
		lWidth = m_rtPanel[6].Width() - 2 * lMargn;

		rtBtn[0].SetRect(lLeft, lTop, lRight, lBottom);
		rtBtn[1].SetRect(lLeft, lTop, lLeft + lWidth / 2 - 1, lBottom);
		rtBtn[2].SetRect(lLeft + lWidth / 2 + 1, lTop, lRight, lBottom);
	}

	//2. do test or draw function.
	if (!bTest)
	{
		COLORREF clrUse[3];

		clrUse[0] = clrUse[1] = clrUse[2] = m_BaseInfo.clrGray;
		if (m_lShowType == 1)
		{
			if (m_bArrowFrm)
				clrUse[0] = m_BaseInfo.clrFrame;

			if (m_bArrowLeft)
				clrUse[1] = m_BaseInfo.clrFrame;

			if (m_bArrowRight)
				clrUse[2] = m_BaseInfo.clrFrame;
		}
			
		CPen pen[1], *pOldPen;
		pen[0].CreatePen(PS_SOLID, 1, clrUse[0]);
		pOldPen = m_MemDC.SelectObject(&pen[0]);
		
		CBrush brsBK(m_BaseInfo.clrBkGround);
		CBrush *pOldBrh = m_MemDC.SelectObject(&brsBK);

		//draw frame
		m_MemDC.RoundRect(rtBtn[0], CPoint(15, 15));
		m_MemDC.MoveTo(rtBtn[1].right, rtBtn[1].top);
		m_MemDC.LineTo(rtBtn[1].right, rtBtn[1].bottom);
		m_MemDC.MoveTo(rtBtn[2].left, rtBtn[2].top);
		m_MemDC.LineTo(rtBtn[2].left, rtBtn[2].bottom);

		//show arrow	
		ShowText("<", rtBtn[1], clrUse[1], 5, 1);
		ShowText(">", rtBtn[2], clrUse[2], 5, 1);

		m_MemDC.SelectObject(pOldBrh);
		m_MemDC.SelectObject(pOldPen);
	}
	else
	{
		if (m_lShowType == 1)
		{
			BOOL bOld[3] = { m_bArrowFrm, m_bArrowLeft, m_bArrowRight };

			m_bArrowFrm = m_bArrowLeft = m_bArrowRight = FALSE;

			if (rtBtn[0].PtInRect(*pPoint))
				m_bArrowFrm = TRUE;

			if (rtBtn[1].PtInRect(*pPoint) && m_lCurrLine > 0)
				m_bArrowLeft = TRUE;

			if (rtBtn[2].PtInRect(*pPoint) && m_lCurrLine != m_DataInfo.lLineNum - 1)
				m_bArrowRight = TRUE;

			if (bOld[0] != m_bArrowFrm || bOld[1] != m_bArrowLeft || bOld[2] != m_bArrowRight)
				bRet = TRUE;
		}
	}

	return bRet;
}

BOOL CDvChart::DrawColumnFlag(BOOL bTest /*= FALSE*/, POINT *pPoint /*= NULL*/)
{
	BOOL bRet = FALSE;

	CRect rtBtn = m_rtPanel[7];
	//1. get column button rects
	{
		long lMargn = m_rtPanel[7].Width() / 5; 
		rtBtn.left += lMargn;
		rtBtn.top += 3; 
		rtBtn.right -= lMargn;
	}

	//2. do test or draw function.
	if (!bTest)
	{
		COLORREF clrUse = m_BaseInfo.clrGray;
		if (m_lShowType == 1 && m_bColumnFrm)
			clrUse = m_BaseInfo.clrFrame;

		CPen pen[1], *pOldPen;
		pen[0].CreatePen(PS_SOLID, 1, clrUse);
		pOldPen = m_MemDC.SelectObject(&pen[0]);
		
		CBrush brsBK(m_BaseInfo.clrBkGround);
		CBrush *pOldBrh = m_MemDC.SelectObject(&brsBK);

		m_MemDC.RoundRect(rtBtn, CPoint(5, 5));

		CString strText("柱状图");
		if (m_bIsColumn)
			strText = "线形图";

		ShowText(strText, rtBtn, clrUse, 5, 1);

		m_MemDC.SelectObject(pOldBrh);
		m_MemDC.SelectObject(pOldPen);
	}
	else
	{
		if (m_lShowType == 1)
		{
			BOOL bOld = m_bColumnFrm;

			m_bColumnFrm = FALSE;
			if (rtBtn.PtInRect(*pPoint))
				m_bColumnFrm = TRUE;

			if (bOld != m_bColumnFrm)
				bRet = TRUE;
		}
	}

	return bRet;
}

void CDvChart::DrawLine()
{
	DrawRuler();
	DrawKLine();
}

void CDvChart::DrawRuler()
{
	//1. get vmark info
	GetVMarkInfo();
	
	//2. draw ruler detail
	COLORREF clrUse = (m_lShowType == 1) ? m_BaseInfo.clrRuler : m_BaseInfo.clrGray;	
	CRect rtText;
	CString strText;
	long a;

	CPen pen[2], *pOldPen;
	pen[0].CreatePen(PS_SOLID, 1, clrUse);
	pen[1].CreatePen(PS_DOT, 1, clrUse);
	pOldPen = m_MemDC.SelectObject(&pen[0]);
	
	//a. draw ruler line
	{
		//draw vertical mark and horizontal line
		POINT ptRuler[3] = 
		{
			m_rtPanel[1].left, m_rtPanel[1].top - 10,
			m_rtPanel[1].left, m_rtPanel[1].bottom,		
			m_rtPanel[1].right + 10, m_rtPanel[1].bottom,
		};

		m_MemDC.MoveTo(ptRuler[0]);
		m_MemDC.LineTo(ptRuler[1]);
		m_MemDC.LineTo(ptRuler[2]);

		//draw edge arrow
		POINT ptArrow[4] =
		{
			ptRuler[0].x - 3, ptRuler[0].y + 5,
			ptRuler[0].x + 3, ptRuler[0].y + 5,

			ptRuler[2].x - 5, ptRuler[2].y - 3,
			ptRuler[2].x - 5, ptRuler[2].y + 3,
		};
		
		m_MemDC.MoveTo(ptArrow[0]);
		m_MemDC.LineTo(ptRuler[0]);
		m_MemDC.MoveTo(ptArrow[1]);
		m_MemDC.LineTo(ptRuler[0]);

		m_MemDC.MoveTo(ptArrow[2]);
		m_MemDC.LineTo(ptRuler[2]);
		m_MemDC.MoveTo(ptArrow[3]);
		m_MemDC.LineTo(ptRuler[2]);
	}
	
	//b. draw vertical mark.
	{		
		rtText.left = m_rtPanel[0].left;
		rtText.right = m_rtPanel[1].left - 5;

		//draw val mark
		long lY = m_rtPanel[1].bottom;
		for (a = 0; a <= m_VMarkInfo.lMarkNum; a++, lY -= m_VMarkInfo.lMarkStep)
		{
			m_MemDC.SelectObject(&pen[0]);

			//draw mark.
			m_MemDC.MoveTo(m_rtPanel[1].left - 3, lY);
			m_MemDC.LineTo(m_rtPanel[1].left, lY);

			//draw text.			
			if (m_DataInfo.bIsInt)
			{
				long lVal = m_VMarkInfo.lValMin + a * m_VMarkInfo.lValStep;
				strText.Format("%d", lVal);
			}
			else
			{
				float fVal = m_VMarkInfo.fValMin + a * m_VMarkInfo.fValStep;
				if (m_DataInfo.bShowPercent)
				{
					fVal *= 100;
					strText.Format("%.2f%%", fVal);
				}
				else
					strText.Format("%.2f", fVal);
			}
						
			rtText.top = lY - m_VMarkInfo.lMarkStep / 2;			
			rtText.bottom = lY + m_VMarkInfo.lMarkStep / 2;

			ShowText(strText, rtText, clrUse, 6, 2);

			//draw dot line
			if (a)
			{
				m_MemDC.SelectObject(&pen[1]);
				m_MemDC.MoveTo(m_rtPanel[1].left + 2, lY);
				m_MemDC.LineTo(m_rtPanel[1].right - 2, lY);
			}
		}

		//draw vertical unit
		rtText.left = m_rtPanel[0].left;
		rtText.top = m_rtPanel[1].top - m_VMarkInfo.lMarkStep;
		rtText.right = m_rtPanel[1].left - 1;
		rtText.bottom = m_rtPanel[1].top - 5;
		strText.Format("(%s)", m_DataInfo.szVUnitName);
		ShowText(strText, rtText, clrUse, 9, 2);
	}

	//c. draw horizontal mark
	{
		//draw horiz mark
		DVCT_LINE_DATA *pLine = m_DataInfo.pLineData + m_lCurrLine;
		DVCT_UNIT_DATA *pUnit = pLine->pUnitData;
		long lX = m_rtPanel[1].left;
		for (a = m_lStartPos - 1; a < m_lStartPos + m_lOnePageNum; a++, lX += m_lCellWidth)
		{
			if (a < 0)
				continue;
			else if (a >= m_lTotalNum)
				break;
			
			strText.Format("%s", (pUnit + a)->szMarkName);

			long lMargn = 1;

			//draw text
			if (strText.GetLength() > 0)
			{
				lMargn = 3;
				
				rtText.left = lX - m_lCellWidth * 2;
				rtText.top = m_rtPanel[1].bottom + lMargn;
				rtText.right = lX + m_lCellWidth * 2;
				rtText.bottom = m_rtPanel[0].bottom;

				ShowText(strText, rtText, clrUse, 2, 2);
			}

			//draw mark
			m_MemDC.MoveTo(lX, m_rtPanel[1].bottom + lMargn);
			m_MemDC.LineTo(lX, m_rtPanel[1].bottom);
		}

		//draw horiz unit
		rtText.left = m_rtPanel[1].right - 30;
		rtText.top = m_rtPanel[1].bottom;
		rtText.right = m_rtPanel[1].right + 30;
		rtText.bottom = m_rtPanel[0].bottom;
		strText.Format("(%s)", m_DataInfo.szHUnitName);
		ShowText(strText, rtText, clrUse, 2, 2);
	}

	m_MemDC.SelectObject(pOldPen);
}

void CDvChart::GetVMarkInfo()
{
	memset(&m_VMarkInfo, 0, sizeof(DVCT_VMARK_INFO));

	long a, b;
	long lCalNum = 0;
	long *plData = NULL;
	float *pfData = NULL;
	BOOL bInt = m_DataInfo.bIsInt;

	//1. clip data.
	{
		DVCT_LINE_DATA *pLineData = m_DataInfo.pLineData;
		long lNum = m_DataInfo.lLineNum;

		if (m_bIsColumn || !m_DataInfo.bShowMultiData)
		{
			pLineData = m_DataInfo.pLineData + m_lCurrLine;
			lNum = 1;
		}
	
		BOOL bHavePervData = (m_lStartPos > 0) ? TRUE : FALSE;
		long lShowDataNum = GetShowNum();

		if (bHavePervData)
			lShowDataNum++;

		lCalNum = lShowDataNum * lNum;

		if (bInt)
		{
			plData = new long[lCalNum];
			memset(plData, 0, sizeof(long) * lCalNum);
		}
		else
		{
			pfData = new float[lCalNum];
			memset(pfData, 0, sizeof(float) * lCalNum);
		}

		long *plDataTmp = plData;
		float *pfDataTmp = pfData;
		for (a = 0; a < lNum; a++, pLineData++)
		{
			long bStart = bHavePervData ? -1 : 0;
			for (b = bStart; b < bStart + lShowDataNum; b++)
			{				
				if (bInt)
					*plDataTmp++ = (pLineData->pUnitData + m_lStartPos + b)->lVal;
				else
					*pfDataTmp++ = (pLineData->pUnitData + m_lStartPos + b)->fVal;
			}
		}
	}

	//2. get min/max val
	long lMin, lMax;
	float fMin, fMax;
	lMin = lMax = 0;
	fMin = fMax = 0.0;
	{
		for (a = 0; a < lCalNum; a++)
		{
			if (bInt)
			{
				long lVal = *(plData + a);
				if (a == 0)
					lMin = lMax = lVal;
				else
				{
					lMin = (lMin > lVal) ? lVal : lMin;
					lMax = (lMax < lVal) ? lVal : lMax;
				}
			}
			else
			{
				float fVal = *(pfData + a);
				if (a == 0)
					fMin = fMax = fVal;
				else
				{
					fMin = (fMin > fVal) ? fVal : fMin;
					fMax = (fMax < fVal) ? fVal : fMax;
				}
			}
		}
	}

	//3. get value step
	{
		m_VMarkInfo.lMarkNum = 4;
		m_VMarkInfo.lMarkStep = m_rtPanel[1].Height() / m_VMarkInfo.lMarkNum;
		m_VMarkInfo.lValMin = lMin;
		m_VMarkInfo.fValMin = fMin;

		if (bInt)
		{			
			long lStep = (lMax - lMin) / 4;
			if (lStep == 0)
				lStep = 1;
			else
			{
				if ((lMax - lMin) % 4)
					lStep += 1;
			}

			m_VMarkInfo.lValStep = lStep;
			m_VMarkInfo.fStepRatio = (float)(m_VMarkInfo.lMarkStep * 1.0 / lStep);
		}
		else
		{
			float fStep = (fMax - fMin) / 4;
			if (fStep < 0.0001)
				fStep = (float)0.0001;

			m_VMarkInfo.fValStep = fStep;
			m_VMarkInfo.fStepRatio = (float)(m_VMarkInfo.lMarkStep * 1.0 / fStep);
		}
	}

	//4. free mem
	if (bInt)
		delete plData;
	else
		delete pfData;
}

void CDvChart::DrawKLine()
{
	if (m_bIsColumn)
	{
		DVCT_LINE_DATA *pLineData = m_DataInfo.pLineData + m_lCurrLine;
		DrawKLineColumn(pLineData);
	}
	else
	{		
		if (m_DataInfo.bShowMultiData)
		{
			DVCT_LINE_DATA *pLineData = m_DataInfo.pLineData;
			for (long a = 0; a < m_DataInfo.lLineNum; a++, pLineData++)
			{
				BOOL bActive = (a == m_lCurrLine) ? TRUE : FALSE;
				DrawKLineEach(pLineData, bActive);
			}
		}
		else
		{
			DVCT_LINE_DATA *pLineData = m_DataInfo.pLineData + m_lCurrLine;
			DrawKLineEach(pLineData, TRUE);
		}
	}
}

void CDvChart::DrawKLineColumn(DVCT_LINE_DATA *pLineData)
{
	COLORREF clrUsed[3];
	clrUsed[0] = (m_lShowType == 1) ? pLineData->clrLine : m_BaseInfo.clrGray;
	clrUsed[1] = HightLight(clrUsed[0], -30);
	clrUsed[2] = HightLight(clrUsed[0], -60);

	CPen pen[3], *pOldPen;
	pen[0].CreatePen(PS_SOLID, 1, clrUsed[0]);
	pen[1].CreatePen(PS_SOLID, 1, clrUsed[1]);
	pen[2].CreatePen(PS_SOLID, 1, clrUsed[2]);
	pOldPen = m_MemDC.SelectObject(&pen[0]);

	CBrush brs[3], *pOldBrush;
	brs[0].CreateSolidBrush(clrUsed[0]);
	brs[1].CreateSolidBrush(clrUsed[1]);
	brs[2].CreateSolidBrush(clrUsed[2]);
	pOldBrush = m_MemDC.SelectObject(&brs[0]);

	CRgn rgnUse;
	rgnUse.CreateRectRgn(m_rtPanel[1].left + 2, m_rtPanel[1].top - 5, 
						m_rtPanel[1].right - 2, m_rtPanel[1].bottom);
	m_MemDC.SelectClipRgn(&rgnUse);	

	long a, lX;
	lX = m_rtPanel[1].left;
	for (a = m_lStartPos - 1; a < m_lStartPos + GetShowNum(); a++, lX += m_lCellWidth)
	{
		if (a < 0)
			continue;

		long lPos = GetCurrValPos(pLineData, a);
		long lMargn = (long)(m_lCellWidth * 0.4);

		//draw front panel
		m_MemDC.SelectObject(&pen[0]);
		m_MemDC.SelectObject(&brs[0]);		
		CRect rtFront(lX - lMargn, lPos, lX + lMargn, m_rtPanel[1].bottom - 1);
		m_MemDC.Rectangle(rtFront);
		
		//draw top panel
		m_MemDC.SelectObject(&pen[1]);
		m_MemDC.SelectObject(brs[1]);
		long lFrontX, lSideX, lTopX1, lTopX2, lTopY;
		lFrontX = 2 * lMargn;
		lSideX = (long)(lFrontX / 3);
		lTopX1 = lX - lMargn + lSideX;
		lTopX2 = lX + lMargn + lSideX;
		lTopY = lPos - lSideX;

		POINT ptTop[4] =
		{
			{ lX - lMargn, lPos }, 
			{ lTopX1, lTopY }, 
			{ lTopX2, lTopY }, 
			{ lX + lMargn, lPos }, 
		};
					
		m_MemDC.Polygon(ptTop, 4);

		//draw side panel
		m_MemDC.SelectObject(&pen[2]);
		m_MemDC.SelectObject(brs[2]);
		POINT ptSide[4] =
		{
			{ lX + lMargn, lPos }, 
			{ lTopX2, lTopY }, 
			{ lTopX2, m_rtPanel[1].bottom - lSideX }, 
			{ lX + lMargn, m_rtPanel[1].bottom - 1 },
		};
					
		m_MemDC.Polygon(ptSide, 4);
	}

	m_MemDC.SelectClipRgn(NULL);
	m_MemDC.SelectObject(pOldPen);
	m_MemDC.SelectObject(pOldBrush);
}

void CDvChart::DrawKLineEach(DVCT_LINE_DATA *pLineData, BOOL bActive)
{
	COLORREF clrUsed = (m_lShowType == 1) ? pLineData->clrLine : m_BaseInfo.clrGray;

	CPen pen, *pOldPen;
	pen.CreatePen(PS_SOLID, 1, clrUsed);
	pOldPen = m_MemDC.SelectObject(&pen);

	long a, lX;
	BOOL bFirst = TRUE;
	lX = m_rtPanel[1].left;
	for (a = m_lStartPos - 1; a < m_lStartPos + GetShowNum(); a++, lX += m_lCellWidth)
	{
		if (a < 0)
			continue;

		long lPos = GetCurrValPos(pLineData, a);

		//draw line
		if (bFirst)
		{
			m_MemDC.MoveTo(lX, lPos);
			bFirst = FALSE;
		}
		else
			m_MemDC.LineTo(lX, lPos);

		//draw active flag
		if (bActive)
		{
			CRect rtFlag(lX - 2, lPos - 2, lX + 2, lPos + 2);
			m_MemDC.FillSolidRect(rtFlag, clrUsed);
		}
	}

	m_MemDC.SelectObject(pOldPen);
}

long CDvChart::GetCurrValPos(DVCT_LINE_DATA *pLineData, long lCurrPos)
{
	long lPos = m_rtPanel[1].bottom;
	
	if (m_DataInfo.bIsInt)
	{		
		long lVal = (pLineData->pUnitData + lCurrPos)->lVal;
		lVal -= m_VMarkInfo.lValMin;
		long lOffSet = (long)(lVal * m_VMarkInfo.fStepRatio);
		lPos -= lOffSet;
	}
	else
	{
		float fVal = (pLineData->pUnitData + lCurrPos)->fVal;
		fVal -= m_VMarkInfo.fValMin;
		long lOffSet = (long)(fVal * m_VMarkInfo.fStepRatio);
		lPos -= lOffSet;
	}

	return lPos;
}

void CDvChart::DrawCursor()
{
	DVCT_LINE_DATA *pLineData = m_DataInfo.pLineData + m_lCurrLine;

	COLORREF clrUsed = m_BaseInfo.clrGray;
	if (m_lShowType == 1)
		clrUsed = HightLight(clrUsed, 20);

	CPen pen, *pOldPen;
	pen.CreatePen(PS_SOLID, 1, clrUsed);
	pOldPen = m_MemDC.SelectObject(&pen);
	
	long lX = m_rtPanel[1].left + (m_lCurrPos - m_lStartPos + 1) * m_lCellWidth;
	m_MemDC.MoveTo(lX, m_rtPanel[1].top + 1);
	m_MemDC.LineTo(lX, m_rtPanel[1].bottom - 1);

	long lY = GetCurrValPos(pLineData, m_lCurrPos);
	m_MemDC.MoveTo(m_rtPanel[1].left + 1, lY);
	m_MemDC.LineTo(m_rtPanel[1].right - 1, lY);

	m_MemDC.SelectObject(pOldPen);

	//draw value
	DrawValue();
}

void CDvChart::DrawValue()
{
	CRect rtText(m_rtPanel[5]);
	rtText.left += 10;
	rtText.right -= 10;
	rtText.bottom -= 2;

	CString strText;	
	CSize szText;

	DVCT_LINE_DATA *pLineData = m_DataInfo.pLineData + m_lCurrLine;
	DVCT_UNIT_DATA *pUnitData = pLineData->pUnitData + m_lCurrPos;

	//1. draw period info
	COLORREF clrPeriod = (m_lShowType == 1) ? m_BaseInfo.clrFrame : m_BaseInfo.clrGray;		
	strText.Format("  %s  ", pUnitData->szPeriodName);
	ShowText(strText, rtText, clrPeriod, 7, 1);
	szText = m_MemDC.GetTextExtent(strText);
	rtText.left += szText.cx;

	//2. draw val info
	if (m_bIsColumn)
	{		
		if (m_DataInfo.bIsInt)
			strText.Format("%s:%d ", pLineData->szName, pUnitData->lVal);
		else
		{
			float fVal = pUnitData->fVal;			
			if (m_DataInfo.bShowPercent)
			{
				fVal *= 100;
				strText.Format("%s:%.2f%% ", pLineData->szName, fVal);
			}
			else
				strText.Format("%s:%.2f ", pLineData->szName, fVal);
		}

		COLORREF clrLine = (m_lShowType == 1) ? pLineData->clrLine : m_BaseInfo.clrGray;
		ShowText(strText, rtText, clrLine, 7, 1);
	}
	else
	{
		long lCurrIndex = m_lCurrLine;

		long lCnt = 0;
		while (lCnt < m_DataInfo.lLineNum)
		{
			pLineData = m_DataInfo.pLineData + lCurrIndex;
			pUnitData = pLineData->pUnitData + m_lCurrPos;

			if (m_DataInfo.bIsInt)
				strText.Format("%s:%d ", pLineData->szName, pUnitData->lVal);
			else
			{
				float fVal = pUnitData->fVal;
				if (m_DataInfo.bShowPercent)
				{
					fVal *= 100;
					strText.Format("%s:%.2f%% ", pLineData->szName, fVal);
				}
				else
					strText.Format("%s:%.2f ", pLineData->szName, fVal);
			}

			COLORREF clrLine = (m_lShowType == 1) ? pLineData->clrLine : m_BaseInfo.clrGray;
			ShowText(strText, rtText, clrLine, 7, 1);

			szText = m_MemDC.GetTextExtent(strText);
			rtText.left += szText.cx;

			lCurrIndex++;

			if (lCurrIndex >= m_DataInfo.lLineNum)
				lCurrIndex = 0;

			lCnt++;

			if (rtText.left >= rtText.right)
				break;
		}
	}
}

long CDvChart::GetShowNum()
{
	long lShowDataNum = m_lTotalNum - m_lStartPos;
	if (lShowDataNum > m_lOnePageNum)
		lShowDataNum = m_lOnePageNum;

	return lShowDataNum;
}

COLORREF CDvChart::HightLight(COLORREF clrRef, long lPercent)
{
	COLORREF clrRet = clrRef;

	long lR, lG, lB;

	lR = ((clrRef >>  0) & 0xff);
	lG = ((clrRef >>  8) & 0xff);
	lB = ((clrRef >> 16) & 0xff);

	long lRChange = (long)(lR * 0.01 * lPercent);
	long lGChange = (long)(lR * 0.01 * lPercent);
	long lBChange = (long)(lR * 0.01 * lPercent);

	lR = (lR + lRChange > 255) ? 255 : (lR + lRChange);
	lG = (lG + lGChange > 255) ? 255 : (lG + lGChange);
	lB = (lB + lBChange > 255) ? 255 : (lB + lBChange);

	lR = (lR > 0) ? lR : 0;
	lG = (lG > 0) ? lG : 0;
	lB = (lB > 0) ? lB : 0;

	clrRet = lR + (lG << 8) + (lB << 16);

	return clrRet;
}
