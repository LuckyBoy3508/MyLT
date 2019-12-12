
#include "stdafx.h"
#include "mylt.h"
#include "DrawRes.h"
#include "DrawResSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
///
CDrawRes::CDrawRes()
{
	GetDefalultVal();
}

CDrawRes::~CDrawRes()
{
	DeleteDrawObject();
}

void CDrawRes::GetDefalultVal()
{
	//color	
	//stock.
	m_clrStock[0] = DV_CLR_BLACK;
	m_clrStock[1] = DV_CLR_WHITE;
	m_clrStock[2] = DV_CLR_RED;
	m_clrStock[3] = DV_CLR_GREEN;
	m_clrStock[4] = DV_CLR_BLUE;
	m_clrStock[5] = DV_CLR_YELLOW;
	m_clrStock[6] = DV_CLR_PINK;
	m_clrStock[7] = DV_CLR_CYAN;
	m_clrStock[8] = DV_CLR_GRAY;
	
	//custom
	m_clrStock[9] = RGB(144, 153, 174);
	m_clrStock[10] = RGB(153, 51, 102);
	m_clrStock[11] = DV_CLR_BLUE;

	//font
	LOGFONT lf[6] = 
	{	
		{-19, 0, 0, 0, 400, 0, 0, 0, 134, 3, 2, 1, 2, "ºÚÌå"},
		{-15, 0, 0, 0, 400, 0, 0, 0, 134, 3, 2, 1, 2, "ºÚÌå"},
		{-12, 0, 0, 0, 400, 0, 0, 0, 134, 3, 2, 1, 2, "ËÎÌå"},
		{-19, 0, 0, 0, 700, 0, 0, 0, 0, 3, 2, 1, 34, "Arial"},
		{-15, 0, 0, 0, 700, 0, 0, 0, 0, 3, 2, 1, 34, "Arial"},
		{-12, 0, 0, 0, 700, 0, 0, 0, 0, 3, 2, 1, 34, "Arial"},
	};

	memcpy(m_LogFont, lf, sizeof(LOGFONT) * 6);
}

void CDrawRes::RWCfg(BOOL bIsRead)
{
	CDvXML &dvXml = theApp.m_DvXml;

	DVXML_NODE *pTmpNode = NULL;
	DVXML_NODE *pRoot = dvXml.GetRoot();

	if (bIsRead)
	{
		//1. get res config
		DVXML_NODE *pDrawResNode = dvXml.OperateNode(pRoot, "DrawRes", CDvXML::CHECKONLY);
		if (pDrawResNode)
		{
			//color
			DVXML_NODE *pColorNode = dvXml.OperateNode(pDrawResNode, "Color", CDvXML::CHECKONLY);
			if (pColorNode)
			{
				dvXml.GetAttrib(pColorNode, "BackGround", m_clrStock[9]);
				dvXml.GetAttrib(pColorNode, "Frame", m_clrStock[10]);
				dvXml.GetAttrib(pColorNode, "Cell", m_clrStock[11]);
			}

			//font
			DVXML_NODE *pFontNode = dvXml.OperateNode(pDrawResNode, "Fonts", CDvXML::CHECKONLY);
			if (pFontNode)
			{
				long lCnt = 0;
				DVXML_NODE_LIST lstTmp;
				dvXml.SelectNodes(pFontNode, "Font", lstTmp);
				POSITION pos = lstTmp.GetHeadPosition();
				while (pos)
				{
					pTmpNode = lstTmp.GetNext(pos);

					dvXml.GetAttrib(pTmpNode, "Hight", m_LogFont[lCnt].lfHeight);
					dvXml.GetAttrib(pTmpNode, "Width", m_LogFont[lCnt].lfWidth);
					dvXml.GetAttrib(pTmpNode, "Escapement", m_LogFont[lCnt].lfEscapement);
					dvXml.GetAttrib(pTmpNode, "Orientation", m_LogFont[lCnt].lfOrientation);
					dvXml.GetAttrib(pTmpNode, "Weight", m_LogFont[lCnt].lfWeight);
					dvXml.GetAttrib(pTmpNode, "Italic", m_LogFont[lCnt].lfItalic);
					dvXml.GetAttrib(pTmpNode, "Underline", m_LogFont[lCnt].lfUnderline);
					dvXml.GetAttrib(pTmpNode, "StrikeOut", m_LogFont[lCnt].lfStrikeOut);
					dvXml.GetAttrib(pTmpNode, "CharSet", m_LogFont[lCnt].lfCharSet);
					dvXml.GetAttrib(pTmpNode, "OutPrecision", m_LogFont[lCnt].lfOutPrecision);
					dvXml.GetAttrib(pTmpNode, "ClipPrecision", m_LogFont[lCnt].lfClipPrecision);
					dvXml.GetAttrib(pTmpNode, "Quality", m_LogFont[lCnt].lfQuality);
					dvXml.GetAttrib(pTmpNode, "PitchAndFamily", m_LogFont[lCnt].lfPitchAndFamily);
					dvXml.GetAttrib(pTmpNode, "FaceName", m_LogFont[lCnt].lfFaceName);

					lCnt++;
				}
			}
		}

		//2. create draw object.
		CreateDrawObject();
	}
	else
	{
		//set res config
		DVXML_NODE *pDrawResNode = dvXml.OperateNode(pRoot, "DrawRes", CDvXML::DELETECREATE);
		if (pDrawResNode)
		{
			//color
			DVXML_NODE *pColorNode = dvXml.OperateNode(pDrawResNode, "Color", CDvXML::CREATEONLY);
			if (pColorNode)
			{
				dvXml.SetAttrib(pColorNode, "BackGround", m_clrStock[9]);
				dvXml.SetAttrib(pColorNode, "Frame", m_clrStock[10]);
				dvXml.SetAttrib(pColorNode, "Cell", m_clrStock[11]);
			}

			//font
			DVXML_NODE *pFontNode = dvXml.OperateNode(pDrawResNode, "Fonts", CDvXML::CREATEONLY);
			if (pFontNode)
			{
				for (long lCnt = 0; lCnt < 6; lCnt++)
				{
					pTmpNode = dvXml.OperateNode(pFontNode, "Font", CDvXML::CREATEONLY);

					if (pTmpNode)
					{
						dvXml.SetAttrib(pTmpNode, "Hight", m_LogFont[lCnt].lfHeight);
						dvXml.SetAttrib(pTmpNode, "Width", m_LogFont[lCnt].lfWidth);
						dvXml.SetAttrib(pTmpNode, "Escapement", m_LogFont[lCnt].lfEscapement);
						dvXml.SetAttrib(pTmpNode, "Orientation", m_LogFont[lCnt].lfOrientation);
						dvXml.SetAttrib(pTmpNode, "Weight", m_LogFont[lCnt].lfWeight);
						dvXml.SetAttrib(pTmpNode, "Italic", m_LogFont[lCnt].lfItalic);
						dvXml.SetAttrib(pTmpNode, "Underline", m_LogFont[lCnt].lfUnderline);
						dvXml.SetAttrib(pTmpNode, "StrikeOut", m_LogFont[lCnt].lfStrikeOut);
						dvXml.SetAttrib(pTmpNode, "CharSet", m_LogFont[lCnt].lfCharSet);
						dvXml.SetAttrib(pTmpNode, "OutPrecision", m_LogFont[lCnt].lfOutPrecision);
						dvXml.SetAttrib(pTmpNode, "ClipPrecision", m_LogFont[lCnt].lfClipPrecision);
						dvXml.SetAttrib(pTmpNode, "Quality", m_LogFont[lCnt].lfQuality);
						dvXml.SetAttrib(pTmpNode, "PitchAndFamily", m_LogFont[lCnt].lfPitchAndFamily);
						dvXml.SetAttrib(pTmpNode, "FaceName", m_LogFont[lCnt].lfFaceName);
					}
				}
			}
		}
	}	
}

