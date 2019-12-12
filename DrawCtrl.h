
#ifndef _DRAWCTRL_H
#define _DRAWCTRL_H

#define LTDC_CORNER_VALUE		10

/////////////////////////////////////////////////////////////////////////////////
////IDrawCtrl
class IDrawCtrl
{
public:
	IDrawCtrl();
	virtual ~IDrawCtrl();

public:
	//property
	virtual char *GetName();

	//config(all data init at here)
	virtual BOOL RWCfg(BOOL bIsRead);

	//data	
	virtual void SetData(LT_DATA_INFO *pData, long lDataNum, 
					long lSelFrom, long lSelFor, BOOL bHaveSpec);
	virtual void SetParam(RECT *pRect);

	//oper
	virtual void SetScrollPos(long lStartPos, long lCurrPos);
	virtual void GetObjState(BOOL bBtns[6], long &lStartPos, long &lCurrPos, 
					long &lOnePageNum, long &lScrollRange, long &lTotalNum);
	virtual BOOL OnKeyDown(UINT nChar, BOOL bIsCtrlKeyDown);
	virtual BOOL OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL OnMouseMove(UINT nFlags, POINT point);

	//draw lShowType:0:Gray 1:Active 2:Mouse Hover.
	virtual void Draw(HDC hDC, long lShowType);

	//command
	virtual CMenu *GetPopupMenu(CMenu *pSrcMenu, UINT nStartMenuID);
	virtual void OnCommand(UINT uCmdID);

public:
	//helper function.
	static BOOL AdjustStartPosByCurrPos(long lNewCurrPos, 
					long &lOldCurrPos, long &lStartPos, long lOnePageNum, long lTotalNum);
	void ShowText(LPCTSTR lpszText, RECT *pRect, COLORREF clrText, long lPos, long lFontIndex);
	BOOL IsHaveDrawData();

protected:
	//name
	char m_szName[LT_MAX_FILENAMELEN + 1];
	
	//data
	LT_DATA_INFO *m_pDataInfo;
	long m_lDataNum;
	long m_lSelFrom, m_lSelFor;
	BOOL m_bHaveSpec;

	//draw
	CRect m_rtShow;
	CDC m_MemDC;
	
	//ctrl
	long m_lStartPos;
	long m_lCurrPos;
	long m_lOnePageNum;
	long m_lScrollRange;
	long m_lTotalNum;
	long m_lCellWidth;
};

/////////////////////////////////////////////////////////////////////////////////
////IAnalyseDrawCtrl
class IAnalyseDrawCtrl : public IDrawCtrl
{
public:
	IAnalyseDrawCtrl();
	virtual ~IAnalyseDrawCtrl();

public:
	//data	
	virtual void SetData(LT_DATA_INFO *pData, long lDataNum, 
						long lSelFrom, long lSelFor, BOOL bHaveSpec);
	virtual void SetParam(RECT *pRect);

	//oper
	virtual void SetScrollPos(long lStartPos, long lCurrPos);
	virtual void GetObjState(BOOL bBtns[6], long &lStartPos, long &lCurrPos, 
					long &lOnePageNum, long &lScrollRange, long &lTotalNum);
	virtual BOOL OnKeyDown(UINT nChar, BOOL bIsCtrlKeyDown);
};

//same range analyse struct.
#define LTDC_DWRGNGRID_ROW		10
#define LTDC_DWRGNGRID_COL		10

struct LTDC_SAMERGN_INFO
{
	char szName[LT_MAX_FILENAMELEN + 1];
	long lRgnMap[LTDC_DWRGNGRID_ROW][LTDC_DWRGNGRID_COL];

	BOOL b90, b180, b270, bHMirror, bVMirror;
};
typedef CList<LTDC_SAMERGN_INFO *, LTDC_SAMERGN_INFO *> SAMERGNLIST;

#define LTDC_DWPERIOD_WIDTH		2
#define LTDC_DWTITLE_HEIGHT		2

class CMainAnalyse : public IAnalyseDrawCtrl
{
public:
	CMainAnalyse();
	virtual ~CMainAnalyse();

public:
	//config
	virtual BOOL RWCfg(BOOL bIsRead);

	//data
	virtual void SetData(LT_DATA_INFO *pData, long lDataNum, 
						long lSelFrom, long lSelFor, BOOL bHaveSpec);
	virtual void SetParam(RECT *pRect);

	//oper
	virtual BOOL OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL OnMouseMove(UINT nFlags, POINT point);

