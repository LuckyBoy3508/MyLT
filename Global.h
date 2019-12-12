
#ifndef __LTGLOBAL_H
#define __LTGLOBAL_H

#include "LTDef.h"
#include "DrawRes.h"

//global class
class CGloablData
{
public:
	CGloablData();
	~CGloablData();

public:
	//r-w config
	void RWCfg(BOOL bIsRead);

	//open file fillter
	enum GD_OF_FILTER
	{
		LOF_HIST_TEXT = 0,
		LOF_HIST_XML,
		LOF_USERGOT,
		LOF_LOG,
	};
	LPCTSTR GetFilter(GD_OF_FILTER eType);

	//number in zone
	void GetZoneInfo(long lSelFrom, long &lHZoneNum, long &lVZoneNum);

	//province info
	LPCTSTR GetProvinceName(long lProvince);
	long GetProvinceNum();

	//gdi info
	COLORREF GetColor(CDrawRes::CLR_STYLE eColor);
	CFont *GetFont(CDrawRes::FONT_STYLE eFont);
	CPen *GetPen(CDrawRes::PEN_STYLE ePen);
	CBrush *GetBrush(CDrawRes::BRS_STYLE eBrush);	

	//set gdi info
	void DrawResSetUp();

private:
	CDrawRes m_DrawRes;
};

extern CGloablData g_DataInfo;

#endif