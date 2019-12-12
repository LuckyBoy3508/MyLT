
#include "stdafx.h"
#include "mylt.h"
#include "DrawCtrl.h"
#include "StatHelper.h"
#include "RgnTmplManageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////////
////IDrawCtrl
IDrawCtrl::IDrawCtrl()
{
	m_szName[0] = 0;

	m_pDataInfo = NULL;
	m_lDataNum = 0;
	m_lSelFrom = m_lSelFor = 0;
	m_bHaveSpec = FALSE;

	m_rtShow.SetRectEmpty();

	m_lStartPos = 0;
	m_lCurrPos = 0;
	m_lOnePageNum = 0;
	m_lScrollRange = 0;
	m_lTotalNum = 0;
	m_lCellWidth = 0;
}

IDrawCtrl::~IDrawCtrl()
{
	;
}

char *IDrawCtrl::GetName()
{
	return m_szName;
}

BOOL IDrawCtrl::RWCfg(BOOL bIsRead)
{
	return TRUE;
}

void IDrawCtrl::SetData(LT_DATA_INFO *pData, long lDataNum, 
					long lSelFrom, long lSelFor, BOOL bHaveSpec)
{
	m_pDataInfo = pData;
	m_lDataNum = lDataNum;
	m_lSelFrom = lSelFrom;
	m_lSelFor = lSelFor;
	m_bHaveSpec = bHaveSpec;
}

void IDrawCtrl::SetParam(RECT *pRect)
{
	if (pRect)
		m_rtShow.SetRect(pRect->left, pRect->top, pRect->right, pRect->bottom);
	else
		m_rtShow.SetRectEmpty();
}

void IDrawCtrl::SetScrollPos(long lStartPos, long lCurrPos)
{
	;
}

void IDrawCtrl::GetObjState(BOOL bBtns[6], long &lStartPos, long &lCurrPos, 
						long &lOnePageNum, long &lScrollRange, long &lTotalNum)
{
	;
}

BOOL IDrawCtrl::OnKeyDown(UINT nChar, BOOL bIsCtrlKeyDown)
{
	return FALSE;
}

BOOL IDrawCtrl::OnLButtonDown(UINT nFlags, POINT point)
{
	return FALSE;
}

BOOL IDrawCtrl::OnMouseMove(UINT nFlags, POINT point)
{
	return FALSE;
}

void IDrawCtrl::Draw(HDC hDC, long lShowType)
{
	;
}

CMenu *IDrawCtrl::GetPopupMenu(CMenu *pSrcMenu, UINT nStartMenuID)
{
	return pSrcMenu;
}

void IDrawCtrl::OnCommand(UINT uCmdID)
{
	;
}

BOOL IDrawCtrl::AdjustStartPosByCurrPos(long lNewCurrPos, 
					long &lOldCurrPos, long &lStartPos, long lOnePageNum, long lTotalNum)
{
	BOOL bRet = FALSE;

	if (lNewCurrPos < 0)
		lNewCurrPos = 0;
	else if (lNewCurrPos > lTotalNum - 1)
		lNewCurrPos = lTotalNum - 1;
	
	if (lNewCurrPos != lOldCurrPos)
	{
		bRet = TRUE;

		lOldCurrPos = lNewCurrPos;

		if (lNewCurrPos < lStartPos)
			lStartPos = lNewCurrPos;
		else if (lNewCurrPos > lStartPos + lOnePageNum - 1)
			lStartPos = lNewCurrPos - lOnePageNum + 1;
	}

	return bRet;
}

void IDrawCtrl::ShowText(LPCTSTR lpszText, RECT *pRect, COLORREF clrText, long lPos, long lFontIndex)
{
	if (lpszText && pRect && !::IsRectEmpty(pRect) && m_MemDC.GetSafeHdc() &&		
		lPos >= 1 && lPos <= 9 &&
		lFontIndex >= 0 && lFontIndex < 6)
	{
		COLORREF clrOld = m_MemDC.SetTextColor(clrText);
		
		CDrawRes::FONT_STYLE eStyle = (CDrawRes::FONT_STYLE)(CDrawRes::FONT_CH_BIG + lFontIndex);
		CFont *pOldFont = m_MemDC.SelectObject(g_DataInfo.GetFont(eStyle));

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

		CString strTmp(lpszText);
		m_MemDC.DrawText(strTmp, pRect, uPos);

		m_MemDC.SetTextColor(clrOld);
		m_MemDC.SelectObject(pOldFont);
	}
}

