
#include "stdafx.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//global instance
CGloablData g_DataInfo;
////////////////////////////////////////////////////////////////
//
CGloablData::CGloablData()
{
	
}

CGloablData::~CGloablData()
{

}
	
void CGloablData::RWCfg(BOOL bIsRead)
{
	m_DrawRes.RWCfg(bIsRead);
}

LPCTSTR CGloablData::GetFilter(GD_OF_FILTER eType)
{
	static char *pFilterStr[] = 
	{
		"历史数据文本文件 (*.txt)|*.txt||",
		"历史数据文件 (*.xml)|*.xml||",
		"自选号码 (*.txt)|*.txt||",
		"操作日志 (*.txt)|*.txt||",
	};

	return pFilterStr[eType];
}

void CGloablData::GetZoneInfo(long lSelFrom, long &lHZoneNum, long &lVZoneNum)
{
	lHZoneNum = lVZoneNum = 0;

	static long lNumberInZone[LT_MAX_SELFROM - LT_MIN_SELFROM + 1] = 
	{
		//15
		3,
		//16~24
		4, 4, 4, 4, 4, 4, 4, 4, 4, 
		//25~35
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
		//36~37
		6, 6,
	};

	long lNoInZone = lNumberInZone[lSelFrom - LT_MIN_SELFROM];

	lHZoneNum = lSelFrom / lNoInZone;
	if (lSelFrom % lNoInZone)
		lHZoneNum++;

	lVZoneNum = lNoInZone;
}


LPCTSTR CGloablData::GetProvinceName(long lProvince)
{
	static char *pProvince[] = 
	{
		"北京", "上海", "天津", "重庆",
		"辽宁", "吉林", "黑龙江",
		"山东", "山西", "河南", "河北",
		"湖南", "湖北", "广东", "广西",
		"江苏", "浙江", "江西", "福建",
		"陕西", "甘肃", "青海", 
		"贵州", "四川", "云南", "海南",
		"新疆", "西藏", "内蒙", "宁夏",
	};

	return pProvince[lProvince];
}

long CGloablData::GetProvinceNum()
{
	static char *pProvince[] = 
	{
		"北京", "上海", "天津", "重庆",
		"辽宁", "吉林", "黑龙江",
		"山东", "山西", "河南", "河北",
		"湖南", "湖北", "广东", "广西",
		"江苏", "浙江", "江西", "福建",
		"陕西", "甘肃", "青海", 
		"贵州", "四川", "云南", "海南",
		"新疆", "西藏", "内蒙", "宁夏",
	};

	return (sizeof(pProvince) / sizeof(pProvince[0]));
}

COLORREF CGloablData::GetColor(CDrawRes::CLR_STYLE eColor)
{
	return m_DrawRes.GetColor(eColor);
}

CFont *CGloablData::GetFont(CDrawRes::FONT_STYLE eFont)
{
	return m_DrawRes.GetFont(eFont);
}

CPen *CGloablData::GetPen(CDrawRes::PEN_STYLE ePen)
{
	return m_DrawRes.GetPen(ePen);
}

CBrush *CGloablData::GetBrush(CDrawRes::BRS_STYLE eBrush)
{
	return m_DrawRes.GetBrush(eBrush);
}

void CGloablData::DrawResSetUp()
{
	m_DrawRes.SetUp();
}

	

