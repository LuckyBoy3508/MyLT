
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
		"��ʷ�����ı��ļ� (*.txt)|*.txt||",
		"��ʷ�����ļ� (*.xml)|*.xml||",
		"��ѡ���� (*.txt)|*.txt||",
		"������־ (*.txt)|*.txt||",
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
		"����", "�Ϻ�", "���", "����",
		"����", "����", "������",
		"ɽ��", "ɽ��", "����", "�ӱ�",
		"����", "����", "�㶫", "����",
		"����", "�㽭", "����", "����",
		"����", "����", "�ຣ", 
		"����", "�Ĵ�", "����", "����",
		"�½�", "����", "����", "����",
	};

	return pProvince[lProvince];
}

long CGloablData::GetProvinceNum()
{
	static char *pProvince[] = 
	{
		"����", "�Ϻ�", "���", "����",
		"����", "����", "������",
		"ɽ��", "ɽ��", "����", "�ӱ�",
		"����", "����", "�㶫", "����",
		"����", "�㽭", "����", "����",
		"����", "����", "�ຣ", 
		"����", "�Ĵ�", "����", "����",
		"�½�", "����", "����", "����",
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

	