	//draw
	virtual void Draw(HDC hDC, long lShowType);

	//command
	virtual CMenu *GetPopupMenu(CMenu *pSrcMenu, UINT nStartMenuID);
	virtual void OnCommand(UINT uCmdID);

private:
	struct LTDC_MANO
	{
		long lNo;
		BOOL bIsSpec;
		BOOL bIsLink;
	};

	struct LTDC_MADATA
	{
		long lPeriod;
		LTDC_MANO eachData[LT_MAX_SELFOR];
	};

	struct LTDC_MACELL
	{
		POINT ptCenter;
	};

	struct LTDC_MALINE
	{
		POINT ptStartPos, ptEndPos;
		BOOL bIsWillLine;
		BOOL bIsHotLine;

		BOOL bCanShow;
		POINT ptStart, ptEnd;
	};

	struct LTDC_DWRGN_TEMPL
	{
		BOOL bFillArray[LTDC_DWRGNGRID_ROW][LTDC_DWRGNGRID_COL];
		long lMapRow, lMapCol;
	};

	struct LTDC_MARGN
	{
		LTDC_DWRGN_TEMPL rgnTemplet;

		long *plData;
		long lDataNum;
		COLORREF clrRgn;
	};
	typedef CList<LTDC_MARGN *, LTDC_MARGN *> MAGRNLIST;

	//read/write config
	void RWCfgSameRgn(DVXML_NODE *pSameRgnNode, BOOL bIsRead);
	void RWCfgSameRgnEachRow(DVXML_NODE *pRgnRowNode, 
							   LTDC_SAMERGN_INFO *pSameRgn, BOOL bIsRead);

	//size
	void CalcPanelsRect();
	//0:wk 1:wk-show 2:title 3:title-show 4:name 5:value 6:showline 7:showrgn
	CRect m_rtPanel[8]; 

	//data
	void InitDrawData();

	//hot-cell-link property
	void GetDWLinKArray();
	void GetHLink();
	void GetVLine();
	LTDC_MADATA *m_pDWData;
	long m_lDWDataNum;

	//all cell
	void GetDWCellArray();
	LTDC_MACELL *m_pDWCell;
	long m_lDWCellNum;

	//cal link-line
	void GetDWLineArray();
	long CalTotalLine(BOOL bOnlyCalNum);
	long CalHaveLine(BOOL bOnlyCalNum);	
	long CalLineAtPoint(long lStartPeriod, long lNo, BOOL bIsSpec, BOOL bOnlyCalNum);
	BOOL CanMakeLine(long lStartPeriod, long lNo, BOOL bIsSpec, long lXOffSet, 
												long lYOffSetBOOL, BOOL &bIsHotLine);					
	void MakeHotNum(long lStartPeriod, long lNo, long lXOffSet, long lYOffSet);
	BOOL HaveThisNo(long lPeriod, long lNo, BOOL &bIsSpec);
	long CalWillLine(BOOL bOnlyCalNum);
	long CalWillLineAtPoint(long lStartPeriod, long lNo, BOOL bIsSpec, BOOL bOnlyCalNum);
	BOOL CanMakeWillLine(long lStartPeriod, long lNo, BOOL bIsSpec, long lXOffSet, 
												long lYOffSet);
	LTDC_MALINE *m_pDWLine, *m_pDWLineTemp;
	long m_lDWLineNum;	
	BOOL m_bShowLinkLine;

	//came range
	void GetDWRgnArray();
	LTDC_SAMERGN_INFO *GetCurrSelRgnTmpl();
	void GetDWRgnTemplet(LTDC_SAMERGN_INFO *pSameRgn);
	void FreeRgnDataMem();
	void GetDWRgnTemplRotateRet(LTDC_DWRGN_TEMPL &rgnInput, long lType, 
											LTDC_DWRGN_TEMPL &rgnRet);
	void GetDWRgnTemplRotate90Degree(LTDC_DWRGN_TEMPL &rgnInput, LTDC_DWRGN_TEMPL &rgnRet);	
	void GetDWRgnMap();
	BOOL GetDWRgnEachArea(long lRow, long lCol, LTDC_DWRGN_TEMPL *pTemplet,								
							BOOL bRgnVal[LTDC_DWRGNGRID_ROW][LTDC_DWRGNGRID_COL]);
	BOOL GetDWRgnCompTempet(LTDC_DWRGN_TEMPL *pTemplet, 
							BOOL bRgnVal[LTDC_DWRGNGRID_ROW][LTDC_DWRGNGRID_COL]);
	void GetDWRgnPutToRet(long lRow, long lCol, LTDC_MARGN *pRgn);
	SAMERGNLIST m_lstSameRgn;
	MAGRNLIST m_lstRgnData;
	long m_lCurrSameRgn;
	BOOL m_bShowSameRgn;

