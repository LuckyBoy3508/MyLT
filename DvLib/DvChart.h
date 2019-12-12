
#ifndef __DVCHART_H
#define __DVCHART_H

#include "DvDef.h"

#define DVCT_MINCELL_WIDTH		5
#define DVCT_MAXCELL_WIDTH		50
#define DVCT_DEFCELL_WIDTH		25

struct DVCT_BASE_INFO
{
	//name
	char szName[DV_LEN_SMALL + 1];

	//color
	COLORREF clrBkGround;
	COLORREF clrFrame;
	COLORREF clrRuler;
	COLORREF clrGray;

	//font  0:ctrl-name(e.g. Sum..) 1:val-name(e.g. hot-num) 1:val(23.04..)
	HFONT hFont[3];

	//property
	long lCornerRadius;		//frame corner radius.
};

struct DVCT_UNIT_DATA
{
	//unit mark name
	char szMarkName[DV_LEN_SMALL + 1];
	char szPeriodName[DV_LEN_SMALL + 1];

	//unit data
	long lVal;
	float fVal;
};

struct DVCT_LINE_DATA
{
	//line property
	char szName[DV_LEN_SMALL + 1];	//value name
	COLORREF clrLine;		//line color

	//unit data
	DVCT_UNIT_DATA *pUnitData;
	long lUnitNum;
};

struct DVCT_DATA_INFO
{
	//data property
	BOOL bIsInt;			//data type(long or float)	
	BOOL bShowPercent;		//validate only when bIsInt==FALSE
	BOOL bIsFixWidth;		//is can horizontal scorll
	BOOL bShowMultiData;	//is show multi data together(only in line state)	

	char szHUnitName[DV_LEN_SMALL + 1];	//horizontal mark explain
	char szVUnitName[DV_LEN_SMALL + 1];	//vertical mark explain		

	//line data
	DVCT_LINE_DATA *pLineData;
	long lLineNum;
	long lCurrDataIndex;
};

class AFX_CLASS_EXPORT CDvChart
{
public:
	CDvChart();
	~CDvChart();

public:
	//data info.
	void SetBaseInfo(DVCT_BASE_INFO *pBaseInfo);
	void SetData(DVCT_DATA_INFO *pData);
	void SetParam(RECT *pRect);

	//draw control
	void SetPosInfo(long lStartPos, long lCurrPos);
	void SetIsColumn(BOOL bIsColumn);
	long SetCellWidth(long lNewWidth);
	void GetDrawInfo(long &lStartPos, long &lCurrPos, 
					long &lOnePageNum, long &lScrollRange, long &lTotalNum, 
					long &lCellWidth, BOOL &bIsColumn);	

	//gui control
	BOOL OnLButtonDown(UINT nFlags, POINT point);
	BOOL OnMouseMove(UINT nFlags, POINT point);	

	//draw
	void Draw(HDC hDC, long lShowType);

private:
	void CalcPanelsRect();
	void ResetDrawInfo();

	BOOL DrawOutlook();	
	void DrawLine();
	void DrawCursor();

	void DrawRuler();
	void DrawKLine();	
	void DrawValue();

	BOOL IsHaveData();
	void ShowText(LPCTSTR lpszText, RECT *pRect, COLORREF clrText, long lPos, long lFontIndex);
	BOOL DrawScrollBar(BOOL bTest = FALSE, POINT *pPoint = NULL);
	BOOL DrawColumnFlag(BOOL bTest = FALSE, POINT *pPoint = NULL);
	void DrawKLineColumn(DVCT_LINE_DATA *pLineData);
	void DrawKLineEach(DVCT_LINE_DATA *pLineData, BOOL bActive);
	long GetCurrValPos(DVCT_LINE_DATA *pLineData, long lCurrPos);
	void GetVMarkInfo();
	long GetShowNum();
	COLORREF HightLight(COLORREF clrRef, long lPercent);		

private:
	DVCT_BASE_INFO m_BaseInfo;
	DVCT_DATA_INFO m_DataInfo;
	long m_lCurrLine;	
	CRect m_rtShow;

	long m_lStartPos;
	long m_lCurrPos;
	long m_lOnePageNum;
	long m_lScrollRange;
	long m_lTotalNum;
	long m_lCellWidth;
	BOOL m_bIsColumn;

	long m_lShowType;
	CDC m_MemDC;
	//0:wk 1:wk-show 2:title 3:title-show 4:name 5:value 6:scroll 7:colum
	CRect m_rtPanel[8];

private:
	struct DVCT_VMARK_INFO
	{
		long lMarkNum;
		long lMarkStep;
		long lValMin;
		long lValStep;
		float fValMin;
		float fValStep;
		float fStepRatio;	//= lMarkStep / lValStep[OR fValStep]
	};

	DVCT_VMARK_INFO m_VMarkInfo;

	//arrow btn state
	BOOL m_bArrowFrm, m_bArrowLeft, m_bArrowRight;
	BOOL m_bColumnFrm;
};

#endif
