
#ifndef __STATHLEPER_H
#define __STATHLEPER_H

//each period stat, use for(analyse state view & stat form view)
struct LTSH_ETCHPERIOD_RATE
{
	long lPeriod;						//Total period ID
	long lUnSortProb[LT_MAX_SELFROM];	//Use No sequence.
	long lSortedProb[LT_MAX_SELFROM];	//Percent * 10000 + NO.  eg. (7.56%, NO.12 = 075612)
	long lLayer[3];						//Hot, Middle, Cold	
};

typedef int (*LTSH_CMP_FUNC)(const void *, const void *);

//normal state function(calculate average value)
struct LTSH_NORMALSTAT_RET
{
	long *plData;
	long lNum;
	long lAllowMix, lAllowMax;
	long lDnPercent, lUpPercent;

	float fAve;	
	float fAveUp;
	float fAveDown;
	float fRetUp;
	float fRetDown;
	long lRetUp;
	long lRetDown;
};

//zero zone stat infomation
struct LTSH_ZONESTAT_RET
{
	long lHEmptyZoneMin, lHEmptyZoneMax;	//horizontal min/max empty zone number.
	long lVEmptyZoneMin, lVEmptyZoneMax;	//vertical min/max empty zone number.
	long lHZoneMaxOutMin, lHZoneMaxOutMax;	//horizontal zone min/max out number
	long lVZoneMaxOutMin, lVZoneMaxOutMax;	//vertical zone min/max out number
};

class IFilterDrawCtrl;
class CStatHelper
{
public:
	CStatHelper();
	virtual ~CStatHelper();

public:
	void SetData(LT_DATA_INFO *pData, long lDataNum, long lSelFrom, long lSelFor, BOOL bHaveSpec);
	LTSH_ETCHPERIOD_RATE *GetProbability(long &lNum);	

	//|HZone number|HZone zeor zone number|HZone max out number|
	//|VZone number|VZone zeor zone number|VZone max out number|
	void GetDataZoneInfo(long lHistData[LT_MAX_SELFOR], long lRet[LT_MAX_SELFROM]);	
	void SetZoneStatRet(LTSH_ZONESTAT_RET *pZoneStatRet);
	LTSH_ZONESTAT_RET *GetZoneStatRet();

	//helper function.
	static void NormalStat(LTSH_NORMALSTAT_RET *pNormalStat);
	
public:
	IFilterDrawCtrl *m_pFilters[20];

	//data
	LT_DATA_INFO *m_pDataInfo;
	long m_lDataNum;
	long m_lSelFrom, m_lSelFor;
	BOOL m_bHaveSpec;

private:
	//do propbability.
	void DoProb();
	void DoEachProb(LT_DATA_INFO *pData, long lNum, long *plEachProb);
	void GetProbLayer(long *plEachProb, LT_DATA_INFO *pRealData, long lLayer[3]);
	static int MyCmpForRate(long *pl1,long *pl2);

	//state calculate function.	
	LTSH_ETCHPERIOD_RATE *m_pEtchPeriodRate;
	long m_lEtchPeriodRateNum;

	//zone stat info
	LTSH_ZONESTAT_RET m_ZoneStatRet;
};

extern CStatHelper g_StatHelper;

#endif