	//draw
	BOOL DrawOutlook();
	void DrawContent();
	void DrawCursor();
	void DrawValue();

	BOOL IsLineCanShow(LTDC_MALINE *pLine);
	void FillLinePoint(LTDC_MALINE * pLine);
	BOOL m_bDrawShowLine, m_bDrawShowRange;
	BOOL DrawShowLineFlag(BOOL bTest = FALSE, POINT *pPoint = NULL);
	BOOL DrawShowRangeFlag(BOOL bTest = FALSE, POINT *pPoint = NULL);
};

#define LTDC_ZONEPERIOD_WIDTH	1
#define LTDC_ZONETITLE_HEIGHT	2

class CZoneAnalyse : public IAnalyseDrawCtrl
{
public:
	CZoneAnalyse();
	virtual ~CZoneAnalyse();

public:
	//data
	virtual void SetData(LT_DATA_INFO *pData, long lDataNum, 
						long lSelFrom, long lSelFor, BOOL bHaveSpec);
	virtual void SetParam(RECT *pRect);

	//oper
	virtual BOOL OnLButtonDown(UINT nFlags, POINT point);

	//draw
	virtual void Draw(HDC hDC, long lShowType);

private:
	struct LTDC_ZONE_DATA
	{
		long lPeriod;
		long lData[LT_MAX_SELFROM];		//|HZone number|HZone zeor zone number|HZone max out number|
										//|VZone number|VZone zeor zone number|VZone max out number|				
	};

	struct LTDC_ZONE_INFO
	{
		LTDC_ZONE_DATA *pZoneData;
		long lZoneNum;

		float fAveVal[LT_MAX_SELFROM];
		long lRetVal[LT_MAX_SELFROM][2];
	};

	//size
	void CalcPanelsRect();
	long m_lCellHeight;
	long m_lHZoneNum, m_lVZoneNum;
	//0:wk 1:wk-show 2:title 3:title-show 4:name 5:value 6:showline 7:showrgn
	CRect m_rtPanel[8];
	
	//data
	void InitDrawData();
	void GetDrawData();
	LTDC_ZONE_INFO m_ZoneInfo;

	//draw
	BOOL DrawOutlook();
	void DrawContent();
	void DrawCursor();
	void DrawValue();
};

/////////////////////////////////////////////////////////////////////////////////
////IStatDrawCtrl
class IStatDrawCtrl : public IDrawCtrl
{
public:
	IStatDrawCtrl();
	virtual ~IStatDrawCtrl();

public:
	//config.
	BOOL RWCfg(BOOL bIsRead);

	//data	
	virtual void SetData(LT_DATA_INFO *pData, long lDataNum, 
						long lSelFrom, long lSelFor, BOOL bHaveSpec);
	virtual void SetParam(RECT *pRect);

	//oper
	virtual void SetScrollPos(long lStartPos, long lCurrPos);
	virtual void GetObjState(BOOL bBtns[6], long &lObjStartPos, long &lObjCurrPos, 
					long &lObjOnePageNum, long &lObjScrollRange, long &lObjTotalNum);
	virtual BOOL OnKeyDown(UINT nChar, BOOL bIsCtrlKeyDown);
	virtual BOOL OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL OnMouseMove(UINT nFlags, POINT point);

	//draw
	virtual void Draw(HDC hDC, long lShowType);

protected:
	void FreeDataInfo();
	DVCT_BASE_INFO m_BaseInfo;
	DVCT_DATA_INFO m_DataInfo;
	CDvChart m_DvChart;

	BOOL m_bIsColumn;
	long m_lMinCellWidth, m_lMaxCellWidth;
};

class CProbStat : public IStatDrawCtrl
{
public:
	CProbStat();
	virtual ~CProbStat();

public:
	//data
	virtual void SetData(LT_DATA_INFO *pData, long lDataNum, 
						long lSelFrom, long lSelFor, BOOL bHaveSpec);
};

class CThreeNoStat : public IStatDrawCtrl
{
public:
	CThreeNoStat();
	virtual ~CThreeNoStat();

public:
	//data
	virtual void SetData(LT_DATA_INFO *pData, long lDataNum, 
						long lSelFrom, long lSelFor, BOOL bHaveSpec);
};