void CDrawRes::DeleteDrawObject()
{
	long a;

	//font
	for (a = 0; a < 6; a++)
		m_StockFont[a].DeleteObject();

	//pen
	for (a = 0; a < 6; a++)
		m_StockPen[a].DeleteObject();

	//brush
	for (a = 0; a < 4; a++)
		m_StockBrush[a].DeleteObject();
}

void CDrawRes::CreateDrawObject()
{
	DeleteDrawObject();

	//font
	for (long a = 0; a < 6; a++)
	{
		m_StockFont[a].CreateFontIndirect(&m_LogFont[a]);
	}

	//pen
	m_StockPen[0].CreatePen(PS_SOLID, 2, m_clrStock[10]);
	m_StockPen[1].CreatePen(PS_SOLID, 1, m_clrStock[10]);
	m_StockPen[2].CreatePen(PS_SOLID, 2, m_clrStock[11]);
	m_StockPen[3].CreatePen(PS_SOLID, 1, m_clrStock[11]);
	m_StockPen[4].CreatePen(PS_SOLID, 2, m_clrStock[8]);
	m_StockPen[5].CreatePen(PS_SOLID, 1, m_clrStock[8]);

	//brush
	m_StockBrush[0].CreateSolidBrush(m_clrStock[9]);
	m_StockBrush[1].CreateSolidBrush(m_clrStock[10]);
	m_StockBrush[2].CreateSolidBrush(m_clrStock[11]);
	m_StockBrush[3].CreateSolidBrush(m_clrStock[8]);
}

COLORREF CDrawRes::GetColor(CLR_STYLE eColor)
{
	COLORREF clrRet;
	if (eColor == CDrawRes::CLR_RANDOM)
	{
		srand((unsigned int)time(NULL));

		long lR, lG, lB;
		lR = rand() % 255;
		lG = rand() % 255;
		lB = rand() % 255;

		clrRet = lR + (lG << 8) + (lB << 16);
	}
	else
		clrRet = m_clrStock[eColor];

	return clrRet;
}

CFont *CDrawRes::GetFont(FONT_STYLE eFont)
{
	return &m_StockFont[eFont];
}

CPen *CDrawRes::GetPen(PEN_STYLE ePen)
{
	return &m_StockPen[ePen];
}

CBrush *CDrawRes::GetBrush(BRS_STYLE eBrush)
{
	return &m_StockBrush[eBrush];
}

void CDrawRes::SetUp()
{
	CDrawResSetupDlg dlg;

	COLORREF clrParam[3] = { m_clrStock[9], m_clrStock[10], m_clrStock[11] };
	LOGFONT logFont[6];
	memcpy(logFont, m_LogFont, sizeof(LOGFONT) * 6);

	dlg.SetParam(clrParam, logFont);
	if (dlg.DoModal() == IDOK)
	{
		m_clrStock[9] = clrParam[0];
		m_clrStock[10] = clrParam[1];
		m_clrStock[11] = clrParam[2];

		memcpy(m_LogFont, logFont, sizeof(LOGFONT) * 6);

		CreateDrawObject();
	}
}