BOOL IDrawCtrl::IsHaveDrawData()
{
	if (m_pDataInfo && m_lDataNum > 0 && !m_rtShow.IsRectEmpty())
		return TRUE;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////
////IAnalyseDrawCtrl
IAnalyseDrawCtrl::IAnalyseDrawCtrl()
{
	;
}

IAnalyseDrawCtrl::~IAnalyseDrawCtrl()
{
	;
}

void IAnalyseDrawCtrl::SetData(LT_DATA_INFO *pData, long lDataNum, 
					long lSelFrom, long lSelFor, BOOL bHaveSpec)
{
	IDrawCtrl::SetData(pData, lDataNum, lSelFrom, lSelFor, bHaveSpec);
}

void IAnalyseDrawCtrl::SetParam(RECT *pRect)
{
	IDrawCtrl::SetParam(pRect);
}

void IAnalyseDrawCtrl::SetScrollPos(long lStartPos, long lCurrPos)
{
	m_lStartPos = lStartPos;
	m_lCurrPos = lCurrPos;
}

void IAnalyseDrawCtrl::GetObjState(BOOL bBtns[6], long &lStartPos, long &lCurrPos, 
					long &lOnePageNum, long &lScrollRange, long &lTotalNum)
{
	lStartPos = m_lStartPos;
	lCurrPos = m_lCurrPos;
	lOnePageNum = m_lOnePageNum;
	lScrollRange = m_lScrollRange;
	lTotalNum = m_lTotalNum;

	bBtns[0] = bBtns[1] = FALSE;

	if (m_lCurrPos > 0)
		bBtns[2] = TRUE;

	if (m_lCurrPos < m_lTotalNum - 1)
		bBtns[3] = TRUE;

	if (m_lStartPos > 0)
		bBtns[4] = TRUE;
	
	if (m_lStartPos < m_lScrollRange)
		bBtns[5] = TRUE;
}

BOOL IAnalyseDrawCtrl::OnKeyDown(UINT nChar, BOOL bIsCtrlKeyDown)
{
	BOOL bRet = FALSE;

	long lNewPos = m_lCurrPos;

	switch (nChar)
	{
	case VK_UP:
		lNewPos--;
		break;
	case VK_DOWN:
		lNewPos++;
		break;
	case VK_HOME:
		lNewPos = 0;
		break;
	case VK_END:
		lNewPos = m_lTotalNum - 1;
		break;
	case VK_PRIOR:
		lNewPos -= m_lOnePageNum;
		break;
	case VK_NEXT:
		lNewPos += m_lOnePageNum;
		break;
	default:
		return FALSE;
		break;
	}

	bRet = AdjustStartPosByCurrPos(lNewPos, m_lCurrPos, m_lStartPos, m_lOnePageNum, m_lTotalNum);
	
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////////
////CMainAnalyse
CMainAnalyse::CMainAnalyse()
{
	strcpy(m_szName, "关系图");	

	for (long a = 0; a < 8; a++)
		m_rtPanel[a].SetRectEmpty();

	m_pDWData = NULL;
	m_lDWDataNum = 0;

	m_pDWCell = NULL;
	m_lDWCellNum = 0;

	m_pDWLine = m_pDWLineTemp = NULL;
	m_lDWLineNum = 0;
	m_bShowLinkLine = TRUE;

	m_lCurrSameRgn = 0;
	m_bShowSameRgn = FALSE;	
	
	m_bDrawShowLine = m_bDrawShowRange = FALSE;
}

CMainAnalyse::~CMainAnalyse()
{
	if (m_pDWData)
		delete m_pDWData;

	if (m_pDWCell)
		delete m_pDWCell;

	if (m_pDWLine)
		delete m_pDWLine;

	POSITION pos = m_lstSameRgn.GetHeadPosition();
	while (pos)
	{
		LTDC_SAMERGN_INFO *pTmp = m_lstSameRgn.GetNext(pos);
		delete pTmp;
	}
	m_lstSameRgn.RemoveAll();

	FreeRgnDataMem();
}

void CMainAnalyse::FreeRgnDataMem()
{
	POSITION pos = m_lstRgnData.GetHeadPosition();
	while (pos)
	{
		LTDC_MARGN *pTmp = m_lstRgnData.GetNext(pos);
		delete pTmp->plData;
		delete pTmp;
	}
	m_lstRgnData.RemoveAll();
}

BOOL CMainAnalyse::RWCfg(BOOL bIsRead)
{
	CDvXML &dvXml = theApp.m_DvXml;

	DVXML_NODE *pRoot = dvXml.GetRoot();
	DVXML_NODE *pTmpNode, *pSameRgnNode;
	pTmpNode = pSameRgnNode = NULL;

	if (bIsRead)
	{
		pTmpNode = dvXml.OperateNode(pRoot, "MainAnalyse", CDvXML::CHECKONLY);
		if (pTmpNode)
		{
			dvXml.GetAttrib(pTmpNode, "IsShowLinkLine", m_bShowLinkLine);
			dvXml.GetAttrib(pTmpNode, "IsShowSameRgn", m_bShowSameRgn);
			dvXml.GetAttrib(pTmpNode, "CurrSelRgn", m_lCurrSameRgn);

			pSameRgnNode = dvXml.OperateNode(pTmpNode, "SameRgns", CDvXML::CHECKONLY);
			RWCfgSameRgn(pSameRgnNode, bIsRead);
		}
	}
	else
	{
		pTmpNode = dvXml.OperateNode(pRoot, "MainAnalyse", CDvXML::DELETECREATE);

		dvXml.SetAttrib(pTmpNode, "IsShowLinkLine", m_bShowLinkLine);
		dvXml.SetAttrib(pTmpNode, "IsShowSameRgn", m_bShowSameRgn);
		dvXml.SetAttrib(pTmpNode, "CurrSelRgn", m_lCurrSameRgn);

		pSameRgnNode = dvXml.OperateNode(pTmpNode, "SameRgns", CDvXML::DELETECREATE);
		RWCfgSameRgn(pSameRgnNode, bIsRead);
	}

	return TRUE;
}

void CMainAnalyse::RWCfgSameRgn(DVXML_NODE *pSameRgnNode, BOOL bIsRead)
{
	DVXML_NODE *pTmpNode = NULL;
	
	POSITION pos = NULL;
	LTDC_SAMERGN_INFO *pSameRgn = NULL; 
	
	if (bIsRead)
	{
		DVXML_NODE_LIST lstTmp;
		CDvXML::SelectNodes(pSameRgnNode, "SameRgn", lstTmp);
		pos = lstTmp.GetHeadPosition();
		while (pos)
		{
			pTmpNode = lstTmp.GetNext(pos);

			pSameRgn = new LTDC_SAMERGN_INFO;
			m_lstSameRgn.AddTail(pSameRgn);

			CDvXML::GetAttrib(pTmpNode, "Name", pSameRgn->szName);

			CDvXML::GetAttrib(pTmpNode, "AllowRotate90", pSameRgn->b90);
			CDvXML::GetAttrib(pTmpNode, "AllowRotate180", pSameRgn->b180);
			CDvXML::GetAttrib(pTmpNode, "AllowRotate270", pSameRgn->b270);
			CDvXML::GetAttrib(pTmpNode, "AllowHorizMirror", pSameRgn->bHMirror);
			CDvXML::GetAttrib(pTmpNode, "AllowVertMirror", pSameRgn->bVMirror);
					
			RWCfgSameRgnEachRow(pTmpNode, pSameRgn, bIsRead);
		}
	}
	else
	{
		pos = m_lstSameRgn.GetHeadPosition();
		while (pos)
		{
			pSameRgn = m_lstSameRgn.GetNext(pos);

			pTmpNode = CDvXML::OperateNode(pSameRgnNode, "SameRgn", CDvXML::CREATEONLY);

			CDvXML::SetAttrib(pTmpNode, "Name", pSameRgn->szName);

			CDvXML::SetAttrib(pTmpNode, "AllowRotate90", pSameRgn->b90);
			CDvXML::SetAttrib(pTmpNode, "AllowRotate180", pSameRgn->b180);
			CDvXML::SetAttrib(pTmpNode, "AllowRotate270", pSameRgn->b270);
			CDvXML::SetAttrib(pTmpNode, "AllowHorizMirror", pSameRgn->bHMirror);
			CDvXML::SetAttrib(pTmpNode, "AllowVertMirror", pSameRgn->bVMirror);

			RWCfgSameRgnEachRow(pTmpNode, pSameRgn, bIsRead);				
		}
	}
}

void CMainAnalyse::RWCfgSameRgnEachRow(DVXML_NODE *pRgnRowNode, 
									   LTDC_SAMERGN_INFO *pSameRgn, BOOL bIsRead)
{
	DVXML_NODE *pTmpNode = NULL;
	
	POSITION pos = NULL;
	CString strVal;
	
	if (bIsRead)
	{
		long lRow = 0;

		DVXML_NODE_LIST lstTmp;
		CDvXML::SelectNodes(pRgnRowNode, "Row", lstTmp);			
		pos = lstTmp.GetHeadPosition();
		while (pos)
		{
			pTmpNode = lstTmp.GetNext(pos);
		
			if (CDvXML::GetInnerText(pTmpNode, strVal))
			{
				char szRowVal[LTDC_DWRGNGRID_COL + 1] = {0};
				strcpy(szRowVal, strVal);

				for (long a = 0; a < LTDC_DWRGNGRID_COL; a++)
				{
					pSameRgn->lRgnMap[lRow][a] = 0;
					if (szRowVal[a] == '1')
						pSameRgn->lRgnMap[lRow][a] = 1;
				}
			}

			lRow++;
		}
	}
	else
	{
		for (long a = 0; a < LTDC_DWRGNGRID_ROW; a++)
		{
			pTmpNode = CDvXML::OperateNode(pRgnRowNode, "Row", CDvXML::CREATEONLY);

			char szRowVal[LTDC_DWRGNGRID_COL + 1] = {0};
			for (long b = 0; b < LTDC_DWRGNGRID_COL; b++)
			{
				szRowVal[b] = '0';
				if (pSameRgn->lRgnMap[a][b] == 1)
					szRowVal[b] = '1';
			}

			strVal = szRowVal;

			CDvXML::SetInnerText(pTmpNode, strVal);
		}
	}
}

void CMainAnalyse::SetData(LT_DATA_INFO *pData, long lDataNum, 
					long lSelFrom, long lSelFor, BOOL bHaveSpec)
{
	IAnalyseDrawCtrl::SetData(pData, lDataNum, lSelFrom, lSelFor, bHaveSpec);

	InitDrawData();
}

void CMainAnalyse::SetParam(RECT *pRect)
{
	IAnalyseDrawCtrl::SetParam(pRect);

	//get all show rect.
	CalcPanelsRect();

	//init all draw data.
	InitDrawData();
}

void CMainAnalyse::CalcPanelsRect()
{	
	if (IsHaveDrawData())
	{
		long x, y, lWidth, lHeight, xdt, ydt;
		x = y = lWidth = lHeight = xdt = ydt = 0;

		//wk
		lWidth = m_rtShow.Width();
		lHeight = m_rtShow.Height() - 20;

		m_lCellWidth = lWidth / (m_lSelFrom + LTDC_DWPERIOD_WIDTH);

		x = m_lSelFrom;
		y = lHeight / m_lCellWidth - LTDC_DWTITLE_HEIGHT;

		//set if can scroll & on page number & start pos
		m_lOnePageNum = y;
		m_lScrollRange = m_lDataNum + 1 - m_lOnePageNum;	//add 1 means the will period.
		if (m_lScrollRange < 0)
			m_lScrollRange = 0;

		m_lStartPos = m_lScrollRange;
		m_lCurrPos = m_lDataNum - 1;
		m_lTotalNum = m_lDataNum + 1;

		//adjust show rect
		xdt = (lWidth - m_lCellWidth * (m_lSelFrom + LTDC_DWPERIOD_WIDTH)) / 2;
		ydt = (lHeight - m_lCellWidth * (y + LTDC_DWTITLE_HEIGHT)) / 2;
		
		m_rtPanel[0].SetRect(m_rtShow.left + xdt, m_rtShow.top + 20 + ydt,
							m_rtShow.right - xdt, m_rtShow.bottom - ydt);
		m_rtPanel[1].SetRect(m_rtPanel[0].left + m_lCellWidth * LTDC_DWPERIOD_WIDTH, 
							m_rtPanel[0].top + m_lCellWidth * LTDC_DWTITLE_HEIGHT,
							m_rtPanel[0].right, m_rtPanel[0].bottom);		

		//title
		m_rtPanel[2].SetRect(m_rtShow.left, m_rtShow.top, m_rtShow.right, m_rtShow.top + 18);
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

void CMainAnalyse::InitDrawData()
{	
	if (IsHaveDrawData())
	{
		GetDWLinKArray();
		GetDWCellArray();
		GetDWLineArray();
		GetDWRgnArray();
	}
}

void CMainAnalyse::GetDWLinKArray()
{
	if (m_pDWData)
		delete m_pDWData;
	m_lDWDataNum = m_lDataNum;
	m_pDWData = new	LTDC_MADATA[m_lDWDataNum];	
	memset(m_pDWData, 0, sizeof(LTDC_MADATA) * m_lDWDataNum);

	LTDC_MADATA *pDWDataTemp = m_pDWData;
	LT_DATA_INFO *pDataInfoTemp = m_pDataInfo;
	for (long a = 0; a < m_lDWDataNum; a++, pDWDataTemp++, pDataInfoTemp++)
	{
		pDWDataTemp->lPeriod = pDataInfoTemp->lPeriodID;

		for (long b = 0; b < m_lSelFor; b++)
		{
			pDWDataTemp->eachData[b].lNo = pDataInfoTemp->lData[b];

			pDWDataTemp->eachData[b].bIsSpec = FALSE;
			if (b == m_lSelFor - 1 && m_bHaveSpec)
				pDWDataTemp->eachData[b].bIsSpec = TRUE;
		}
	}

	GetHLink();
	GetVLine();
}

void CMainAnalyse::GetHLink()
{
	long a, b, c;

	LT_DATA_INFO *pDataInfoTemp = m_pDataInfo;
	LTDC_MADATA *pDWDataTemp = m_pDWData;
	for (a = 0; a < m_lDataNum; a++, pDataInfoTemp++, pDWDataTemp++)
	{
		long lTotalNum[LT_MAX_SELFROM] = {0};
		for (b = 0; b < m_lSelFor; b++)
		{//有球赋值为1
			lTotalNum[pDataInfoTemp->lData[b] - 1] = 1;
		}

		for (b = 0; b < m_lSelFrom; b++)
		{//查找相连大于等于3个
			if (lTotalNum[b])
			{
				long lBegin, lEnd;
				lBegin = lEnd = b;

				for (lEnd = b + 1; lEnd < m_lSelFrom; lEnd++)
				{
					if (!lTotalNum[lEnd])
						break;
				}

				if (lEnd - lBegin >= 3)
				{
					for (c = lBegin; c < lEnd; c++)
						lTotalNum[c] = 2; //相连数大于3个
				}

				b = lEnd;
			}
		}

		for (b = 0; b < m_lSelFrom; b++)
		{//赋值相连
			if (lTotalNum[b] == 2)
			{
				for (c = 0; c < m_lSelFor; c++)
				{
					if (pDWDataTemp->eachData[c].lNo == b + 1)
					{
						pDWDataTemp->eachData[c].bIsLink = TRUE;
						break;
					}
				}
			}
		}
	}
}

void CMainAnalyse::GetVLine()
{
	long a, b, c;

	long *plTotal = new long[m_lDataNum];
	long *plTotalTemp = plTotal;

	for (a = 0; a < m_lSelFrom; a++)
	{
		memset(plTotal, 0, sizeof(long) * m_lDataNum);
		plTotalTemp = plTotal;

		LT_DATA_INFO *pDataInfoTemp = m_pDataInfo;
		for (b = 0; b < m_lDataNum; b++, pDataInfoTemp++, plTotalTemp++)
		{
			for (c = 0; c < m_lSelFor; c++)
			{
				if (pDataInfoTemp->lData[c] == a + 1)
				{
					*plTotalTemp = 1;
					break;
				}
			}
		}

		for (b = 0; b < m_lDataNum; b++)
		{
			if (plTotal[b])
			{
				long lBegin, lEnd;
				lBegin = lEnd = b;

				for (lEnd = b + 1; lEnd < m_lDataNum; lEnd++)
				{
					if (!plTotal[lEnd])
						break;
				}

				if (lEnd - lBegin >= 3)
				{
					for (c = lBegin; c < lEnd; c++)
						plTotal[c] = 2; //相连数大于3个
				}

				b = lEnd;
			}
		}

		LTDC_MADATA *pDWDataTemp = m_pDWData;
		for (b = 0; b < m_lDataNum; b++, pDWDataTemp++)
		{
			if (plTotal[b] == 2)
			{
				for (c = 0; c < m_lSelFor; c++)
				{
					if (pDWDataTemp->eachData[c].lNo == a + 1)
					{
						pDWDataTemp->eachData[c].bIsLink = TRUE;
						break;
					}
				}
			}
		}
	}

	delete plTotal;
}

void CMainAnalyse::GetDWCellArray()
{
	if (m_pDWCell)
		delete m_pDWCell;
	m_lDWCellNum = m_lSelFrom * (m_lOnePageNum + 12); 
	m_pDWCell = new LTDC_MACELL[m_lDWCellNum];
	memset(m_pDWCell, 0, sizeof(LTDC_MACELL) * m_lDWCellNum);

	LTDC_MACELL *pCellTemp = m_pDWCell;
	for (long a = 0; a < m_lDWCellNum; a++, pCellTemp++)
	{
		long x = a % m_lSelFrom;
		long y = a / m_lSelFrom - 6;

		pCellTemp->ptCenter.x = m_rtPanel[1].left + x * m_lCellWidth + m_lCellWidth / 2;
		pCellTemp->ptCenter.y = m_rtPanel[1].top + y * m_lCellWidth + m_lCellWidth / 2;
	}
}

void CMainAnalyse::GetDWLineArray()
{
	if (m_pDWLine)
		delete m_pDWLine;
	m_lDWLineNum = CalTotalLine(TRUE);

	m_pDWLine = new LTDC_MALINE[m_lDWLineNum];
	memset(m_pDWLine, 0, sizeof(LTDC_MALINE) * m_lDWLineNum);
	m_pDWLineTemp = m_pDWLine;

	CalTotalLine(FALSE);
}

long CMainAnalyse::CalTotalLine(BOOL bOnlyCalNum)
{
	long lTotalLineNum, lHaveLineNum, lWillLineNum;
	lTotalLineNum = lHaveLineNum = lWillLineNum = 0;

	lHaveLineNum = CalHaveLine(bOnlyCalNum);
	lWillLineNum = CalWillLine(bOnlyCalNum);
	
	lTotalLineNum = lHaveLineNum + lWillLineNum;

	return lTotalLineNum;
}

long CMainAnalyse::CalHaveLine(BOOL bOnlyCalNum)
{
	long lHaveLineNum = 0;

	LT_DATA_INFO *pDataTemp = m_pDataInfo;
	for (long a = 0; a < m_lDataNum - 2; a++, pDataTemp++)
	{
		for (long b = 0; b < m_lSelFor; b++)
		{
			BOOL bIsSpec = FALSE;
			if (m_bHaveSpec && b == m_lSelFor - 1)
				bIsSpec = TRUE;

			long lLineNum = CalLineAtPoint(a, pDataTemp->lData[b], bIsSpec, bOnlyCalNum);
			lHaveLineNum += lLineNum;
		}
	}

	return lHaveLineNum;
}

long CMainAnalyse::CalLineAtPoint(long lStartPeriod, long lNo, BOOL bIsSpec, BOOL bOnlyCalNum)
{
	long lLineNum = 0;

	//Rule: 2 <= (abs(x) + abs(y)) <= 4
	POINT ptMayMakeLine[12] = 
	{
		{-3, 1}, {-2, 2}, {-2, 1}, {-1, 3}, {-1, 2}, {-1, 1}, 
		{3, 1}, {2, 2}, {2, 1}, {1, 3}, {1, 2}, {1, 1}, 
	};

	for (long a = 0; a < 12; a++)
	{
		long x = ptMayMakeLine[a].x;
		long y = ptMayMakeLine[a].y;

		BOOL bIsHotLine = FALSE;
		if (CanMakeLine(lStartPeriod, lNo, bIsSpec, x, y, bIsHotLine))
		{
			if (!bOnlyCalNum)
			{
				m_pDWLineTemp->ptStartPos.x = lNo - 1;
				m_pDWLineTemp->ptStartPos.y = lStartPeriod;
				m_pDWLineTemp->ptEndPos.x = lNo - 1 + 2 * x;
				m_pDWLineTemp->ptEndPos.y = lStartPeriod + 2 * y;
				m_pDWLineTemp->bIsWillLine = FALSE;
				m_pDWLineTemp->bIsHotLine = bIsHotLine;

				m_pDWLineTemp++;

				if (bIsHotLine)
					MakeHotNum(lStartPeriod, lNo, x, y);
			}

			lLineNum++;
		}
	}

	return lLineNum;
}

BOOL CMainAnalyse::CanMakeLine(long lStartPeriod, long lNo, BOOL bIsSpec, 
						   long lXOffSet, long lYOffSet, BOOL &bIsHotLine)
{
	bIsHotLine = FALSE;

	long lSecondPeriod = lStartPeriod + lYOffSet;
	long lSecondNo = lNo + lXOffSet;

	long lThirdPeriod = lStartPeriod + 2 * lYOffSet;
	long lThirdNo = lNo + 2 * lXOffSet;

	if ((lSecondPeriod < m_lDataNum && lThirdPeriod < m_lDataNum) &&
		(lSecondNo > 0 && lSecondNo <= m_lSelFrom) &&
		(lThirdNo > 0 && lThirdNo <= m_lSelFrom))
	{
		BOOL bIsSndSpec, bIsTrdSpec;
		bIsSndSpec = bIsTrdSpec = FALSE;
		if (HaveThisNo(lSecondPeriod, lSecondNo, bIsSndSpec) && HaveThisNo(lThirdPeriod, lThirdNo, bIsTrdSpec))
		{
			if (abs(lXOffSet) == 2 && lYOffSet == 2)
			{
				if (bIsSpec && bIsSndSpec && bIsTrdSpec)
					return TRUE;
				else
					return FALSE;
			}
			
			if (abs(lXOffSet) == 1 && lYOffSet == 1)
				bIsHotLine = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMainAnalyse::HaveThisNo(long lPeriod, long lNo, BOOL &bIsSpec)
{
	LT_DATA_INFO *pTemp = m_pDataInfo + lPeriod;

	bIsSpec = FALSE;
	for (long a = 0; a < m_lSelFor; a++)
	{
		if (pTemp->lData[a] == lNo)
		{
			if (m_bHaveSpec && a == m_lSelFor - 1)
				bIsSpec = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}

void CMainAnalyse::MakeHotNum(long lStartPeriod, long lNo, long lXOffSet, long lYOffSet)
{
	long lPeriod = lStartPeriod;
	long lNowNo = lNo;

	for (long a = 0; a < 3; a++)
	{
		LTDC_MADATA *pDWData = m_pDWData + lPeriod;
		for (long b = 0; b < m_lSelFor; b++)
		{
			if (pDWData->eachData[b].lNo == lNowNo)
			{
				pDWData->eachData[b].bIsLink = TRUE;
				break;
			}
		}

		lNowNo += lXOffSet;
		lPeriod += lYOffSet;
	}
}

long CMainAnalyse::CalWillLine(BOOL bOnlyCalNum)
{
	long lWillLineNum = 0;

	LT_DATA_INFO *pDataTemp = NULL;
	long lStarIndex = m_lDataNum - 2;

	for (long a = 0; a < 5; a++, lStarIndex--)
	{
		if (lStarIndex >= 0)
		{
			pDataTemp = m_pDataInfo + lStarIndex;
			
			for (long b = 0; b < m_lSelFor; b++)
			{
				BOOL bIsSpec = FALSE;
				if (m_bHaveSpec && b == m_lSelFor - 1)
					bIsSpec = TRUE;

				long lLineNum = CalWillLineAtPoint(lStarIndex, pDataTemp->lData[b], bIsSpec, bOnlyCalNum);
				lWillLineNum += lLineNum;
			}
		}
		else
			break;
	}

	return lWillLineNum;
}

long CMainAnalyse::CalWillLineAtPoint(long lStartPeriod, long lNo, BOOL bIsSpec, BOOL bOnlyCalNum)
{
	long lLineNum = 0;

	//Rule: 2 <= (abs(x) + abs(y)) <= 4
	POINT ptMayMakeLine[12] = 
	{
		{-3, 1}, {-2, 2}, {-2, 1}, {-1, 3}, {-1, 2}, {-1, 1}, 
		{3, 1}, {2, 2}, {2, 1}, {1, 3}, {1, 2}, {1, 1}, 
	};

	for (long a = 0; a < 12; a++)
	{
		long x = ptMayMakeLine[a].x;
		long y = ptMayMakeLine[a].y;

		if (CanMakeWillLine(lStartPeriod, lNo, bIsSpec, x, y))
		{
			if (!bOnlyCalNum)
			{
				m_pDWLineTemp->ptStartPos.x = lNo -1;
				m_pDWLineTemp->ptStartPos.y = lStartPeriod;
				m_pDWLineTemp->ptEndPos.x = lNo - 1 + 2 * x;
				m_pDWLineTemp->ptEndPos.y = lStartPeriod + 2 * y;
				m_pDWLineTemp->bIsWillLine = TRUE;

				m_pDWLineTemp++;
			}
			lLineNum++;
		}
	}

	return lLineNum;
}

BOOL CMainAnalyse::CanMakeWillLine(long lStartPeriod, long lNo, BOOL bIsSpec, long lXOffSet, long lYOffSet)
{
	long lSecondPeriod = lStartPeriod + lYOffSet;
	long lSecondNo = lNo + lXOffSet;

	long lThirdPeriod = lStartPeriod + 2 * lYOffSet;
	long lThirdNo = lNo + 2 * lXOffSet;

	if ((lSecondPeriod < m_lDataNum && lThirdPeriod == m_lDataNum) &&
		(lSecondNo > 0 && lSecondNo <= m_lSelFrom) &&
		(lThirdNo > 0 && lThirdNo <= m_lSelFrom))
	{
		BOOL bIsSecSpec = FALSE;
		if (HaveThisNo(lSecondPeriod, lSecondNo, bIsSecSpec))
		{
			if (abs(lXOffSet) == 2 && lYOffSet == 2)
			{
				if (bIsSpec && bIsSecSpec)
					return TRUE;
				else
					return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMainAnalyse::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bRet = FALSE;

	if (m_rtPanel[1].PtInRect(point))
	{
		long lOldPos = m_lCurrPos;

		m_lCurrPos = (point.y - m_rtPanel[1].top) / m_lCellWidth;
		if (m_lCurrPos < 0)
			m_lCurrPos = 0;

		m_lCurrPos += m_lStartPos;

		if (m_lCurrPos > m_lTotalNum - 1)
			m_lCurrPos = m_lTotalNum - 1;
		
		if (m_lCurrPos != lOldPos)
			bRet = TRUE;
	}
	else if (m_bDrawShowLine)
	{
		m_bShowLinkLine ^= 1;
		bRet = TRUE;
	}
	else if (m_bDrawShowRange)
	{
		m_bShowSameRgn ^= 1;
		bRet = TRUE;
	}

	return bRet;
}

BOOL CMainAnalyse::OnMouseMove(UINT nFlags, POINT point)
{
	BOOL bRet = FALSE;

	if (DrawShowLineFlag(TRUE, &point) || DrawShowRangeFlag(TRUE, &point))
		bRet = TRUE;

	return bRet;
}

void CMainAnalyse::Draw(HDC hDC, long lShowType)
{
	m_MemDC.Attach(hDC);

	if (DrawOutlook()) //draw frame/name/title-numbers
	{
		//draw content(link-line, same-rgn, cell)
		DrawContent();

		//draw cursor.
		DrawCursor();
	}

	m_MemDC.Detach();
}

BOOL CMainAnalyse::DrawOutlook()
{
	COLORREF clrColor[2];
	clrColor[0] = g_DataInfo.GetColor(CDrawRes::CLR_FRAME);		//red
	clrColor[1] = g_DataInfo.GetColor(CDrawRes::CLR_CELL);		//blue

	CPen *pPen[3], *pOldPen;
	pPen[0] = g_DataInfo.GetPen(CDrawRes::PEN_FRAME_BOLD);		//frame
	pPen[1] = g_DataInfo.GetPen(CDrawRes::PEN_FRAME_THIN);		//long cell line
	pPen[2] = g_DataInfo.GetPen(CDrawRes::PEN_CELL_THIN);		//short cell line
	pOldPen = m_MemDC.SelectObject(pPen[0]);

	CBrush *pBrush[1], *pOldBrh;
	pBrush[0] = g_DataInfo.GetBrush(CDrawRes::BRS_BKGROUND);	//bkground
	pOldBrh = m_MemDC.SelectObject(pBrush[0]);

	//1. draw frame
	if (IsHaveDrawData())
		m_MemDC.RoundRect(m_rtPanel[0], CPoint(LTDC_CORNER_VALUE, LTDC_CORNER_VALUE));
	else
	{
		ShowText("没有数据！", m_rtShow, clrColor[0], 5, 0);
		return FALSE;
	}

	long a, lHZoneNum, lVZoneNum, lX, lY;
	CString strTmp;
	
	a = lHZoneNum = lVZoneNum = lX = lY = 0;
	g_DataInfo.GetZoneInfo(m_lSelFrom, lHZoneNum, lVZoneNum);

	//2. draw cell frame
	{
		POINT ptStart, ptEnd;

		//draw vertical line	
		lX = m_rtPanel[1].left;
		for (a = 0; a < m_lSelFrom; a++, lX += m_lCellWidth)
		{
			ptStart.x = ptEnd.x = lX;

			if (a % lVZoneNum == 0)
			{
				ptStart.y = m_rtPanel[0].top;
				m_MemDC.SelectObject(pPen[1]);
			}
			else
			{
				ptStart.y = m_rtPanel[0].top + m_lCellWidth;
				m_MemDC.SelectObject(pPen[2]);
			}

			ptEnd.y = m_rtPanel[0].bottom;
				
			ptStart.y += 2;
			ptEnd.y -= 3;

			m_MemDC.MoveTo(ptStart);
			m_MemDC.LineTo(ptEnd);
		}
		
		//draw horizontal
		long lHLineNum = m_lOnePageNum + LTDC_DWTITLE_HEIGHT - 1;
		lY = m_rtPanel[0].top + m_lCellWidth;
		for (a = 0; a < lHLineNum; a++, lY += m_lCellWidth)
		{
			if (a == 0)
				ptStart.x = m_rtPanel[0].left + LTDC_DWPERIOD_WIDTH * m_lCellWidth;
			else
				ptStart.x = m_rtPanel[0].left;

			ptEnd.x = m_rtPanel[0].right;
			ptStart.y = ptEnd.y = lY;

			if (a % lVZoneNum == 0)
				m_MemDC.SelectObject(pPen[1]);
			else
				m_MemDC.SelectObject(pPen[2]);

			ptStart.x += 2;
			ptEnd.x -= 3;

			m_MemDC.MoveTo(ptStart);
			m_MemDC.LineTo(ptEnd);
		}
	}

	//3. draw titles
	{
		//draw name
		ShowText(m_szName, m_rtPanel[4], clrColor[0], 9, 0);

		//draw val's bottom line
		m_MemDC.SelectObject(pPen[1]);
		m_MemDC.MoveTo(m_rtPanel[5].left + 5, m_rtPanel[5].bottom - 1);
		m_MemDC.LineTo(m_rtPanel[5].right - 15, m_rtPanel[5].bottom - 1);

		//draw show line flag
		DrawShowLineFlag();
		
		//draw show range flag
		DrawShowRangeFlag();
	}

	//4. draw zone-name/period-number.
	{
		CRect rtText;
		BOOL bCanShowAll = (m_lSelFrom % lVZoneNum) ? FALSE : TRUE;

		//draw range
		lX = m_rtPanel[0].left + LTDC_DWPERIOD_WIDTH * m_lCellWidth;
		for (a = 0; a < lHZoneNum; a++, lX += lVZoneNum * m_lCellWidth)
		{
			rtText.left = lX;
			rtText.top = m_rtPanel[0].top;

			if (a != lHZoneNum - 1)
				rtText.right = rtText.left + lVZoneNum * m_lCellWidth;
			else
				rtText.right = m_rtPanel[0].right;

			rtText.bottom = rtText.top + m_lCellWidth;

			if (bCanShowAll)
				strTmp.Format("%c 区域", 'A' + a); 
			else
				strTmp.Format("%c", 'A' + a);

			ShowText(strTmp, rtText, clrColor[0], 5, 1);
		}

		//draw sel from number.
		lX = m_rtPanel[0].left + LTDC_DWPERIOD_WIDTH * m_lCellWidth;
		for (a = 0; a < m_lSelFrom; a++, lX += m_lCellWidth)
		{
			rtText.left = lX;
			rtText.top = m_rtPanel[0].top + m_lCellWidth;
			rtText.right = rtText.left + m_lCellWidth;
			rtText.bottom = rtText.top + m_lCellWidth;

			strTmp.Format("%02d", a + 1);
			ShowText(strTmp, rtText, clrColor[1], 5, 4);
		}		
	}

	m_MemDC.SelectObject(pOldPen);
	m_MemDC.SelectObject(pOldBrh);

	return TRUE;
}

BOOL CMainAnalyse::DrawShowLineFlag(BOOL bTest /*= FALSE*/, POINT *pPoint /*= NULL*/)
{
	BOOL bRet = FALSE;

	CRect rtBtn = m_rtPanel[6];
	rtBtn.left += m_rtPanel[6].Width() / 10;
	rtBtn.right -= m_rtPanel[6].Width() / 10;

	if (!bTest)
	{
		COLORREF clrUse;
		CPen *pPen, *pOldPen;		
		if (m_bDrawShowLine)
		{
			clrUse = g_DataInfo.GetColor(CDrawRes::CLR_FRAME);
			pPen = g_DataInfo.GetPen(CDrawRes::PEN_FRAME_THIN);
		}
		else
		{
			clrUse = g_DataInfo.GetColor(CDrawRes::CLR_GRAY);
			pPen = g_DataInfo.GetPen(CDrawRes::PEN_GRAY_THIN);
		}
		pOldPen = m_MemDC.SelectObject(pPen);

		CBrush *pBrush, *pOldBrh;
		pBrush = g_DataInfo.GetBrush(CDrawRes::BRS_BKGROUND);
		pOldBrh = m_MemDC.SelectObject(pBrush);

		m_MemDC.Rectangle(rtBtn);

		CString strText("显示连接线");
		if (m_bShowLinkLine)
			strText = "关闭连接线";

		ShowText(strText, rtBtn, clrUse, 5, 2);

		m_MemDC.SelectObject(pOldPen);
		m_MemDC.SelectObject(pOldBrh);
	}
	else
	{
		BOOL bOld = m_bDrawShowLine;

		m_bDrawShowLine = FALSE;
		if (rtBtn.PtInRect(*pPoint))
			m_bDrawShowLine = TRUE;

		if (bOld != m_bDrawShowLine)
			bRet = TRUE;
	}

	return bRet;
}

BOOL CMainAnalyse::DrawShowRangeFlag(BOOL bTest /*= FALSE*/, POINT *pPoint /*= NULL*/)
{
	BOOL bRet = FALSE;

	CRect rtBtn = m_rtPanel[7];
	rtBtn.left += m_rtPanel[7].Width() / 10;
	rtBtn.right -= m_rtPanel[7].Width() / 10;

	if (!bTest)
	{
		COLORREF clrUse;
		CPen *pPen, *pOldPen;		
		if (m_bDrawShowRange)
		{
			clrUse = g_DataInfo.GetColor(CDrawRes::CLR_FRAME);
			pPen = g_DataInfo.GetPen(CDrawRes::PEN_FRAME_THIN);
		}
		else
		{
			clrUse = g_DataInfo.GetColor(CDrawRes::CLR_GRAY);
			pPen = g_DataInfo.GetPen(CDrawRes::PEN_GRAY_THIN);
		}
		pOldPen = m_MemDC.SelectObject(pPen);

		CBrush *pBrush, *pOldBrh;
		pBrush = g_DataInfo.GetBrush(CDrawRes::BRS_BKGROUND);
		pOldBrh = m_MemDC.SelectObject(pBrush);

		m_MemDC.Rectangle(rtBtn);

		CString strText("显示同区域");
		if (m_bShowSameRgn)
			strText = "关闭同区域";

		ShowText(strText, rtBtn, clrUse, 5, 2);

		m_MemDC.SelectObject(pOldPen);
		m_MemDC.SelectObject(pOldBrh);
	}
	else
	{
		BOOL bOld = m_bDrawShowRange;

		m_bDrawShowRange = FALSE;
		if (rtBtn.PtInRect(*pPoint))
			m_bDrawShowRange = TRUE;

		if (bOld != m_bDrawShowRange)
			bRet = TRUE;
	}

	return bRet;
}

void CMainAnalyse::DrawContent()
{
	COLORREF clrColor[4];
	clrColor[0] = g_DataInfo.GetColor(CDrawRes::CLR_FRAME);		//red
	clrColor[1] = g_DataInfo.GetColor(CDrawRes::CLR_CELL);		//blue
	clrColor[2] = g_DataInfo.GetColor(CDrawRes::CLR_WHITE);		//white
	clrColor[3] = g_DataInfo.GetColor(CDrawRes::CLR_YELLOW);	//yellow

	CPen *pPen[2], *pOldPen;
	pPen[0] = g_DataInfo.GetPen(CDrawRes::PEN_FRAME_THIN);		//red
	pPen[1] = g_DataInfo.GetPen(CDrawRes::PEN_CELL_THIN);		//blue
	pOldPen = m_MemDC.SelectObject(pPen[0]);

	CBrush *pBrush[3], *pOldBrh;
	pBrush[0] = g_DataInfo.GetBrush(CDrawRes::BRS_FRAME);		//red
	pBrush[1] = g_DataInfo.GetBrush(CDrawRes::BRS_CELL);		//blue
	pBrush[2] = g_DataInfo.GetBrush(CDrawRes::BRS_BKGROUND);	//bkground
	pOldBrh = m_MemDC.SelectObject(pBrush[0]);
	
	//1. draw link line
	if (m_bShowLinkLine)
	{
		long a;

		//clear all can show flag.
		LTDC_MALINE *pLineTemp = m_pDWLine;
		for (a = 0; a < m_lDWLineNum; a++, pLineTemp++)
		{
			pLineTemp->bCanShow = FALSE;
			if (IsLineCanShow(pLineTemp))
			{
				pLineTemp->bCanShow = TRUE;
				FillLinePoint(pLineTemp);
			}
		}
		
		//create can show range
		CRgn rgnShow;		
		rgnShow.CreateRectRgn(m_rtPanel[1].left, m_rtPanel[1].top, m_rtPanel[1].right, m_rtPanel[1].bottom);
		m_MemDC.SelectClipRgn(&rgnShow);

		//show all line
		pLineTemp = m_pDWLine;
		for (a = 0; a < m_lDWLineNum; a++, pLineTemp++)
		{
			if (pLineTemp->bCanShow )
			{
				if (pLineTemp->bIsWillLine)
					m_MemDC.SelectObject(pPen[0]);
				else
				{
					if (pLineTemp->bIsHotLine)
						m_MemDC.SelectObject(pPen[0]);
					else
						m_MemDC.SelectObject(pPen[1]);
				}

				m_MemDC.MoveTo(pLineTemp->ptStart);
				m_MemDC.LineTo(pLineTemp->ptEnd);
			}
		}

		m_MemDC.SelectClipRgn(NULL);
	}

	//2. draw same range.
	if (m_bShowSameRgn)
	{
		POSITION pos = m_lstRgnData.GetHeadPosition();
		while (pos)
		{
			LTDC_MARGN *pRgn = m_lstRgnData.GetNext(pos);

			CPen pen(PS_SOLID, 1, pRgn->clrRgn);
			pOldPen = m_MemDC.SelectObject(&pen);

			long a, b, lMaxShow;
			CRect rtRgn;

			CRgn rgnAll;
			rgnAll.CreateRectRgn(0, 0, 0, 0);

			lMaxShow = m_lDataNum - m_lStartPos + 1;
			if (lMaxShow > m_lOnePageNum)
				lMaxShow = m_lOnePageNum;

			//1. draw outline, create clip rgn
			for (a = 0; a < lMaxShow; a++)
			{
				for (b = 0; b < m_lSelFrom; b++)
				{
					long lPos = (m_lStartPos + a) * m_lSelFrom + b;
					long *plData = pRgn->plData + lPos;

					if (*plData > 0)
					{//in rgn.
						rtRgn.left = m_rtPanel[1].left + b * m_lCellWidth;
						rtRgn.top = m_rtPanel[1].top + a * m_lCellWidth;
						rtRgn.right = rtRgn.left + m_lCellWidth;
						rtRgn.bottom = rtRgn.top + m_lCellWidth;

						CRgn rgnTmp;
						rgnTmp.CreateRectRgnIndirect(rtRgn);
						rgnAll.CombineRgn(&rgnAll, &rgnTmp, RGN_OR);

						//draw line.
						rtRgn.DeflateRect(1, 1);

						m_MemDC.MoveTo(rtRgn.left, rtRgn.top);
						m_MemDC.LineTo(rtRgn.right, rtRgn.top);
						m_MemDC.LineTo(rtRgn.right, rtRgn.bottom);
						m_MemDC.LineTo(rtRgn.left, rtRgn.bottom);
						m_MemDC.LineTo(rtRgn.left, rtRgn.top);
					}
				}
			}

			//2. select clip rgn.
			m_MemDC.SelectClipRgn(&rgnAll);

			CBrush brsRgn;
			brsRgn.CreateHatchBrush(HS_DIAGCROSS, pRgn->clrRgn);
			m_MemDC.FillRect(m_rtPanel[0], &brsRgn);

			m_MemDC.SelectClipRgn(NULL);
			m_MemDC.SelectObject(pOldPen);
		}
	}

	//3. draw cell
	{
		long a, b, lMaxShow, lX, lY;
		BOOL bShowWill = FALSE;
		CRect rtText;
		CString strTmp;

		//addition '1' in order to show guess line.
		lMaxShow = m_lDataNum + 1 - m_lStartPos;
		if (lMaxShow <= m_lOnePageNum)
			bShowWill = TRUE;
		else
			lMaxShow = m_lOnePageNum;

		//show every period's number.
		LTDC_MADATA *pTemp = m_pDWData + m_lStartPos;	
		for (a = 0; a < lMaxShow; a++, pTemp++)
		{
			lX = m_rtPanel[0].left;
			lY = m_rtPanel[0].top + (LTDC_DWTITLE_HEIGHT + a) * m_lCellWidth;

			//1. draw period number.
			rtText.left = lX;
			rtText.top = lY;
			rtText.right = rtText.left + LTDC_DWPERIOD_WIDTH * m_lCellWidth;
			rtText.bottom = rtText.top + m_lCellWidth;

			long lColorIndex = 1;
			long lFontIndex = 4;
			if (bShowWill && a == lMaxShow - 1)
			{
				lColorIndex = 0;
				lFontIndex = 1;
				strTmp.Format("%s", "下一期");
			}
			else
				strTmp.Format("%03d", pTemp->lPeriod);

			ShowText(strTmp, rtText, clrColor[lColorIndex], 5, lFontIndex);
			
			//exist if it is the show will line
			if (bShowWill && a == lMaxShow - 1)
				break;

			//2. draw cell.
			lColorIndex = 0;
			lX += LTDC_DWPERIOD_WIDTH * m_lCellWidth;
			for (b = 0; b < m_lSelFor; b++)
			{
				rtText.left = lX + (pTemp->eachData[b].lNo - 1) * m_lCellWidth;
				rtText.top = lY;
				rtText.right = rtText.left + m_lCellWidth;
				rtText.bottom = rtText.top + m_lCellWidth;
				
				if (pTemp->eachData[b].bIsSpec)
				{
					if (pTemp->eachData[b].bIsLink)
					{
						m_MemDC.SelectObject(pPen[0]);
						lColorIndex = 0;
					}
					else
					{
						m_MemDC.SelectObject(pPen[1]);
						lColorIndex = 1;
					}

					m_MemDC.SelectObject(pBrush[2]);
				}
				else
				{
					if (pTemp->eachData[b].bIsLink)
					{
						m_MemDC.SelectObject(pPen[0]);
						m_MemDC.SelectObject(pBrush[0]);
					}
					else
					{
						m_MemDC.SelectObject(pPen[1]);
						m_MemDC.SelectObject(pBrush[1]);
					}

					lColorIndex = 2;
				}

				rtText.DeflateRect(2, 2);
				m_MemDC.Ellipse(rtText);

				strTmp.Format("%02d", pTemp->eachData[b].lNo);
				ShowText(strTmp, rtText, clrColor[lColorIndex], 5, lFontIndex);
			}
		}
	}

	m_MemDC.SelectObject(pOldPen);
	m_MemDC.SelectObject(pOldBrh);
}

BOOL CMainAnalyse::IsLineCanShow(LTDC_MALINE *pLine)
{
	if ((pLine->ptStartPos.y >= m_lStartPos && pLine->ptStartPos.y < m_lStartPos + m_lOnePageNum) ||
		(pLine->ptEndPos.y >= m_lStartPos && pLine->ptEndPos.y <= m_lStartPos + m_lOnePageNum))
	{
		return TRUE;
	}
		
	return FALSE;
}

void CMainAnalyse::FillLinePoint(LTDC_MALINE * pLine)
{
	long x, y;
	LTDC_MACELL *pCell = NULL;

	//statr point
	x = pLine->ptStartPos.x;
	y = pLine->ptStartPos.y - m_lStartPos + 6;

	pCell = m_pDWCell + y * m_lSelFrom + x;
	pLine->ptStart = pCell->ptCenter;


	//end point
	x = pLine->ptEndPos.x;
	y = pLine->ptEndPos.y - m_lStartPos + 6;

	pCell = m_pDWCell + y * m_lSelFrom + x;
	pLine->ptEnd = pCell->ptCenter;
}

void CMainAnalyse::DrawCursor()
{
	CPen pen, *pOldPen;
	pen.CreatePen(PS_DOT, 1, g_DataInfo.GetColor(CDrawRes::CLR_YELLOW));
	pOldPen = m_MemDC.SelectObject(&pen);

	CBrush brush, *pOldBrush;
	HGDIOBJ hHollowBrh = ::GetStockObject(HOLLOW_BRUSH);
	brush.Attach(hHollowBrh);
	pOldBrush = m_MemDC.SelectObject(&brush);

	CRect rt;
	rt.left = m_rtPanel[0].left;
	rt.right = m_rtPanel[1].right;
	rt.top = m_rtPanel[1].top + m_lCellWidth * (m_lCurrPos - m_lStartPos);
	rt.bottom = rt.top + m_lCellWidth;

	//m_MemDC.Rectangle(rt);
	m_MemDC.RoundRect(rt, CPoint(LTDC_CORNER_VALUE / 2, LTDC_CORNER_VALUE / 2));

	brush.Detach();
	m_MemDC.SelectObject(pOldBrush);
	m_MemDC.SelectObject(pOldPen);

	//draw value
	DrawValue();
}

void CMainAnalyse::DrawValue()
{
	if (m_lCurrPos < m_lDataNum)
	{
		COLORREF clrUse = g_DataInfo.GetColor(CDrawRes::CLR_FRAME);
		CString strText, strTmp;

		LT_DATA_INFO *pDataInfo = m_pDataInfo + m_lCurrPos;
		if (m_bHaveSpec)
		{
			for (long a = 0; a < m_lSelFor - 1; a++)
			{
				if (a == m_lSelFor - 2)
					strTmp.Format("%d", pDataInfo->lData[a]);
				else
					strTmp.Format("%d ", pDataInfo->lData[a]);

				strText += strTmp;
			}
			strTmp = strText;

			long lSpecialNo = pDataInfo->lData[m_lSelFor - 1];
			strText.Format("  总第[%03d]期  基本号[%s] 特别号[%d]", 
							m_lCurrPos + 1, strTmp, lSpecialNo);
		}
		else
		{
			for (long a = 0; a < m_lSelFor; a++)
			{
				if (a == m_lSelFor - 1)
					strTmp.Format("%d", pDataInfo->lData[a]);
				else
					strTmp.Format("%d ", pDataInfo->lData[a]);

				strText += strTmp;
			}
			strTmp = strText;

			strText.Format("  总第[%03d]期  号码[%s]", m_lCurrPos + 1, strTmp);
		}

		CRect rtText(m_rtPanel[5]);
		rtText.left += 10;
		rtText.right -= 10;
		rtText.bottom -= 2;
		
		ShowText(strText, rtText, clrUse, 7, 1);
	}
}

void CMainAnalyse::GetDWRgnArray()
{
	LTDC_SAMERGN_INFO *pSameRgn = GetCurrSelRgnTmpl();
	if (pSameRgn)
	{
		//1. make real templete.
		GetDWRgnTemplet(pSameRgn);

		//2. get rgn data
		GetDWRgnMap();
	}
}

LTDC_SAMERGN_INFO *CMainAnalyse::GetCurrSelRgnTmpl()
{
	LTDC_SAMERGN_INFO *pRet = NULL;

	if (m_lCurrSameRgn >= 0 && m_lCurrSameRgn < (long)m_lstSameRgn.GetCount())
	{
		long lCnt = 0;
		POSITION pos = m_lstSameRgn.GetHeadPosition();
		while (pos)
		{
			LTDC_SAMERGN_INFO *pTmp = m_lstSameRgn.GetNext(pos);

			if (lCnt == m_lCurrSameRgn)
			{
				pRet = pTmp;
				break;
			}

			lCnt++;
		}
	}

	return pRet;
}

void CMainAnalyse::GetDWRgnTemplet(LTDC_SAMERGN_INFO *pSameRgn)
{
	BOOL bTemp[LTDC_DWRGNGRID_ROW][LTDC_DWRGNGRID_COL] = { FALSE };

	long a, b;

	//1. fill the map	
	for (a = 0; a < LTDC_DWRGNGRID_ROW; a++)
	{
		for (b = 0; b < LTDC_DWRGNGRID_COL; b++)
		{
			if (pSameRgn->lRgnMap[a][b] == 1)
				bTemp[a][b] = TRUE;
		}
	}

	//2. calc the map pos
	long lStartRow, lEndRow, lStartCol, lEndCol;
	lStartRow = LTDC_DWRGNGRID_ROW;
	lStartCol = LTDC_DWRGNGRID_COL;
	lEndRow = lEndCol = 0;

	for (a = 0; a < LTDC_DWRGNGRID_ROW; a++)
	{
		for (b = 0; b < LTDC_DWRGNGRID_COL; b++)
		{
			if (bTemp[a][b])
			{
				if (a < lStartRow)
					lStartRow = a;

				if (a > lEndRow)
					lEndRow = a;

				if (b < lStartCol)
					lStartCol = b;

				if (b > lEndCol)
					lEndCol = b;
			}
		}
	}

	//3. clip map.
	LTDC_DWRGN_TEMPL rgnTemplet;
	memset(&rgnTemplet, 0, sizeof(LTDC_DWRGN_TEMPL));

	rgnTemplet.lMapRow = lEndRow - lStartRow + 1;
	rgnTemplet.lMapCol = lEndCol - lStartCol + 1;
	
	for (a = 0; a < LTDC_DWRGNGRID_ROW; a++)
	{
		for (b = 0; b < LTDC_DWRGNGRID_COL; b++)
		{
			if (bTemp[a][b])
				rgnTemplet.bFillArray[a - lStartRow][b - lStartCol] = TRUE;
		}
	}

	//4. get rgn templet.
	{
		//free mem first.
		FreeRgnDataMem();
		
		//get all rotated templet
		for (a = 0; a < 6; a++)
		{
			BOOL bSel = FALSE;
			if (a == 0 || 
				(a == 1 && pSameRgn->b90) || 				
				(a == 2 && pSameRgn->b180) ||
				(a == 3 && pSameRgn->b270) ||
				(a == 4 && pSameRgn->bHMirror) ||
				(a == 5 && pSameRgn->bVMirror))
			{
				bSel = TRUE;
			}

			if (bSel)
			{
				LTDC_MARGN *pRgn = new LTDC_MARGN;
				memset(pRgn, 0, sizeof(LTDC_MARGN));
				m_lstRgnData.AddTail(pRgn);
								
				GetDWRgnTemplRotateRet(rgnTemplet, a, pRgn->rgnTemplet);
			}
		}		
	}
}

void CMainAnalyse::GetDWRgnTemplRotateRet(LTDC_DWRGN_TEMPL &rgnInput, 
										  long lType, LTDC_DWRGN_TEMPL &rgnRet)
{
	if (lType < 4)
	{//roate.
		LTDC_DWRGN_TEMPL tmpTempl1, tmpTempl2;
		memcpy(&tmpTempl1, &rgnInput, sizeof(LTDC_DWRGN_TEMPL));

		for (long a = 0; a < lType; a++)
		{
			GetDWRgnTemplRotate90Degree(tmpTempl1, tmpTempl2);
			memcpy(&tmpTempl1, &tmpTempl2, sizeof(LTDC_DWRGN_TEMPL));
		}

		memcpy(&rgnRet, &tmpTempl1, sizeof(LTDC_DWRGN_TEMPL));
	}
	else if (lType == 4)
	{//h mirror
		memset(&rgnRet, 0, sizeof(LTDC_DWRGN_TEMPL));

		for (long a = 0; a < rgnInput.lMapRow; a++)
		{
			for (long b = 0; b < rgnInput.lMapCol; b++)
			{
				rgnRet.bFillArray[a][b] = rgnInput.bFillArray[rgnInput.lMapRow - a - 1][b];
			}
		}

		rgnRet.lMapRow = rgnInput.lMapRow;
		rgnRet.lMapCol = rgnInput.lMapCol;
	}
	else if (lType == 5)
	{//v mirror
		memset(&rgnRet, 0, sizeof(LTDC_DWRGN_TEMPL));

		for (long a = 0; a < rgnInput.lMapRow; a++)
		{
			for (long b = 0; b < rgnInput.lMapCol; b++)
			{
				rgnRet.bFillArray[a][b] = rgnInput.bFillArray[a][rgnInput.lMapCol - b - 1];
			}
		}

		rgnRet.lMapRow = rgnInput.lMapRow;
		rgnRet.lMapCol = rgnInput.lMapCol;
	}
}

void CMainAnalyse::GetDWRgnTemplRotate90Degree(LTDC_DWRGN_TEMPL &rgnInput, 
											   LTDC_DWRGN_TEMPL &rgnRet)
{
	long a, b;

	memset(&rgnRet, 0, sizeof(LTDC_DWRGN_TEMPL));
	for (a = 0; a < rgnInput.lMapCol; a++)
	{
		for (b = 0; b < rgnInput.lMapRow; b++)
		{
			rgnRet.bFillArray[a][b] = rgnInput.bFillArray[b][rgnInput.lMapCol - a - 1];
		}
	}

	rgnRet.lMapRow = rgnInput.lMapCol;
	rgnRet.lMapCol = rgnInput.lMapRow; 
}

void CMainAnalyse::GetDWRgnMap()
{
	static COLORREF clrRgn[6] = 
	{
		RGB(255, 0, 0), RGB(255, 0, 0), RGB(255, 0, 0), 
		RGB(255, 0, 0), RGB(255, 0, 0), RGB(255, 0, 0), 	
	};

	long lCnt = 0;
	POSITION pos = m_lstRgnData.GetHeadPosition();
	while (pos)
	{
		LTDC_MARGN *pRgn = m_lstRgnData.GetNext(pos);

		//1. alloc mem space
		pRgn->lDataNum = (m_lDataNum + 1) * m_lSelFrom;
		pRgn->plData = new long[pRgn->lDataNum];
		memset(pRgn->plData, 0, sizeof(long) * pRgn->lDataNum);
		
		//2. get a clip area to check.
		long a, b;
		BOOL bRgnVal[LTDC_DWRGNGRID_ROW][LTDC_DWRGNGRID_COL] = { FALSE };
		for (a = 0; a < m_lDataNum; a++)
		{
			for (b = 0; b < m_lSelFrom; b++)
			{
				if (GetDWRgnEachArea(a, b, &pRgn->rgnTemplet, bRgnVal) 
					&& GetDWRgnCompTempet(&pRgn->rgnTemplet, bRgnVal))
				{
					GetDWRgnPutToRet(a, b, pRgn);
				}
			}
		}

		pRgn->clrRgn = clrRgn[lCnt];

		lCnt++;
		if (lCnt >= 6) lCnt = 0;
	}
}

BOOL CMainAnalyse::GetDWRgnEachArea(long lRow, long lCol, 
								LTDC_DWRGN_TEMPL *pTemplet,
								BOOL bRgnVal[LTDC_DWRGNGRID_ROW][LTDC_DWRGNGRID_COL])
{
	BOOL bRet = FALSE;

	//1. judge if can get
	long lRemainRowNum = m_lDataNum - lRow;
	long lRemainColNum = m_lSelFrom - lCol;

	BOOL bHaveGuessLine = FALSE;
	if (pTemplet->lMapRow <= lRemainRowNum && pTemplet->lMapCol <= lRemainColNum)
		bRet = TRUE;
	else
	{//if extend one line, then this will be guess line.
		if (pTemplet->lMapRow > 1 && lRemainRowNum == pTemplet->lMapRow - 1 &&
			pTemplet->lMapCol <= lRemainColNum)
		{//templete must be multi-line.
			bRet = TRUE;

			bHaveGuessLine = TRUE;
		}
	}

	//2. loop for each area
	if (bRet)
	{
		long a, b, c;

		LT_DATA_INFO *pDataInfoTmp = NULL;
		for (a = 0; a < pTemplet->lMapRow; a++)
		{
			pDataInfoTmp = m_pDataInfo + lRow + a;
			for (b = 0; b < pTemplet->lMapCol; b++)
			{
				BOOL bFill = FALSE;

				if (bHaveGuessLine && a == pTemplet->lMapRow - 1)
				{//if guess line, use template's value as real value.
					if (pTemplet->bFillArray[a][b] > 0)
						bFill = TRUE;
				}
				else
				{
					for (c = 0; c < m_lSelFor; c++)
					{
						if (pDataInfoTmp->lData[c] == lCol + b + 1)
						{
							bFill = TRUE;
							break;
						}
					}
				}

				bRgnVal[a][b] = bFill;
			}
		}
	}

	return bRet;
}

BOOL CMainAnalyse::GetDWRgnCompTempet(LTDC_DWRGN_TEMPL *pTemplet, 
									  BOOL bRgnVal[LTDC_DWRGNGRID_ROW][LTDC_DWRGNGRID_COL])
{
	BOOL bRet = TRUE;

	long a, b;

	for (a = 0; a < pTemplet->lMapRow; a++)
	{
		for (b = 0; b < pTemplet->lMapCol; b++)
		{
			if (bRgnVal[a][b] != pTemplet->bFillArray[a][b])
			{
				bRet = FALSE;
				break;
			}
		}
	}

	return bRet;
}

void CMainAnalyse::GetDWRgnPutToRet(long lRow, long lCol, LTDC_MARGN *pRgn)
{
	BOOL bCanPut = TRUE;

	long a, b;
	//1. try to put	
	for (a = 0; a < pRgn->rgnTemplet.lMapRow; a++)
	{
		for (b = 0; b < pRgn->rgnTemplet.lMapCol; b++)
		{
			if (pRgn->rgnTemplet.bFillArray[a][b])
			{
				long lPos = (lRow + a) * m_lSelFrom + lCol + b;
				long *plDataTmp = pRgn->plData + lPos;

				if (*plDataTmp > 0)
				{
					bCanPut = FALSE;
					break;
				}
			}
		}
	}

	//2. really put
	if (bCanPut)
	{
		for (a = 0; a < pRgn->rgnTemplet.lMapRow; a++)
		{
			for (b = 0; b < pRgn->rgnTemplet.lMapCol; b++)
			{
				if (pRgn->rgnTemplet.bFillArray[a][b])
				{
					long lPos = (lRow + a) * m_lSelFrom + lCol + b;
					long *plDataTmp = pRgn->plData + lPos;

					*plDataTmp = 1;
				}
			}
		}
	}
}

CMenu *CMainAnalyse::GetPopupMenu(CMenu *pSrcMenu, UINT nStartMenuID)
{
	CMenu *pRetMeun = pSrcMenu;

	if (m_bShowLinkLine)
		pRetMeun->AppendMenu(MF_STRING | MF_CHECKED, nStartMenuID, "连接线");
	else
		pRetMeun->AppendMenu(MF_STRING | MF_UNCHECKED, nStartMenuID, "连接线");
	
	nStartMenuID++;

	CMenu menu;
	menu.CreatePopupMenu();	

	menu.AppendMenu(MF_STRING, nStartMenuID, "设置");
	menu.AppendMenu(MF_SEPARATOR);
	nStartMenuID++;

	POSITION pos = m_lstSameRgn.GetHeadPosition();
	while (pos)
	{
		LTDC_SAMERGN_INFO *pRgn = m_lstSameRgn.GetNext(pos);

		menu.AppendMenu(MF_STRING, nStartMenuID, pRgn->szName);	
		nStartMenuID++;
	}
	
	pRetMeun->AppendMenu(MF_STRING | MF_POPUP, (UINT)menu.GetSafeHmenu(), "同区域");

	return pRetMeun;
}

void CMainAnalyse::OnCommand(UINT uCmdID)
{
	if (uCmdID == 0)
	{
		m_bShowLinkLine ^= 1;
	}
	else if (uCmdID == 1)
	{
		CRgnTmplManageDlg dlg;
		dlg.SetSameRgnList(&m_lstSameRgn, m_lCurrSameRgn);
		if (dlg.DoModal() == IDOK)
		{
			m_lCurrSameRgn = dlg.m_lSelIndex;
		}
	}
	else
	{
		m_lCurrSameRgn = uCmdID - 2;
	}
}

/////////////////////////////////////////////////////////////////////////////////
////CZoneAnalyse
CZoneAnalyse::CZoneAnalyse()
{
	strcpy(m_szName, "区间图");

	m_lCellHeight = 0;
	m_lHZoneNum = m_lVZoneNum = 0;
	for (long a = 0; a < 8; a++)
		m_rtPanel[a].SetRectEmpty();

	memset(&m_ZoneInfo, 0, sizeof(LTDC_ZONE_INFO));
}

CZoneAnalyse::~CZoneAnalyse()
{
	if (m_ZoneInfo.pZoneData)
		delete m_ZoneInfo.pZoneData;	
}

void CZoneAnalyse::SetData(LT_DATA_INFO *pData, long lDataNum, 
					long lSelFrom, long lSelFor, BOOL bHaveSpec)
{
	IAnalyseDrawCtrl::SetData(pData, lDataNum, lSelFrom, lSelFor, bHaveSpec);

	InitDrawData();
}

void CZoneAnalyse::SetParam(RECT *pRect)
{
	IAnalyseDrawCtrl::SetParam(pRect);

	//get all show rect.
	CalcPanelsRect();

	//init all draw data.
	InitDrawData();
}

void CZoneAnalyse::CalcPanelsRect()
{	
	if (IsHaveDrawData())
	{
		long lWidth, lHeight, lXCellNum, lYCellNum, xdt, ydt;
		lWidth = lHeight = lXCellNum = lYCellNum = xdt = ydt = 0;

		g_DataInfo.GetZoneInfo(m_lSelFrom, m_lHZoneNum, m_lVZoneNum);

		lWidth = m_rtShow.Width();		
		lHeight = m_rtShow.Height() - 20;

		//calcu width and height of cell.
		lXCellNum = LTDC_ZONEPERIOD_WIDTH + m_lHZoneNum + 2 + 1 + m_lVZoneNum + 2;
		m_lCellWidth = lWidth / lXCellNum;

		m_lCellHeight = lWidth / (m_lSelFrom + 2 * LTDC_ZONEPERIOD_WIDTH);
		lYCellNum = lHeight / m_lCellHeight;
		
		//set if can scroll & on page number & start pos
		m_lOnePageNum = lYCellNum - 3;
		m_lScrollRange = m_lDataNum - m_lOnePageNum;
		if (m_lScrollRange < 0)
			m_lScrollRange = 0;

		m_lStartPos = m_lScrollRange;
		m_lCurrPos = m_lDataNum - 1;
		m_lTotalNum = m_lDataNum;

		//adjust work rect
		xdt = (lWidth - m_lCellWidth * lXCellNum) / 2;
		ydt = (lHeight - lYCellNum * m_lCellHeight) / 2;
		
		m_rtPanel[0].SetRect(m_rtShow.left + xdt, m_rtShow.top + 20 + ydt,
							m_rtShow.right - xdt, m_rtShow.bottom - ydt);
		m_rtPanel[1].SetRect(m_rtPanel[0].left + m_lCellWidth * LTDC_ZONEPERIOD_WIDTH, 
							m_rtPanel[0].top + m_lCellHeight * LTDC_ZONETITLE_HEIGHT,
							m_rtPanel[0].right, 
							m_rtPanel[0].bottom - m_lCellHeight);

		//title
		m_rtPanel[2].SetRect(m_rtShow.left, m_rtShow.top, m_rtShow.right, m_rtShow.top + 18);
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

void CZoneAnalyse::InitDrawData()
{	
	if (IsHaveDrawData())
		GetDrawData();
}

void CZoneAnalyse::GetDrawData()
{
	long a, b;

	//1. get horizontal & vertical zone infomation.
	if (m_ZoneInfo.pZoneData)
		delete m_ZoneInfo.pZoneData;
	m_ZoneInfo.lZoneNum = m_lDataNum;
	m_ZoneInfo.pZoneData = new LTDC_ZONE_DATA[m_lDataNum];
	memset(m_ZoneInfo.pZoneData, 0, sizeof(LTDC_ZONE_DATA) * m_lDataNum);
	
	LTDC_ZONE_DATA *pZoneDataTemp = m_ZoneInfo.pZoneData;
	LT_DATA_INFO *pDataTemp = m_pDataInfo;
	for (a = 0; a < m_lDataNum; a++, pDataTemp++, pZoneDataTemp++)
	{
		pZoneDataTemp->lPeriod = pDataTemp->lPeriodID;
		g_StatHelper.GetDataZoneInfo(pDataTemp->lData, pZoneDataTemp->lData);
	}

	//2. get stat range val
	{
		LTSH_NORMALSTAT_RET ns;

		long lTotalNum = m_lHZoneNum + m_lVZoneNum + 4;
		for (a = 0; a < lTotalNum; a++)
		{
			long *plData = new long[m_ZoneInfo.lZoneNum];
			memset(plData, 0, sizeof(long) * m_ZoneInfo.lZoneNum);

			memset(&ns, 0, sizeof(LTSH_NORMALSTAT_RET));
			ns.plData = plData;
			ns.lNum = m_ZoneInfo.lZoneNum;

			for (b = 0; b < m_ZoneInfo.lZoneNum; b++)
			{
				plData[b] = m_ZoneInfo.pZoneData[b].lData[a];

				if (b == m_lHZoneNum - 1)
				{//horizontal empty zone max number.
					ns.lAllowMix = 0;
					ns.lAllowMax = m_lHZoneNum - 1;
				}
				else if (b == m_lHZoneNum + 2 + m_lVZoneNum - 1)
				{//vertical empty zone max number.
					ns.lAllowMix = 0;
					ns.lAllowMax = m_lVZoneNum - 1;
				}
				else if (b == m_lHZoneNum || b == m_lHZoneNum + 2 + m_lVZoneNum)
				{
					ns.lAllowMix = 1;
					ns.lAllowMax = m_lSelFor;
				}
				else
				{
					ns.lAllowMix = 1;
					ns.lAllowMax = m_lSelFor;
				}

				g_StatHelper.NormalStat(&ns);

				m_ZoneInfo.fAveVal[a] = ns.fAve;

				m_ZoneInfo.lRetVal[a][0] = ns.lRetDown;
				m_ZoneInfo.lRetVal[a][1] = ns.lRetUp;
			}

			delete plData;
		}
	}

	//3. set ret to global.
	LTSH_ZONESTAT_RET zoneRet;
	memset(&zoneRet, 0, sizeof(LTSH_ZONESTAT_RET));
	
	zoneRet.lHEmptyZoneMin = m_ZoneInfo.lRetVal[m_lHZoneNum - 1][0];
	zoneRet.lHEmptyZoneMax = m_ZoneInfo.lRetVal[m_lHZoneNum - 1][1];

	zoneRet.lVEmptyZoneMin = m_ZoneInfo.lRetVal[m_lHZoneNum][0];
	zoneRet.lVEmptyZoneMax = m_ZoneInfo.lRetVal[m_lHZoneNum][1];

	zoneRet.lHZoneMaxOutMin = m_ZoneInfo.lRetVal[m_lHZoneNum + 2 + m_lVZoneNum - 1][0];
	zoneRet.lHZoneMaxOutMax = m_ZoneInfo.lRetVal[m_lHZoneNum + 2 + m_lVZoneNum - 1][1];

	zoneRet.lVZoneMaxOutMin = m_ZoneInfo.lRetVal[m_lHZoneNum + 2 + m_lVZoneNum][0];
	zoneRet.lVZoneMaxOutMax = m_ZoneInfo.lRetVal[m_lHZoneNum + 2 + m_lVZoneNum][1];

	g_StatHelper.SetZoneStatRet(&zoneRet);
}

BOOL CZoneAnalyse::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bRet = FALSE;

	if (m_rtPanel[1].PtInRect(point))
	{
		long lOldPos = m_lCurrPos;

		m_lCurrPos = (point.y - m_rtPanel[1].top) / m_lCellHeight;
		if (m_lCurrPos < 0)
			m_lCurrPos = 0;

		m_lCurrPos += m_lStartPos;

		if (m_lCurrPos > m_lTotalNum - 1)
			m_lCurrPos = m_lTotalNum - 1;
		
		if (m_lCurrPos != lOldPos)
			bRet = TRUE;
	}

	return bRet;
}

void CZoneAnalyse::Draw(HDC hDC, long lShowType)
{
	m_MemDC.Attach(hDC);

	if (DrawOutlook()) //draw frame/name/title-numbers
	{
		//draw content(link-line, same-rgn, cell)
		DrawContent();

		//draw cursor.
		DrawCursor();
	}

	m_MemDC.Detach();
}

BOOL CZoneAnalyse::DrawOutlook()
{
	COLORREF clrColor[3];
	clrColor[0] = g_DataInfo.GetColor(CDrawRes::CLR_FRAME);		//red
	clrColor[1] = g_DataInfo.GetColor(CDrawRes::CLR_CELL);		//blue
	clrColor[2] = g_DataInfo.GetColor(CDrawRes::CLR_BLACK);		//black

	CPen *pPen[3], *pOldPen;
	pPen[0] = g_DataInfo.GetPen(CDrawRes::PEN_FRAME_BOLD);		//frame
	pPen[1] = g_DataInfo.GetPen(CDrawRes::PEN_FRAME_THIN);		//long cell line
	pPen[2] = g_DataInfo.GetPen(CDrawRes::PEN_CELL_THIN);		//short cell line
	pOldPen = m_MemDC.SelectObject(pPen[0]);

	CBrush *pBrush[1], *pOldBrh;
	pBrush[0] = g_DataInfo.GetBrush(CDrawRes::BRS_BKGROUND);	//bkground
	pOldBrh = m_MemDC.SelectObject(pBrush[0]);

	//1. draw frame
	if (IsHaveDrawData())
		m_MemDC.RoundRect(m_rtPanel[0], CPoint(LTDC_CORNER_VALUE, LTDC_CORNER_VALUE));
	else
	{
		ShowText("没有数据！", m_rtShow, clrColor[0], 5, 0);
		return FALSE;
	}

	//2. draw cell frame
	{
		long a, lX, lY, lVLineNum, lHLineNum;
		POINT ptStart, ptEnd;

		//vertical line
		lVLineNum = m_lHZoneNum + 1 + m_lVZoneNum + 4;		
		lX = m_rtPanel[1].left;
		for (a = 0; a < lVLineNum; a++, lX += m_lCellWidth)
		{
			ptStart.x = ptEnd.x = lX;
			ptStart.y = m_rtPanel[0].top + 1;
			ptEnd.y = m_rtPanel[0].bottom - 1;

			if (a == 0 || a == m_lHZoneNum + 2 || a == m_lHZoneNum + 3)
				m_MemDC.SelectObject(pPen[1]);
			else
				m_MemDC.SelectObject(pPen[2]);				

			m_MemDC.MoveTo(ptStart);
			m_MemDC.LineTo(ptEnd);
		}

		//horizontal line
		lHLineNum = m_lOnePageNum + 2;
		lY = m_rtPanel[0].top + m_lCellHeight;
		for (a = 0; a < lHLineNum; a++, lY += m_lCellHeight)
		{
			ptStart.x = m_rtPanel[0].left + 1;
			if (a == 0)
				ptStart.x += LTDC_ZONEPERIOD_WIDTH * m_lCellWidth;

			ptEnd.x = m_rtPanel[0].right - 1;
			ptStart.y = ptEnd.y = lY;

			if (a == 1 || a == lHLineNum - 1)
				m_MemDC.SelectObject(pPen[1]);
			else
				m_MemDC.SelectObject(pPen[2]);

			m_MemDC.MoveTo(ptStart);
			m_MemDC.LineTo(ptEnd);
		}
	}

	//3. draw titles
	{
		//draw name
		ShowText(m_szName, m_rtPanel[4], clrColor[0], 9, 0);

		//draw val's bottom line
		m_MemDC.SelectObject(pPen[1]);
		m_MemDC.MoveTo(m_rtPanel[5].left + 5, m_rtPanel[5].bottom - 1);
		m_MemDC.LineTo(m_rtPanel[5].right - 15, m_rtPanel[5].bottom - 1);
	}

	//4. draw zone-name/period-number.
	{
		long a, lX, lTotalNum;
		CRect rtText;
		CString strText;

		lTotalNum = m_lHZoneNum + m_lVZoneNum + 6;
		lX = m_rtPanel[1].left;
		for (a = 0; a < lTotalNum; a++, lX += m_lCellWidth)
		{
			rtText.left = lX;
			rtText.top = m_rtPanel[0].top;
			rtText.right = lX + m_lCellWidth;
			rtText.bottom = m_rtPanel[0].top + m_lCellHeight;

			if (a < m_lHZoneNum)
			{	
				strText.Format("H%c区", 'A' + a);
				ShowText(strText, rtText, clrColor[2], 5, 1);

				rtText.top += m_lCellHeight;
				rtText.bottom = rtText.top + m_lCellHeight;
				strText.Format("%02d-%02d", a * m_lVZoneNum + 1, (a + 1) * m_lVZoneNum);
				ShowText(strText, rtText, clrColor[2], 5, 4);
			}
			else if (a == m_lHZoneNum)
				ShowText("空区", rtText, clrColor[0], 5, 1);
			else if (a == m_lHZoneNum + 1)
				ShowText("最大", rtText, clrColor[1], 5, 1);
			else if (a == m_lHZoneNum + 2)
			{
				;
			}
			else if (a < m_lHZoneNum + 2 + 1 + m_lVZoneNum)
			{
				strText.Format("V%c区", 'A' + a - m_lHZoneNum - 2 - 1);
				ShowText(strText, rtText, clrColor[2], 5, 1);

				rtText.top += m_lCellHeight;
				rtText.bottom = rtText.top + m_lCellHeight;
				if (a - m_lHZoneNum - 2 < m_lVZoneNum)
					strText.Format("除%d余%d", m_lVZoneNum, a - m_lHZoneNum - 2);
				else
					strText.Format("除%d余%d", m_lVZoneNum, 0);
				ShowText(strText, rtText, clrColor[2], 5, 1);
			}
			else if (a == m_lHZoneNum + 2 + 1 + m_lVZoneNum)
				ShowText("空区", rtText, clrColor[0], 5, 1);
			else if (a == m_lHZoneNum + 2 + 1 + m_lVZoneNum + 1)
				ShowText("最大", rtText, clrColor[1], 5, 1);
		}
	}

	m_MemDC.SelectObject(pOldPen);
	m_MemDC.SelectObject(pOldBrh);

	return TRUE;
}

void CZoneAnalyse::DrawContent()
{
	COLORREF clrColor[3];
	clrColor[0] = g_DataInfo.GetColor(CDrawRes::CLR_FRAME);		//red
	clrColor[1] = g_DataInfo.GetColor(CDrawRes::CLR_CELL);		//blue
	clrColor[2] = g_DataInfo.GetColor(CDrawRes::CLR_BLACK);		//black

	long a, b, lX, lY, lTotalNum, lShowNum;
	CRect rtText;
	CString strText;

	lShowNum = m_lDataNum - m_lStartPos;
	if (lShowNum > m_lOnePageNum)
		lShowNum = m_lOnePageNum;

	lTotalNum = m_lHZoneNum + m_lVZoneNum + 5;

	LTDC_ZONE_DATA *pZoneData = m_ZoneInfo.pZoneData + m_lStartPos;
	lY = m_rtPanel[1].top;
	for (a = 0; a < lShowNum + 1; a++, pZoneData++, lY += m_lCellHeight)
	{
		//draw period
		rtText.left = m_rtPanel[0].left;
		rtText.top = lY;
		rtText.right = m_rtPanel[1].left;
		rtText.bottom = rtText.top + m_lCellHeight;

		if (a != lShowNum)
		{
			strText.Format("%03d", pZoneData->lPeriod);
			ShowText(strText, rtText, clrColor[1], 5, 4);
		}
		else
			ShowText("平均值", rtText, clrColor[0], 5, 1);

		//draw content
		lX = m_rtPanel[1].left;
		long *plVal = pZoneData->lData;
		float *pfVal = m_ZoneInfo.fAveVal;
		for (b = 0; b < lTotalNum; b++, lX += m_lCellWidth)
		{
			rtText.left = lX;
			rtText.top = lY;
			rtText.right = rtText.left + m_lCellWidth;
			rtText.bottom = rtText.top + m_lCellHeight;

			if (b != m_lHZoneNum + 2)
			{
				if (a != lShowNum)
					strText.Format("%d", *plVal++);
				else
					strText.Format("%.2f", *pfVal++);

				long lColIndex = 2;
				if (b == m_lHZoneNum || b == m_lHZoneNum + 2 + 1 + m_lVZoneNum)
					lColIndex = 0;
				else if (b == m_lHZoneNum + 1 || b == m_lHZoneNum + 2 + 1 + m_lVZoneNum + 1)
					lColIndex = 1;

				ShowText(strText, rtText, clrColor[lColIndex], 5, 4);
			}			
		}
	}
}

void CZoneAnalyse::DrawCursor()
{
	CPen pen, *pOldPen;
	pen.CreatePen(PS_DOT, 1, g_DataInfo.GetColor(CDrawRes::CLR_YELLOW));
	pOldPen = m_MemDC.SelectObject(&pen);

	CBrush brush, *pOldBrush;
	HGDIOBJ hHollowBrh = ::GetStockObject(HOLLOW_BRUSH);
	brush.Attach(hHollowBrh);
	pOldBrush = m_MemDC.SelectObject(&brush);

	CRect rt;
	rt.left = m_rtPanel[0].left;
	rt.right = m_rtPanel[1].right;
	rt.top = m_rtPanel[1].top + m_lCellHeight * (m_lCurrPos - m_lStartPos);
	rt.bottom = rt.top + m_lCellHeight;

	//m_MemDC.Rectangle(rt);
	m_MemDC.RoundRect(rt, CPoint(LTDC_CORNER_VALUE / 2, LTDC_CORNER_VALUE / 2));

	brush.Detach();
	m_MemDC.SelectObject(pOldBrush);
	m_MemDC.SelectObject(pOldPen);

	//draw value
	DrawValue();
}

void CZoneAnalyse::DrawValue()
{
	COLORREF clrUse = g_DataInfo.GetColor(CDrawRes::CLR_FRAME);
	
	LTDC_ZONE_DATA *pDataInfo = m_ZoneInfo.pZoneData + m_lCurrPos;
	long lHZZNum, lVZZNum;
	lHZZNum = pDataInfo->lData[m_lHZoneNum];
	lVZZNum = pDataInfo->lData[m_lHZoneNum + 2 + m_lVZoneNum];

	CString strText;
	strText.Format("  总第[%03d]期  横向空区数[%d] 纵向空区数[%d]", 
					pDataInfo->lPeriod, lHZZNum, lVZZNum);

	CRect rtText(m_rtPanel[5]);
	rtText.left += 10;
	rtText.right -= 10;
	rtText.bottom -= 2;
	
	ShowText(strText, rtText, clrUse, 7, 1);
}

/////////////////////////////////////////////////////////////////////////////////
////IStatDrawCtrl
IStatDrawCtrl::IStatDrawCtrl()
{
	memset(&m_BaseInfo, 0, sizeof(DVCT_BASE_INFO));

	m_bIsColumn = FALSE;
	m_lMinCellWidth = 5;
	m_lMaxCellWidth = 50;	
}

IStatDrawCtrl::~IStatDrawCtrl()
{
	FreeDataInfo();
}

void IStatDrawCtrl::FreeDataInfo()
{
	for (long a = 0; a < m_DataInfo.lLineNum; a++)
	{
		delete (m_DataInfo.pLineData + a)->pUnitData;
	}

	delete m_DataInfo.pLineData;
	m_DataInfo.pLineData = NULL;
	m_DataInfo.lLineNum = 0;
}

BOOL IStatDrawCtrl::RWCfg(BOOL bIsRead)
{
	if (bIsRead)
	{
		strcpy(m_BaseInfo.szName, m_szName);		

		m_BaseInfo.clrBkGround = g_DataInfo.GetColor(CDrawRes::CLR_BKGROUND);
		m_BaseInfo.clrFrame = g_DataInfo.GetColor(CDrawRes::CLR_FRAME);
		m_BaseInfo.clrRuler = g_DataInfo.GetColor(CDrawRes::CLR_CELL);
		m_BaseInfo.clrGray = g_DataInfo.GetColor(CDrawRes::CLR_GRAY);

		m_BaseInfo.hFont[0] = (HFONT)(g_DataInfo.GetFont(CDrawRes::FONT_CH_MIDDLE))->GetSafeHandle();
		m_BaseInfo.hFont[1] = (HFONT)(g_DataInfo.GetFont(CDrawRes::FONT_CH_SMALL))->GetSafeHandle();
		m_BaseInfo.hFont[2] = (HFONT)(g_DataInfo.GetFont(CDrawRes::FONT_EN_SMALL))->GetSafeHandle();

		m_BaseInfo.lCornerRadius = LTDC_CORNER_VALUE;

		m_DvChart.SetBaseInfo(&m_BaseInfo);
	}

	return TRUE;
}

void IStatDrawCtrl::SetData(LT_DATA_INFO *pData, long lDataNum, 
					long lSelFrom, long lSelFor, BOOL bHaveSpec)
{
	IDrawCtrl::SetData(pData, lDataNum, lSelFrom, lSelFor, bHaveSpec);
	g_StatHelper.SetData(pData, lDataNum, lSelFrom, lSelFor, bHaveSpec);
}

void IStatDrawCtrl::SetParam(RECT *pRect)
{
	IDrawCtrl::SetParam(pRect);
	m_DvChart.SetParam(m_rtShow);
}

void IStatDrawCtrl::SetScrollPos(long lStartPos, long lCurrPos)
{
	m_DvChart.SetPosInfo(lStartPos, lCurrPos);
}

void IStatDrawCtrl::GetObjState(BOOL bBtns[6], long &lStartPos, long &lCurrPos,
				long &lOnePageNum, long &lScrollRange, long &lTotalNum)
{
	m_DvChart.GetDrawInfo(m_lStartPos, m_lCurrPos, m_lOnePageNum, m_lScrollRange, m_lTotalNum,
				m_lCellWidth, m_bIsColumn);

	lStartPos = m_lStartPos;
	lCurrPos = m_lCurrPos;
	lOnePageNum = m_lOnePageNum;
	lScrollRange = m_lScrollRange;
	lTotalNum = m_lTotalNum;

	if (m_lCurrPos > 0)
		bBtns[0] = TRUE;
	
	if (m_lCurrPos < m_lTotalNum - 1)
		bBtns[1] = TRUE;

	if (m_lCellWidth < m_lMaxCellWidth)
		bBtns[2] = TRUE;

	if (m_lCellWidth > m_lMinCellWidth)
		bBtns[3] = TRUE;

	bBtns[4] = bBtns[5] = FALSE;
}

BOOL IStatDrawCtrl::OnKeyDown(UINT nChar, BOOL bIsCtrlKeyDown)
{
	BOOL bRet = FALSE;

	long lNewPos = m_lCurrPos;
	long lNewWidth = m_lCellWidth;
	long lOldPageNum = m_lOnePageNum;
	BOOL bColumn = m_bIsColumn;

	switch (nChar)
	{
	case VK_LEFT:
		lNewPos -= bIsCtrlKeyDown ? m_lOnePageNum : 1;
		break;
	case VK_RIGHT:
		lNewPos += bIsCtrlKeyDown ? m_lOnePageNum : 1;
		break;
	case VK_UP:
		lNewWidth++;
		break;
	case VK_DOWN:
		lNewWidth--;
		break;
	case VK_HOME:
		lNewPos = m_lStartPos;
		break;
	case VK_END:
		lNewPos = m_lStartPos + m_lOnePageNum - 1;
		break;
	case VK_TAB:
		{
			if (bIsCtrlKeyDown)
				bColumn ^= 1;
			else
				return FALSE;
		}
		break;
	default:
		return FALSE;
		break;
	}

	if (lNewPos < 0)
		lNewPos = 0;
	else if (lNewPos > m_lTotalNum - 1)
		lNewPos = m_lTotalNum - 1;

	if (lNewWidth > m_lMaxCellWidth)
		lNewWidth = m_lMaxCellWidth;
	else if (lNewWidth < m_lMinCellWidth)
		lNewWidth = m_lMinCellWidth;	

	if (lNewPos != m_lCurrPos)
	{
		m_lCurrPos = lNewPos;

		long lOldStart = m_lStartPos;
		if (lNewPos < m_lStartPos)
			m_lStartPos = lNewPos;
		else if (lNewPos > m_lStartPos + m_lOnePageNum - 1)
			m_lStartPos = lNewPos - m_lOnePageNum + 1;

		m_DvChart.SetPosInfo(m_lStartPos, m_lCurrPos);
		bRet = TRUE;
	}
	
	if (lNewWidth != m_lCellWidth)
	{
		m_lOnePageNum = m_DvChart.SetCellWidth(lNewWidth);		
		if (lOldPageNum > m_lOnePageNum)
		{
			if (m_lCurrPos > m_lStartPos + m_lOnePageNum - 1)
			{
				m_lCurrPos = m_lStartPos + m_lOnePageNum - 1;
				m_DvChart.SetPosInfo(m_lStartPos, m_lCurrPos);
			}
		}

		bRet = TRUE;
	}
	
	if (bColumn != m_bIsColumn)
	{
		m_bIsColumn = bColumn;

		m_DvChart.SetIsColumn(m_bIsColumn);
		bRet = TRUE;
	}

	return bRet;
}

BOOL IStatDrawCtrl::OnLButtonDown(UINT nFlags, POINT point)
{
	return m_DvChart.OnLButtonDown(nFlags, point);
}

BOOL IStatDrawCtrl::OnMouseMove(UINT nFlags, POINT point)
{
	return m_DvChart.OnMouseMove(nFlags, point);
}

void IStatDrawCtrl::Draw(HDC hDC, long lShowType)
{
	m_DvChart.Draw(hDC, lShowType);
}

/////////////////////////////////////////////////////////////////////////////////
////CProbStat
CProbStat::CProbStat()
{
	strcpy(m_szName, "概率图");
}

CProbStat::~CProbStat()
{
	;
}

void CProbStat::SetData(LT_DATA_INFO *pData, long lDataNum, 
					long lSelFrom, long lSelFor, BOOL bHaveSpec)
{
	IStatDrawCtrl::SetData(pData, lDataNum, lSelFrom, lSelFor, bHaveSpec);

	FreeDataInfo();

	long lNum = 0;
	LTSH_ETCHPERIOD_RATE *m_pEtchPeriodRate = g_StatHelper.GetProbability(lNum);	
	if (m_pEtchPeriodRate && lNum > 0)
	{
		m_DataInfo.bIsInt = FALSE;
		m_DataInfo.bShowPercent = TRUE;
		m_DataInfo.bIsFixWidth = TRUE;
		m_DataInfo.bShowMultiData = FALSE;
		strcpy(m_DataInfo.szHUnitName, "号码");
		strcpy(m_DataInfo.szVUnitName, "概率");

		m_DataInfo.lLineNum = lNum;
		m_DataInfo.pLineData = new DVCT_LINE_DATA[m_DataInfo.lLineNum];
		memset(m_DataInfo.pLineData, 0, sizeof(DVCT_LINE_DATA) * m_DataInfo.lLineNum);
		DVCT_LINE_DATA *pLineDataTmp = m_DataInfo.pLineData;
		for (long a = 0; a < m_DataInfo.lLineNum; a++, pLineDataTmp++)
		{
			strcpy(pLineDataTmp->szName, "概率值");
			pLineDataTmp->clrLine = g_DataInfo.GetColor(CDrawRes::CLR_RED);

			pLineDataTmp->lUnitNum = m_lSelFrom;
			pLineDataTmp->pUnitData = new DVCT_UNIT_DATA[pLineDataTmp->lUnitNum];
			memset(pLineDataTmp->pUnitData, 0, sizeof(DVCT_UNIT_DATA) * pLineDataTmp->lUnitNum);
			DVCT_UNIT_DATA *pUnitDataTmp = pLineDataTmp->pUnitData;
			for (long b = 0; b < pLineDataTmp->lUnitNum; b++, pUnitDataTmp++)
			{
				sprintf(pUnitDataTmp->szMarkName, "%d", b + 1);
				sprintf(pUnitDataTmp->szPeriodName, "总第[%d]期 号码[%d]", 
					(m_pEtchPeriodRate + a)->lPeriod, b + 1);

				long lVal = ((m_pEtchPeriodRate + a)->lUnSortProb[b] / 100);
				pUnitDataTmp->fVal = (float)(lVal * 0.0001);
			}
		}

		m_DataInfo.lCurrDataIndex = m_DataInfo.lLineNum - 1;
		m_DvChart.SetData(&m_DataInfo);
	}
}

/////////////////////////////////////////////////////////////////////////////////
////CThreeNoStat
CThreeNoStat::CThreeNoStat()
{
	strcpy(m_szName, "热温冷号走势图");

	memset(&m_DataInfo, 0, sizeof(DVCT_DATA_INFO) * 3);
}

CThreeNoStat::~CThreeNoStat()
{
	;
}

void CThreeNoStat::SetData(LT_DATA_INFO *pData, long lDataNum, 
					long lSelFrom, long lSelFor, BOOL bHaveSpec)
{
	IStatDrawCtrl::SetData(pData, lDataNum, lSelFrom, lSelFor, bHaveSpec);

	FreeDataInfo();

	long lNum = 0;
	LTSH_ETCHPERIOD_RATE *m_pEtchPeriodRate = g_StatHelper.GetProbability(lNum);	
	if (m_pEtchPeriodRate && lNum > 0)
	{
		const char *pName[3] = { "热号", "温号", "冷号", };
		const COLORREF clrUse[3] = 
		{
			g_DataInfo.GetColor(CDrawRes::CLR_RED),
			g_DataInfo.GetColor(CDrawRes::CLR_PINK),
			g_DataInfo.GetColor(CDrawRes::CLR_YELLOW),
		};

		m_DataInfo.bIsInt = TRUE;
		m_DataInfo.bShowPercent = FALSE;
		m_DataInfo.bIsFixWidth = FALSE;
		m_DataInfo.bShowMultiData = TRUE;
		strcpy(m_DataInfo.szHUnitName, "总期数");
		strcpy(m_DataInfo.szVUnitName, "个数");

		m_DataInfo.lLineNum = 3;
		m_DataInfo.pLineData = new DVCT_LINE_DATA[m_DataInfo.lLineNum];
		memset(m_DataInfo.pLineData, 0, sizeof(DVCT_LINE_DATA) * m_DataInfo.lLineNum);
		DVCT_LINE_DATA *pLineDataTmp = m_DataInfo.pLineData;
		for (long a = 0; a < m_DataInfo.lLineNum; a++, pLineDataTmp++)
		{
			strcpy(pLineDataTmp->szName, pName[a]);
			pLineDataTmp->clrLine = clrUse[a];

			pLineDataTmp->lUnitNum = lNum;
			pLineDataTmp->pUnitData = new DVCT_UNIT_DATA[pLineDataTmp->lUnitNum];
			memset(pLineDataTmp->pUnitData, 0, sizeof(DVCT_UNIT_DATA) * pLineDataTmp->lUnitNum);
			DVCT_UNIT_DATA *pUnitDataTmp = pLineDataTmp->pUnitData;
			for (long b = 0; b < pLineDataTmp->lUnitNum; b++, pUnitDataTmp++)
			{
				long lPeriod = (m_pEtchPeriodRate + b)->lPeriod;

				if (lPeriod % 5 == 0)					
					sprintf(pUnitDataTmp->szMarkName, "%d", lPeriod);

				sprintf(pUnitDataTmp->szPeriodName, "总第[%d]期", lPeriod);

				pUnitDataTmp->lVal = (m_pEtchPeriodRate + b)->lLayer[a];
			}
		}

		m_DataInfo.lCurrDataIndex = 0;
		m_DvChart.SetData(&m_DataInfo);
	}
}

/////////////////////////////////////////////////////////////////////////////////
////IFilterDrawCtrl
IFilterDrawCtrl::IFilterDrawCtrl()
{
	memset(&m_FilterRetInfo, 0, sizeof(LTDC_FILTERSTAT_RET));

	memset(&m_BaseInfo, 0, sizeof(DVCT_BASE_INFO));
	memset(&m_DataInfo, 0, sizeof(DVCT_DATA_INFO));

	m_bIsColumn = FALSE;
	m_lMinCellWidth = 5;
	m_lMaxCellWidth = 50;
}

IFilterDrawCtrl::~IFilterDrawCtrl()
{
	//free stat data
	if (m_FilterRetInfo.plData)
		delete m_FilterRetInfo.plData;

	//free chart data
	FreeDataInfo();
}

void IFilterDrawCtrl::FreeDataInfo()
{
	for (long a = 0; a < m_DataInfo.lLineNum; a++)
	{
		delete (m_DataInfo.pLineData + a)->pUnitData;
	}

	delete m_DataInfo.pLineData;
	m_DataInfo.pLineData = NULL;
	m_DataInfo.lLineNum = 0;
}

BOOL IFilterDrawCtrl::RWCfg(BOOL bIsRead)
{
	if (bIsRead)
	{
		strcpy(m_BaseInfo.szName, m_szName);		

		m_BaseInfo.clrBkGround = g_DataInfo.GetColor(CDrawRes::CLR_BKGROUND);
		m_BaseInfo.clrFrame = g_DataInfo.GetColor(CDrawRes::CLR_FRAME);
		m_BaseInfo.clrRuler = g_DataInfo.GetColor(CDrawRes::CLR_CELL);
		m_BaseInfo.clrGray = g_DataInfo.GetColor(CDrawRes::CLR_GRAY);

		m_BaseInfo.hFont[0] = (HFONT)(g_DataInfo.GetFont(CDrawRes::FONT_CH_MIDDLE))->GetSafeHandle();
		m_BaseInfo.hFont[1] = (HFONT)(g_DataInfo.GetFont(CDrawRes::FONT_CH_SMALL))->GetSafeHandle();
		m_BaseInfo.hFont[2] = (HFONT)(g_DataInfo.GetFont(CDrawRes::FONT_EN_SMALL))->GetSafeHandle();

		m_BaseInfo.lCornerRadius = LTDC_CORNER_VALUE;

		m_DvChart.SetBaseInfo(&m_BaseInfo);
	}

	return TRUE;
}

void IFilterDrawCtrl::SetData(LT_DATA_INFO *pData, long lDataNum, 
					long lSelFrom, long lSelFor, BOOL bHaveSpec)
{
	IDrawCtrl::SetData(pData, lDataNum, lSelFrom, lSelFor, bHaveSpec);
	
	GetNormalVal();
	SetNormalRet();
}

void IFilterDrawCtrl::GetNormalVal()
{
	if (m_FilterRetInfo.plData)
		delete m_FilterRetInfo.plData;
	
	if (m_lDataNum > 0)
	{
		long *plHist = new long[m_lDataNum];		
		memset(plHist, 0, sizeof(long) * m_lDataNum);
		long *plHistTemp = plHist;

		LT_DATA_INFO *pDataTemp = m_pDataInfo;
		for(long a = 0; a < m_lDataNum; a++, plHistTemp++, pDataTemp++)
		{
			if (a == 0)
				GetEachNormalVal(NULL, pDataTemp, plHistTemp);
			else
				GetEachNormalVal(pDataTemp - 1, pDataTemp, plHistTemp);
		}

		LTSH_NORMALSTAT_RET ns;
		memset(&ns, 0, sizeof(LTSH_NORMALSTAT_RET));
		ns.plData = plHist;
		ns.lNum = m_lDataNum;
		GetMinMaxVal(ns.lAllowMix, ns.lAllowMax);
		g_StatHelper.NormalStat(&ns);

		m_FilterRetInfo.plData = plHist;
		m_FilterRetInfo.lNum = m_lDataNum;
		m_FilterRetInfo.fAve = ns.fAve;
		m_FilterRetInfo.fAveUp = ns.fAveUp;
		m_FilterRetInfo.fAveDown = ns.fAveDown;
		m_FilterRetInfo.fRetUp = ns.fRetUp;
		m_FilterRetInfo.fRetDown = ns.fRetDown;
		m_FilterRetInfo.lRetUp = ns.lRetUp;
		m_FilterRetInfo.lRetDown = ns.lRetDown;
	}
	else
		memset(&m_FilterRetInfo, 0, sizeof(LTDC_FILTERSTAT_RET));	
}

void IFilterDrawCtrl::SetNormalRet()
{
	FreeDataInfo();

	m_DataInfo.bIsInt = TRUE;
	m_DataInfo.bShowPercent = FALSE;
	m_DataInfo.bIsFixWidth = FALSE;
	m_DataInfo.bShowMultiData = FALSE;
	strcpy(m_DataInfo.szHUnitName, "总期数");
	strcpy(m_DataInfo.szVUnitName, "值");

	m_DataInfo.lLineNum = 1;
	m_DataInfo.pLineData = new DVCT_LINE_DATA[m_DataInfo.lLineNum];
	memset(m_DataInfo.pLineData, 0, sizeof(DVCT_LINE_DATA) * m_DataInfo.lLineNum);
	DVCT_LINE_DATA *pLineDataTmp = m_DataInfo.pLineData;
	for (long a = 0; a < m_DataInfo.lLineNum; a++, pLineDataTmp++)
	{
		strcpy(pLineDataTmp->szName, "当前值");
		pLineDataTmp->clrLine = g_DataInfo.GetColor(CDrawRes::CLR_RED);
		pLineDataTmp->lUnitNum = m_FilterRetInfo.lNum;
		pLineDataTmp->pUnitData = new DVCT_UNIT_DATA[pLineDataTmp->lUnitNum];
		memset(pLineDataTmp->pUnitData, 0, sizeof(DVCT_UNIT_DATA) * pLineDataTmp->lUnitNum);

		DVCT_UNIT_DATA *pUnitDataTmp = pLineDataTmp->pUnitData;
		for (long b = 0; b < m_FilterRetInfo.lNum; b++, pUnitDataTmp++)
		{
			if ((b + 1 )% 5 == 0)
				sprintf(pUnitDataTmp->szMarkName, "%d", b + 1);

			sprintf(pUnitDataTmp->szPeriodName, "总第[%d]期", b + 1);

			pUnitDataTmp->lVal = m_FilterRetInfo.plData[b];
		}
	}

	m_DataInfo.lCurrDataIndex = 0;
	m_DvChart.SetData(&m_DataInfo);
}

void IFilterDrawCtrl::SetParam(RECT *pRect)
{
	IDrawCtrl::SetParam(pRect);

	m_DvChart.SetParam(m_rtShow);
}

void IFilterDrawCtrl::SetScrollPos(long lStartPos, long lCurrPos)
{
	m_DvChart.SetPosInfo(lStartPos, lCurrPos);
}

void IFilterDrawCtrl::GetObjState(BOOL bBtns[6], long &lStartPos, long &lCurrPos,
				long &lOnePageNum, long &lScrollRange, long &lTotalNum)
{
	m_DvChart.GetDrawInfo(m_lStartPos, m_lCurrPos, m_lOnePageNum, m_lScrollRange, m_lTotalNum,
				m_lCellWidth, m_bIsColumn);

	lStartPos = m_lStartPos;
	lCurrPos = m_lCurrPos;
	lOnePageNum = m_lOnePageNum;
	lScrollRange = m_lScrollRange;
	lTotalNum = m_lTotalNum;

	if (m_lCurrPos > 0)
		bBtns[0] = TRUE;
	
	if (m_lCurrPos < m_lTotalNum - 1)
		bBtns[1] = TRUE;

	if (m_lCellWidth < m_lMaxCellWidth)
		bBtns[2] = TRUE;

	if (m_lCellWidth > m_lMinCellWidth)
		bBtns[3] = TRUE;

	bBtns[4] = bBtns[5] = FALSE;
}

BOOL IFilterDrawCtrl::OnKeyDown(UINT nChar, BOOL bIsCtrlKeyDown)
{
	BOOL bRet = FALSE;

	long lNewPos = m_lCurrPos;
	long lNewWidth = m_lCellWidth;
	long lOldPageNum = m_lOnePageNum;
	BOOL bColumn = m_bIsColumn;

	switch (nChar)
	{
	case VK_LEFT:
		lNewPos -= bIsCtrlKeyDown ? m_lOnePageNum : 1;
		break;
	case VK_RIGHT:
		lNewPos += bIsCtrlKeyDown ? m_lOnePageNum : 1;
		break;
	case VK_UP:
		lNewWidth++;
		break;
	case VK_DOWN:
		lNewWidth--;
		break;
	case VK_HOME:
		lNewPos = m_lStartPos;
		break;
	case VK_END:
		lNewPos = m_lStartPos + m_lOnePageNum - 1;
		break;
	case VK_TAB:
		{
			if (bIsCtrlKeyDown)
				bColumn ^= 1;
			else
				return FALSE;
		}
		break;
	default:
		return FALSE;
		break;
	}

	if (lNewPos < 0)
		lNewPos = 0;
	else if (lNewPos > m_lTotalNum - 1)
		lNewPos = m_lTotalNum - 1;

	if (lNewWidth > m_lMaxCellWidth)
		lNewWidth = m_lMaxCellWidth;
	else if (lNewWidth < m_lMinCellWidth)
		lNewWidth = m_lMinCellWidth;	

	if (lNewPos != m_lCurrPos)
	{
		m_lCurrPos = lNewPos;

		long lOldStart = m_lStartPos;
		if (lNewPos < m_lStartPos)
			m_lStartPos = lNewPos;
		else if (lNewPos > m_lStartPos + m_lOnePageNum - 1)
			m_lStartPos = lNewPos - m_lOnePageNum + 1;

		m_DvChart.SetPosInfo(m_lStartPos, m_lCurrPos);
		bRet = TRUE;
	}
	
	if (lNewWidth != m_lCellWidth)
	{
		m_lOnePageNum = m_DvChart.SetCellWidth(lNewWidth);		
		if (lOldPageNum > m_lOnePageNum)
		{
			if (m_lCurrPos > m_lStartPos + m_lOnePageNum - 1)
			{
				m_lCurrPos = m_lStartPos + m_lOnePageNum - 1;
				m_DvChart.SetPosInfo(m_lStartPos, m_lCurrPos);
			}
		}

		bRet = TRUE;
	}
	
	if (bColumn != m_bIsColumn)
	{
		m_bIsColumn = bColumn;

		m_DvChart.SetIsColumn(m_bIsColumn);
		bRet = TRUE;
	}

	return bRet;
}

BOOL IFilterDrawCtrl::OnLButtonDown(UINT nFlags, POINT point)
{
	return m_DvChart.OnLButtonDown(nFlags, point);
}

BOOL IFilterDrawCtrl::OnMouseMove(UINT nFlags, POINT point)
{
	return m_DvChart.OnMouseMove(nFlags, point);
}

void IFilterDrawCtrl::Draw(HDC hDC, long lShowType)
{
	m_DvChart.Draw(hDC, lShowType);
}

void IFilterDrawCtrl::GetFinalRet(long &lStartVal, long &lEndVal)
{	
	lStartVal = m_FilterRetInfo.lRetDown;
	lEndVal = m_FilterRetInfo.lRetUp;
}

BOOL IFilterDrawCtrl::IsValidate(long lData[LT_MAX_SELFOR], long lStartVal, long lEndVal)
{
	BOOL bRet = FALSE;

	long lNormalVal = 0;
	
	LT_DATA_INFO dataInfo;
	memset(&dataInfo, 0, sizeof(LT_DATA_INFO));
	memcpy(dataInfo.lData, lData, sizeof(long) * LT_MAX_SELFOR);

	if (m_lDataNum > 0)
		GetEachNormalVal(m_pDataInfo + m_lDataNum - 1, &dataInfo, &lNormalVal);
	else
		GetEachNormalVal(NULL, &dataInfo, &lNormalVal);

	if (lNormalVal >= lStartVal && lNormalVal <= lEndVal)
		bRet = TRUE;

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// CSumFilter
CSumFilter::CSumFilter()
{
	strcpy(m_szName, "总值");
}

CSumFilter::~CSumFilter()
{
}

void CSumFilter::GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
			LT_DATA_INFO *pCurrDataInfo, long *pRetVal)
{
	long lNormal = 0;
	for(long a = 0; a < m_lSelFor; a++)
	{
		lNormal += pCurrDataInfo->lData[a];
	}

	*pRetVal = lNormal;
}

void CSumFilter::GetMinMaxVal(long &lMin, long &lMax)
{
	lMax = lMin = 0;
	for (long a = 0; a < m_lSelFor; a++)
	{
		lMin += a + 1;
		lMax += m_lSelFrom - a;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CSingleFilter
CSingleFilter::CSingleFilter()
{
	strcpy(m_szName, "个位和");
}

CSingleFilter::~CSingleFilter()
{
}

void CSingleFilter::GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
			LT_DATA_INFO *pCurrDataInfo, long *pRetVal)
{
	long lNormal = 0;
	for(long a = 0; a < m_lSelFor; a++)
	{
		lNormal += (pCurrDataInfo->lData[a] % 10);
	}

	*pRetVal = lNormal;
}

void CSingleFilter::GetMinMaxVal(long &lMin, long &lMax)
{
	lMin = 0;
	lMax = m_lSelFor * 9;
}

/////////////////////////////////////////////////////////////////////////////
// COddFilter
COddFilter::COddFilter()
{
	strcpy(m_szName, "奇数");
}

COddFilter::~COddFilter()
{
}

void COddFilter::GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
			LT_DATA_INFO *pCurrDataInfo, long *pRetVal)
{
	long lNormal = 0;
	for(long a = 0; a < m_lSelFor; a++)
	{
		if (pCurrDataInfo->lData[a] % 2) lNormal++; 
	}

	*pRetVal = lNormal;
}

void COddFilter::GetMinMaxVal(long &lMin, long &lMax)
{
	lMin = 0;
	lMax = m_lSelFor;
}

/////////////////////////////////////////////////////////////////////////////
// CBrotherFilter
CBrotherFilter::CBrotherFilter()
{
	strcpy(m_szName, "连号");
}

CBrotherFilter::~CBrotherFilter()
{
}

void CBrotherFilter::GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
			LT_DATA_INFO *pCurrDataInfo, long *pRetVal)
{
	long lNormal = 0;
	for(long a = 0; a < m_lSelFor; a++)
	{
		for(long b = a + 1; b < m_lSelFor; b++)
		{
			if (abs(pCurrDataInfo->lData[a] - pCurrDataInfo->lData[b]) == 1)
				lNormal++;
		}
	}

	*pRetVal = lNormal;
}

void CBrotherFilter::GetMinMaxVal(long &lMin, long &lMax)
{
	lMin = 0;
	lMax = m_lSelFor;
}

/////////////////////////////////////////////////////////////////////////////
// CRepeatFilter
CRepeatFilter::CRepeatFilter()
{
	strcpy(m_szName, "重复号");
}

CRepeatFilter::~CRepeatFilter()
{
}

void CRepeatFilter::GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
			LT_DATA_INFO *pCurrDataInfo, long *pRetVal)
{
	long lNormal = 0;
	if (pLastDataInfo)
	{
		for(long a = 0; a < m_lSelFor; a++)
		{
			for(long b = 0; b < m_lSelFor; b++)
			{
				if (pCurrDataInfo->lData[a] == pLastDataInfo->lData[b])
				{
					lNormal++;
					break;
				}
			}
		}
	}

	*pRetVal = lNormal;
}

void CRepeatFilter::GetMinMaxVal(long &lMin, long &lMax)
{
	lMin = 0;
	lMax = m_lSelFor;
}

/////////////////////////////////////////////////////////////////////////////
// CSameTailFilter
CSameTailFilter::CSameTailFilter()
{
	strcpy(m_szName, "同尾号");
}

CSameTailFilter::~CSameTailFilter()
{
}

void CSameTailFilter::GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
			LT_DATA_INFO *pCurrDataInfo, long *pRetVal)
{
	long lNormal = 0;
	for (long a = 0; a < 10; a++)
	{
		long lTemp = 0;
		for (long b = 0; b < m_lSelFor; b++)
		{
			if (pCurrDataInfo->lData[b] % 10 == a)
				lTemp++;
		}
		lTemp -= 1;

		if (lTemp > 0) lNormal += lTemp;
	}

	*pRetVal = lNormal;
}

void CSameTailFilter::GetMinMaxVal(long &lMin, long &lMax)
{
	lMin = 0;
	lMax = m_lSelFor;
}

/////////////////////////////////////////////////////////////////////////////
// CSeparateFilter
CSeparateFilter::CSeparateFilter()
{
	strcpy(m_szName, "间隔号");
}

CSeparateFilter::~CSeparateFilter()
{
}

void CSeparateFilter::GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
			LT_DATA_INFO *pCurrDataInfo, long *pRetVal)
{
	long lNormal = 0;
	for(long a = 0; a < m_lSelFor; a++)
	{
		for(long b = a + 1; b < m_lSelFor; b++)
		{
			if (abs(pCurrDataInfo->lData[a] - pCurrDataInfo->lData[b]) == 2)
				lNormal++;
		}
	}

	*pRetVal = lNormal;
}

void CSeparateFilter::GetMinMaxVal(long &lMin, long &lMax)
{
	lMin = 0;
	lMax = m_lSelFor;
}

/////////////////////////////////////////////////////////////////////////////
// CRollFilter
CRollFilter::CRollFilter()
{
	strcpy(m_szName, "摇摆号");
}

CRollFilter::~CRollFilter()
{
}

void CRollFilter::GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
			LT_DATA_INFO *pCurrDataInfo, long *pRetVal)
{
	long lNormal = 0;
	if (pLastDataInfo)
	{
		for(long a = 0; a < m_lSelFor; a++)
		{
			for(long b = 0; b < m_lSelFor; b++)
			{
				if (abs(pCurrDataInfo->lData[a] - pLastDataInfo->lData[b]) == 1)
				{
					lNormal++;
					break;
				}
			}
		}
	}

	*pRetVal = lNormal;
}

void CRollFilter::GetMinMaxVal(long &lMin, long &lMax)
{
	lMin = 0;
	lMax = m_lSelFor;
}

/////////////////////////////////////////////////////////////////////////////
// CDistanceFilter
CDistanceFilter::CDistanceFilter()
{
	strcpy(m_szName, "跨距");
}

CDistanceFilter::~CDistanceFilter()
{
}

void CDistanceFilter::GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
			LT_DATA_INFO *pCurrDataInfo, long *pRetVal)
{
	long lNormal = 0;
	long lMin = m_lSelFrom, lMax = 1;
	for(long a = 0; a < m_lSelFor; a++)
	{
		if (pCurrDataInfo->lData[a] < lMin)
			lMin = pCurrDataInfo->lData[a];

		if (pCurrDataInfo->lData[a] > lMax)
			lMax = pCurrDataInfo->lData[a];
	}

	*pRetVal = lMax - lMin;
}

void CDistanceFilter::GetMinMaxVal(long &lMin, long &lMax)
{
	lMin = 0;
	lMax = m_lSelFrom - 1;
}

/////////////////////////////////////////////////////////////////////////////
// CACFilter
CACFilter::CACFilter()
{
	strcpy(m_szName, "AC值");
}

CACFilter::~CACFilter()
{
}

void CACFilter::GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
			LT_DATA_INFO *pCurrDataInfo, long *pRetVal)
{
	//计算方法：
	//AC值，一组数中所有两个数组合相差绝对值会不同，不同的个数减去（选择号码数－1）即为
	//AC值，r/s, 如35选7，r＝7，s＝35
	//如：01 05 09 13 18 26 34，共有13个不同的差值，AC=13-（7-1）=7

	long lNormal = 0;

	long lACRank[LT_MAX_SELFROM] = {0};
	for(long a = 0; a < m_lSelFor; a++)
	{
		for(long b = a + 1; b < m_lSelFor; b++)
		{
			lACRank[abs(pCurrDataInfo->lData[a] - pCurrDataInfo->lData[b])] = 1;
		}
	}

	for (long a = 0; a < LT_MAX_SELFROM; a++)
	{
		lNormal += lACRank[a];
	}

	*pRetVal = lNormal - (m_lSelFor - 1);
}

void CACFilter::GetMinMaxVal(long &lMin, long &lMax)
{
	lMin = 0;
	lMax = 0;

	//方法:
	// (1 + 2 + ... + (m_lSelFor - 1)) - (m_lSelFor - 1)
	for (long a = 1; a < m_lSelFor; a++)
	{
		lMax += a;
	}

	lMax -= m_lSelFor - 1;
}

/////////////////////////////////////////////////////////////////////////////////
///
//analyse ctrl
long g_lAnalyseCtrlNum = 2;
CMainAnalyse g_MainAnalyse;
CZoneAnalyse g_ZoneAnalyse;

//stat ctrl
long g_lStateCtrlNum = 2;
CProbStat g_ProbStat;
CThreeNoStat g_ThreeNoStat;

//filter ctrl
long g_lFilterCtrlNum = 10;
CSumFilter g_SumFilter;
CSingleFilter g_SingleFilter;
COddFilter g_OddFilter;
CBrotherFilter g_BrotherFilter;
CRepeatFilter g_RepeatFilter;
CSameTailFilter g_SameTailFilter;
CSeparateFilter g_SeparateFilter;
CRollFilter g_RollFilter;
CDistanceFilter g_DistanceFilter;
CACFilter g_ACFilter;

/////////////////////////////////////////////////////////////////////////////////
////CDrawProxy
CDrawProxy::CDrawProxy()
{
	m_lCurrCtrl = 0;
	
	strcpy(m_DrawCtrlInfo[0].szName, "基本分析");
	m_DrawCtrlInfo[0].lStartPos = 0;
	m_DrawCtrlInfo[0].lCurrPos = 0;
	m_DrawCtrlInfo[0].lOnePageNum = 1;
	m_DrawCtrlInfo[0].lTotalNum = g_lAnalyseCtrlNum;
	m_DrawCtrlInfo[0].lScrollRange = 1;
	m_DrawCtrlInfo[0].lPanelSpace = 15;
	memset(m_DrawCtrlInfo[0].m_PanelRect, 0, sizeof(RECT) * 5);
	memset(m_DrawCtrlInfo[0].pObj, 0, sizeof(IDrawCtrl *) * 20);
	m_DrawCtrlInfo[0].pObj[0] = &g_MainAnalyse;
	m_DrawCtrlInfo[0].pObj[1] = &g_ZoneAnalyse;		

	strcpy(m_DrawCtrlInfo[1].szName, "统计分析");
	m_DrawCtrlInfo[1].lStartPos = 0;
	m_DrawCtrlInfo[1].lCurrPos = 0;
	m_DrawCtrlInfo[1].lOnePageNum = 2;
	m_DrawCtrlInfo[1].lTotalNum = g_lStateCtrlNum;
	m_DrawCtrlInfo[1].lScrollRange = 0;
	m_DrawCtrlInfo[1].lPanelSpace = 15;
	memset(m_DrawCtrlInfo[1].m_PanelRect, 0, sizeof(RECT) * 5);
	memset(m_DrawCtrlInfo[1].pObj, 0, sizeof(IDrawCtrl *) * 20);
	m_DrawCtrlInfo[1].pObj[0] = &g_ProbStat;
	m_DrawCtrlInfo[1].pObj[1] = &g_ThreeNoStat;

	strcpy(m_DrawCtrlInfo[2].szName, "走势分析");
	m_DrawCtrlInfo[2].lStartPos = 0;
	m_DrawCtrlInfo[2].lCurrPos = 0;
	m_DrawCtrlInfo[2].lOnePageNum = 3;
	m_DrawCtrlInfo[2].lTotalNum = g_lFilterCtrlNum;
	m_DrawCtrlInfo[2].lScrollRange = 7;
	m_DrawCtrlInfo[2].lPanelSpace = 15;
	memset(m_DrawCtrlInfo[2].m_PanelRect, 0, sizeof(RECT) * 5);
	memset(m_DrawCtrlInfo[2].pObj, 0, sizeof(IDrawCtrl *) * 20);
	m_DrawCtrlInfo[2].pObj[0] = &g_SumFilter;
	m_DrawCtrlInfo[2].pObj[1] = &g_SingleFilter;
	m_DrawCtrlInfo[2].pObj[2] = &g_OddFilter;
	m_DrawCtrlInfo[2].pObj[3] = &g_BrotherFilter;
	m_DrawCtrlInfo[2].pObj[4] = &g_RepeatFilter;
	m_DrawCtrlInfo[2].pObj[5] = &g_SameTailFilter;
	m_DrawCtrlInfo[2].pObj[6] = &g_SeparateFilter;
	m_DrawCtrlInfo[2].pObj[7] = &g_RollFilter;
	m_DrawCtrlInfo[2].pObj[8] = &g_DistanceFilter;
	m_DrawCtrlInfo[2].pObj[9] = &g_ACFilter;

	m_lMouseHoverPos = -1;
	m_rtShow.SetRectEmpty();
	m_lShowType = 0;
}

CDrawProxy::~CDrawProxy()
{
}

char *CDrawProxy::GetCtrlName(long lCtrl)
{
	return m_DrawCtrlInfo[lCtrl].szName;
}

char *CDrawProxy::GetObjName(long lCtrl, long lObj)
{
	return m_DrawCtrlInfo[lCtrl].pObj[lObj]->GetName();
}

long CDrawProxy::GetObjNum(long lCtrl)
{
	return m_DrawCtrlInfo[lCtrl].lTotalNum;
}

void CDrawProxy::SetCurrCtrlObj(long lCurrCtrl, long lCurrObj)
{
	BOOL bNeedResetRect = FALSE;

	//1. judge if ctrl type changed.
	if (m_lCurrCtrl != lCurrCtrl)
	{
		m_lCurrCtrl = lCurrCtrl;
		bNeedResetRect = TRUE;
	}

	//2. judge if item changed.
	long lOldCurrPos = m_DrawCtrlInfo[lCurrCtrl].lCurrPos;
	if (lCurrObj != lOldCurrPos)
	{
		long lStartPos = m_DrawCtrlInfo[lCurrCtrl].lStartPos;

		if (lCurrObj < lStartPos)
			lStartPos = lCurrObj;
		else if (lCurrObj > lStartPos + m_DrawCtrlInfo[lCurrCtrl].lOnePageNum - 1)
			lStartPos = lCurrObj - m_DrawCtrlInfo[lCurrCtrl].lOnePageNum + 1;

		if (lStartPos != m_DrawCtrlInfo[lCurrCtrl].lStartPos)
		{
			m_DrawCtrlInfo[lCurrCtrl].lStartPos = lStartPos;
			bNeedResetRect = TRUE;			
		}

		m_DrawCtrlInfo[lCurrCtrl].lCurrPos = lCurrObj;
	}

	//3. reset draw rect param
	if (bNeedResetRect)
		SetObjParam(lCurrCtrl);	
}

long CDrawProxy::GetShowNum(long lCurrCtrl)
{
	long lShowNum = m_DrawCtrlInfo[lCurrCtrl].lTotalNum - m_DrawCtrlInfo[lCurrCtrl].lStartPos;

	if (lShowNum > m_DrawCtrlInfo[lCurrCtrl].lOnePageNum)
		lShowNum = m_DrawCtrlInfo[lCurrCtrl].lOnePageNum;

	return lShowNum;
}

void CDrawProxy::SetObjParam(long lCurrCtrl)
{
	long lStartPos = m_DrawCtrlInfo[lCurrCtrl].lStartPos;
	for (long a = 0; a < GetShowNum(lCurrCtrl); a++)
	{
		RECT *pTmpRect = &m_DrawCtrlInfo[lCurrCtrl].m_PanelRect[a];		
		m_DrawCtrlInfo[lCurrCtrl].pObj[lStartPos + a]->SetParam(pTmpRect);
	}
}

BOOL CDrawProxy::RWCfg(BOOL bIsRead/* = TRUE*/)
{	
	CDvXML &dvXml = theApp.m_DvXml;

	DVXML_NODE *pRoot = dvXml.GetRoot();
	DVXML_NODE *pTmpNode = NULL;

	long a, b;
	CString strItem, strVal;

	//1. read proxy config
	if (bIsRead)
	{
		pTmpNode = dvXml.OperateNode(pRoot, "DrawProxy", CDvXML::CHECKONLY);
		if (pTmpNode)
		{
			dvXml.GetAttrib(pTmpNode, "CurrCtrl", m_lCurrCtrl);
			dvXml.GetAttrib(pTmpNode, "CurrCtrlPos", m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos);
		}
	}
	else
	{
		pTmpNode = dvXml.OperateNode(pRoot, "DrawProxy", CDvXML::DELETECREATE);
		if (pTmpNode)
		{
			dvXml.SetAttrib(pTmpNode, "CurrCtrl", m_lCurrCtrl);
			dvXml.SetAttrib(pTmpNode, "CurrCtrlPos", m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos);
		}
	}

	//2. read ctrls config.
	for (a = 0; a < 3; a++)
	{
		for (b = 0; b < m_DrawCtrlInfo[a].lTotalNum; b++)
		{
			m_DrawCtrlInfo[a].pObj[b]->RWCfg(bIsRead);
		}
	}

	return TRUE;
}

void CDrawProxy::SetData(LT_DATA_INFO *pData, long lDataNum,
			long lSelFrom, long lSelFor, BOOL bHaveSpec)
{
	for (long a = 0; a < 3; a++)
	{
		for (long b = 0; b < m_DrawCtrlInfo[a].lTotalNum; b++)
		{
			m_DrawCtrlInfo[a].pObj[b]->SetData(pData, lDataNum, lSelFrom, lSelFor, bHaveSpec);
		}
	}
}

void CDrawProxy::SetParam(RECT *pRect)
{
	//reset all ctrl panel rect.
	if (pRect && !::IsRectEmpty(pRect))
	{
		m_rtShow = pRect;

		long lPanelHt, Y, lPanelSpace, lOnePageNum;
		for (long a = 0; a < 3; a++)
		{
			lPanelSpace = m_DrawCtrlInfo[a].lPanelSpace;
			lOnePageNum = m_DrawCtrlInfo[a].lOnePageNum;
			
			lPanelHt = m_rtShow.Height() / lOnePageNum;

			Y = m_rtShow.top;
			for (long b = 0; b < lOnePageNum; b++, Y += lPanelHt)
			{
				m_DrawCtrlInfo[a].m_PanelRect[b].left = m_rtShow.left + lPanelSpace;
				m_DrawCtrlInfo[a].m_PanelRect[b].top = Y + lPanelSpace;
				m_DrawCtrlInfo[a].m_PanelRect[b].right = m_rtShow.right - lPanelSpace;
				m_DrawCtrlInfo[a].m_PanelRect[b].bottom = Y + lPanelHt - lPanelSpace;
			}
		}
	}
	else
	{
		for (long a = 0; a < 3; a++)
			memset(m_DrawCtrlInfo[a].m_PanelRect, 0, sizeof(RECT) * 5);
	}

	//set current ctrl param	
	SetObjParam(m_lCurrCtrl);
}

void CDrawProxy::SetScrollPos(long lType, long lStartPos, long lCurrPos)
{
	if (lType == 0)
	{
		if (lStartPos != m_DrawCtrlInfo[m_lCurrCtrl].lStartPos)
		{
			m_DrawCtrlInfo[m_lCurrCtrl].lStartPos = lStartPos;
			SetObjParam(m_lCurrCtrl);
		}

		m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos = lCurrPos;
	}
	else
	{
		long lCurrCtrlPos = m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos;
		m_DrawCtrlInfo[m_lCurrCtrl].pObj[lCurrCtrlPos]->SetScrollPos(lStartPos, lCurrPos);
	}
}

void CDrawProxy::GetCtrlState(long &lCtrlStartPos, long &lCtrlCurrPos, long &lCtrlOnePageNum, 
								long &lCtrlScrollRange, long &lCtrlTotalNum)
{
	lCtrlStartPos = m_DrawCtrlInfo[m_lCurrCtrl].lStartPos;
	lCtrlCurrPos = m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos;
	lCtrlOnePageNum = m_DrawCtrlInfo[m_lCurrCtrl].lOnePageNum;
	lCtrlScrollRange = m_DrawCtrlInfo[m_lCurrCtrl].lScrollRange;
	lCtrlTotalNum = m_DrawCtrlInfo[m_lCurrCtrl].lTotalNum;
}

void CDrawProxy::GetObjState(BOOL bBtns[6], long &lStartPos, long &lCurrPos, long &lOnePageNum, 
								long &lScrollRange, long &lTotalNum)
{
	long lObjCurrPos = m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos;
	m_DrawCtrlInfo[m_lCurrCtrl].pObj[lObjCurrPos]->GetObjState(bBtns, lStartPos, 
					lCurrPos, lOnePageNum, lScrollRange, lTotalNum);
}


BOOL CDrawProxy::OnKeyDown(UINT nChar, BOOL bIsCtrlKeyDown)
{
	BOOL bRet = FALSE;

	long lCurrPos = m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos;
	bRet = m_DrawCtrlInfo[m_lCurrCtrl].pObj[lCurrPos]->OnKeyDown(nChar, bIsCtrlKeyDown);

	return bRet;
}

BOOL CDrawProxy::OnLButtonDown(UINT nFlags, POINT point)
{
	BOOL bRet = FALSE;

	for (long a = 0; a < GetShowNum(m_lCurrCtrl); a++)
	{
		RECT *pTmpRect = &m_DrawCtrlInfo[m_lCurrCtrl].m_PanelRect[a];
		if (::PtInRect(pTmpRect, point))
		{
			long lStartPos = m_DrawCtrlInfo[m_lCurrCtrl].lStartPos;
			if (m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos != a + lStartPos)
			{
				m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos = a + lStartPos;				
				bRet = TRUE;
			}
			else
			{
				long lCurrPos = m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos;
				bRet = m_DrawCtrlInfo[m_lCurrCtrl].pObj[lCurrPos]->OnLButtonDown(nFlags, point);
			}

			break;
		}
	}

	return bRet;
}

BOOL CDrawProxy::OnMouseMove(UINT nFlags, POINT point)
{
	BOOL bRet = FALSE;

	BOOL bFind = FALSE;
	//1. look if point in any item
	for (long a = 0; a < GetShowNum(m_lCurrCtrl); a++)
	{
		RECT *pTmpRect = &m_DrawCtrlInfo[m_lCurrCtrl].m_PanelRect[a];
		if (::PtInRect(pTmpRect, point))
		{
			long lOldHoverPos = m_lMouseHoverPos;
			m_lMouseHoverPos = m_DrawCtrlInfo[m_lCurrCtrl].lStartPos + a;

			if (m_lMouseHoverPos == m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos)
			{
				bRet = m_DrawCtrlInfo[m_lCurrCtrl].pObj[m_lMouseHoverPos]->OnMouseMove(nFlags, point);
			}
			else
			{
				if (m_lMouseHoverPos != lOldHoverPos)
					bRet = TRUE;
			}

			bFind = TRUE;
			break;
		}
	}

	//2. if not do anything
	if (!bFind)
	{
		if (m_lMouseHoverPos != -1)
		{			
			m_lMouseHoverPos = -1;
			bRet = TRUE;
		}		
	}

	return bRet;
}

void CDrawProxy::Draw(HDC hDC)
{
	//1. draw frame
	DrawFrame(hDC);

	//2. call ctrl's draw funciton.
	for (long a = 0; a < GetShowNum(m_lCurrCtrl); a++)
	{
		long lCurrPos = m_DrawCtrlInfo[m_lCurrCtrl].lStartPos + a;

		long lShowType = 0;
		if (lCurrPos == m_lMouseHoverPos)
			lShowType = 2;

		if (lCurrPos == m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos)
			lShowType = 1;

		m_DrawCtrlInfo[m_lCurrCtrl].pObj[lCurrPos]->Draw(hDC, lShowType);
	}
}

void CDrawProxy::DrawFrame(HDC hDC)
{
	CDC memDC;
	memDC.Attach(hDC);

	CBrush *pBrush, *pOldBrh;
	pBrush = g_DataInfo.GetBrush(CDrawRes::BRS_BKGROUND);
	pOldBrh = memDC.SelectObject(pBrush);

	CPen *pPen, *pOldPen;
	pPen = g_DataInfo.GetPen(CDrawRes::PEN_FRAME_BOLD);		//red
	pOldPen = memDC.SelectObject(pPen);
	
	long lCornerVal = 2 * LTDC_CORNER_VALUE;
	CPoint pt(lCornerVal, lCornerVal);
	memDC.RoundRect(m_rtShow, pt);

	memDC.SelectObject(pOldBrh);
	memDC.SelectObject(pOldPen);

	memDC.Detach();
}

CMenu *CDrawProxy::GetPopupMenu(CMenu *pSrcMenu, UINT nStartMenuID)
{
	CMenu *pRetMenu = NULL;

	long lCurrPos = m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos;
	pRetMenu = m_DrawCtrlInfo[m_lCurrCtrl].pObj[lCurrPos]->GetPopupMenu(pSrcMenu, nStartMenuID);
	
	return pRetMenu;
}

void CDrawProxy::OnCommand(UINT uCmdID)
{
	long lCurrPos = m_DrawCtrlInfo[m_lCurrCtrl].lCurrPos;
	m_DrawCtrlInfo[m_lCurrCtrl].pObj[lCurrPos]->OnCommand(uCmdID);
}