/////////////////////////////////////////////////////////////////////////////////
////IFilterDrawCtrl
class IFilterDrawCtrl : public IDrawCtrl
{
public:
	//constructor
	IFilterDrawCtrl();
	virtual ~IFilterDrawCtrl();

public:
	//red config.
	virtual BOOL RWCfg(BOOL bIsRead);

	//data	
	virtual void SetData(LT_DATA_INFO *pData, long lDataNum, 
						long lSelFrom, long lSelFor, BOOL bHaveSpec);
	virtual void SetParam(RECT *pRect);

	//oper
	virtual void SetScrollPos(long lStartPos, long lCurrPos);
	virtual void GetObjState(BOOL bBtns[6], long &lObjStartPos, long &lObjCurrPos, 
					long &lObjOnePageNum, long &lObjScrollRange, long &lObjTotalNum);
	virtual BOOL OnKeyDown(UINT nChar, BOOL bIsCtrlKeyDown);
	virtual BOOL OnLButtonDown(UINT nFlags, POINT point);
	virtual BOOL OnMouseMove(UINT nFlags, POINT point);

	//draw
	virtual void Draw(HDC hDC, long lShowType);

public:
	virtual void GetMinMaxVal(long &lMin, long &lMax) = 0;
	virtual void GetFinalRet(long &lStartVal, long &lEndVal);
	virtual BOOL IsValidate(long lData[LT_MAX_SELFOR], long lStartVal, long lEndVal);

protected:	
	virtual void GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
					LT_DATA_INFO *pCurrDataInfo, long *pRetVal) = 0;

private:
	struct LTDC_FILTERSTAT_RET
	{
		long *plData;
		long lNum;

		float fAve;
		float fAveDown;
		float fAveUp;
		float fRetDown;
		float fRetUp;
		long lRetDown;
		long lRetUp;
	};

	void GetNormalVal();
	void CalNormalRet();
	void SetNormalRet();

	LTDC_FILTERSTAT_RET m_FilterRetInfo;

	void FreeDataInfo();
	DVCT_BASE_INFO m_BaseInfo;
	DVCT_DATA_INFO m_DataInfo;
	CDvChart m_DvChart;

	BOOL m_bIsColumn;
	long m_lMinCellWidth, m_lMaxCellWidth;
};

class CSumFilter : public IFilterDrawCtrl
{
public:
	CSumFilter();
	virtual ~CSumFilter();

public:
	virtual void GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
				LT_DATA_INFO *pCurrDataInfo, long *pRetVal);
	virtual void GetMinMaxVal(long &lMin, long &lMax);
};

class CSingleFilter : public IFilterDrawCtrl
{
public:
	CSingleFilter();
	virtual ~CSingleFilter();

public:
	virtual void GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
				LT_DATA_INFO *pCurrDataInfo, long *pRetVal);
	virtual void GetMinMaxVal(long &lMin, long &lMax);
};

class COddFilter : public IFilterDrawCtrl
{
public:
	COddFilter();
	virtual ~COddFilter();

public:
	virtual void GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
				LT_DATA_INFO *pCurrDataInfo, long *pRetVal);
	virtual void GetMinMaxVal(long &lMin, long &lMax);
};

class CBrotherFilter : public IFilterDrawCtrl
{
public:
	CBrotherFilter();
	virtual ~CBrotherFilter();

public:
	virtual void GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
				LT_DATA_INFO *pCurrDataInfo, long *pRetVal);
	virtual void GetMinMaxVal(long &lMin, long &lMax);
};

class CRepeatFilter : public IFilterDrawCtrl
{
public:
	CRepeatFilter();
	virtual ~CRepeatFilter();

public:
	virtual void GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
				LT_DATA_INFO *pCurrDataInfo, long *pRetVal);
	virtual void GetMinMaxVal(long &lMin, long &lMax);
};

class CSameTailFilter : public IFilterDrawCtrl
{
public:
	CSameTailFilter();
	virtual ~CSameTailFilter();

public:
	virtual void GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
				LT_DATA_INFO *pCurrDataInfo, long *pRetVal);
	virtual void GetMinMaxVal(long &lMin, long &lMax);
};

class CSeparateFilter : public IFilterDrawCtrl
{
public:
	CSeparateFilter();
	virtual ~CSeparateFilter();

public:
	virtual void GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
				LT_DATA_INFO *pCurrDataInfo, long *pRetVal);
	virtual void GetMinMaxVal(long &lMin, long &lMax);
};

class CRollFilter : public IFilterDrawCtrl
{
public:
	CRollFilter();
	virtual ~CRollFilter();

public:
	virtual void GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
				LT_DATA_INFO *pCurrDataInfo, long *pRetVal);
	virtual void GetMinMaxVal(long &lMin, long &lMax);
};

class CDistanceFilter : public IFilterDrawCtrl
{
public:
	CDistanceFilter();
	virtual ~CDistanceFilter();

public:
	virtual void GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
				LT_DATA_INFO *pCurrDataInfo, long *pRetVal);
	virtual void GetMinMaxVal(long &lMin, long &lMax);
};

class CACFilter : public IFilterDrawCtrl
{
public:
	CACFilter();
	virtual ~CACFilter();

public:
	virtual void GetEachNormalVal(LT_DATA_INFO *pLastDataInfo, 
				LT_DATA_INFO *pCurrDataInfo, long *pRetVal);
	virtual void GetMinMaxVal(long &lMin, long &lMax);
};

/////////////////////////////////////////////////////////////////////////////////
///ctrl number global
//Global only one instance define.
//analyse ctrl
extern long g_lAnalyseCtrlNum;
extern CMainAnalyse g_MainAnalyse;
extern CZoneAnalyse g_ZoneAnalyse;

//stat ctrl
extern long g_lStateCtrlNum;
extern CProbStat g_ProbStat;
extern CThreeNoStat g_ThreeNoStat;

//filter ctrl
extern long g_lFilterCtrlNum;
extern CSumFilter g_SumFilter;
extern CSingleFilter g_SingleFilter;
extern COddFilter g_OddFilter;
extern CBrotherFilter g_BrotherFilter;
extern CRepeatFilter g_RepeatFilter;
extern CSameTailFilter g_SameTailFilter;
extern CSeparateFilter g_SeparateFilter;
extern CRollFilter g_RollFilter;
extern CDistanceFilter g_DistanceFilter;
extern CACFilter g_ACFilter;

/////////////////////////////////////////////////////////////////////////////////
///CDrawProxy
class CDrawProxy
{
public:
	CDrawProxy();
	~CDrawProxy();

public:
	//get property
	char *GetCtrlName(long lCtrl);
	char *GetObjName(long lCtrl, long lObj);
	long GetObjNum(long lCtrl);
	void SetCurrCtrlObj(long lCurrCtrl, long lCurrObj);

	//read config file
	BOOL RWCfg(BOOL bIsRead = TRUE);

	//draw data
	void SetData(LT_DATA_INFO *pData, long lDataNum, 
				long lSelFrom, long lSelFor, BOOL bHaveSpec);
	void SetParam(RECT *pRect);

	//draw ctrl
	void SetScrollPos(long lType, long lStartPos, long lCurrPos);
	void GetCtrlState(long &lCtrlStartPos, long &lCtrlCurrPos, long &lCtrlOnePageNum, 
					long &lCtrlScrollRange, long &lCtrlTotalNum);
	void GetObjState(BOOL bBtns[6], long &lObjStartPos, long &lObjCurrPos, long &lObjOnePageNum, 
					long &lObjScrollRange, long &lObjTotalNum);

	BOOL OnKeyDown(UINT nChar, BOOL bIsCtrlKeyDown);
	BOOL OnLButtonDown(UINT nFlags, POINT point);
	BOOL OnMouseMove(UINT nFlags, POINT point);

	//draw
	void Draw(HDC hDC);

	//command
	CMenu *GetPopupMenu(CMenu *pSrcMenu, UINT nStartMenuID);
	void OnCommand(UINT uCmdID);

public:
	long m_lCurrCtrl;

private:
	struct LTDC_DRAWCTRL_INFO
	{
		char szName[LT_MAX_FILENAMELEN + 1];
		IDrawCtrl *pObj[20];

		long lStartPos;
		long lCurrPos;
		long lOnePageNum;
		long lTotalNum;
		long lScrollRange;

		long lPanelSpace;
		RECT m_PanelRect[5];
	};	
	LTDC_DRAWCTRL_INFO m_DrawCtrlInfo[3];
	long m_lMouseHoverPos;
	long m_lShowType;

	CRect m_rtShow;
	void DrawFrame(HDC hDC);
	long GetShowNum(long lCurrCtrl);
	void SetObjParam(long lCurrCtrl);	
};

#endif