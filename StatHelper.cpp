
#include "stdafx.h"
#include "mylt.h"
#include "StatHelper.h"
#include "DrawCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////
//CStatHelper
CStatHelper g_StatHelper;

CStatHelper::CStatHelper()
{
	m_pFilters[0] = &g_SumFilter;
	m_pFilters[1] = &g_SingleFilter;
	m_pFilters[2] = &g_OddFilter;
	m_pFilters[3] = &g_BrotherFilter;
	m_pFilters[4] = &g_RepeatFilter;
	m_pFilters[5] = &g_SameTailFilter;
	m_pFilters[6] = &g_SeparateFilter;
	m_pFilters[7] = &g_RollFilter;
	m_pFilters[8] = &g_DistanceFilter;
	m_pFilters[9] = &g_ACFilter;

	m_pDataInfo = NULL;
	m_lDataNum = 0;
	m_lSelFrom = m_lSelFor = 0;
	m_bHaveSpec = FALSE;

	m_pEtchPeriodRate = NULL;
	m_lEtchPeriodRateNum = 0;

	memset(&m_ZoneStatRet, 0, sizeof(LTSH_ZONESTAT_RET));
}

CStatHelper::~CStatHelper()
{
	if (m_pEtchPeriodRate)
		delete m_pEtchPeriodRate;
}

void CStatHelper::SetData(LT_DATA_INFO *pData, long lDataNum, long lSelFrom, long lSelFor, BOOL bHaveSpec)
{
	m_pDataInfo = pData;
	m_lDataNum = lDataNum;
	m_lSelFrom = lSelFrom;
	m_lSelFor = lSelFor;
	m_bHaveSpec = bHaveSpec;
	
	DoProb();
}

void CStatHelper::DoProb()
{
	//free old mem
	if (m_pEtchPeriodRate)
	{
		delete m_pEtchPeriodRate;
		m_pEtchPeriodRate = NULL;
	}
	m_lEtchPeriodRateNum = 0;
	
	//cal new data
	if (m_pDataInfo && m_lDataNum > 1)
	{
		long lStartPos, lRepeatNum;
		lStartPos = lRepeatNum = 0;

		//1. get repeat number.
		{
			if (m_lDataNum >= 10 * m_lSelFrom)
			{
				lStartPos = m_lDataNum - 9 * m_lSelFrom;
				lRepeatNum = m_lSelFrom;
			}
			else if (m_lDataNum >= 2 * m_lSelFrom)
			{
				lStartPos = m_lSelFrom;
				lRepeatNum = m_lSelFrom;
			}
			else
			{
				lStartPos = m_lDataNum / 2;
				lRepeatNum = lStartPos;
			}
		}

		//2. calcu each period rate.
		{
			long lDataNum = m_lDataNum - lStartPos;
			LTSH_ETCHPERIOD_RATE *pHist = new LTSH_ETCHPERIOD_RATE[lDataNum];
			memset(pHist, 0, sizeof(LTSH_ETCHPERIOD_RATE) * lDataNum);
			LTSH_ETCHPERIOD_RATE *pHistTemp = pHist;
			for (long a = lStartPos; a < m_lDataNum; a++, pHistTemp++)
			{
				pHistTemp->lPeriod = a + 1;

				//get unsort data.
				LT_DATA_INFO *pStartData = m_pDataInfo + a - lRepeatNum;
				DoEachProb(pStartData, lRepeatNum, pHistTemp->lUnSortProb);
				
				//get sort data
				memcpy(pHistTemp->lSortedProb, pHistTemp->lUnSortProb, sizeof(long) * m_lSelFrom);
				qsort(pHistTemp->lSortedProb, m_lSelFrom, sizeof(long), (LTSH_CMP_FUNC)MyCmpForRate);

				//get layer data
				GetProbLayer(pHistTemp->lSortedProb, m_pDataInfo + a, pHistTemp->lLayer);
			}

			m_pEtchPeriodRate = pHist;
			m_lEtchPeriodRateNum = lDataNum;
		}
	}
}

void CStatHelper::DoEachProb(LT_DATA_INFO *pData, long lNum, long *plEachProb)
{
	long a, b, lRow, lCol, lQZ;

	long *plRet = plEachProb;
	memset(plRet, 0, sizeof(long) * m_lSelFrom);

	//lRow = m_lSelFrom, and lCol = lNum
	char *pszQZ = new char[m_lSelFrom * lNum];
	memset(pszQZ, 0, m_lSelFrom * lNum);

	LT_DATA_INFO *pDataTemp = pData;
	for (a = 0; a < lNum; a++, pDataTemp++)
	{
		for (b = 0; b < m_lSelFor; b++)
		{
			lRow = pDataTemp->lData[b] - 1;
			lCol = a;

			ASSERT(lRow >= 0 && lRow < m_lSelFrom);
			ASSERT(lCol >=0 && lCol < lNum);

			pszQZ[lRow * lNum + lCol] = 1;
		}
	}

	long lTotalNoNum = lNum * m_lSelFor;
	long *plRetTemp = plRet;

	for (a = 0; a < m_lSelFrom; a++, plRetTemp++)
	{
		lQZ = 0;

		for (b = 0; b < lNum; b++)
		{
			if (pszQZ[a * lNum + b] == 1)
			{
				//lQZ += b + 1; //加权平均法求概率,求每一个号码的出现概率
				lQZ++; //平均概率
			}
		}

		//值格式: 万分率 * 100 + 号码
		long lPercent = lQZ * 10000 / lTotalNoNum;
		*plRetTemp = lPercent * 100 + a + 1;
	}

	delete pszQZ;
}

void CStatHelper::GetProbLayer(long *plEachProb, LT_DATA_INFO *pRealData, long lLayer[3])
{
	long lA, lB, lC;
	lA = lB = lC = 0;
	
	for (long a = 0; a < m_lSelFor; a++)
	{
		for (long b = 0; b < m_lSelFrom; b++)
		{
			if (pRealData->lData[a] == (plEachProb[b] % 100))
			{				
				if (b < m_lSelFor) lA++;
				else if (b >= m_lSelFrom - m_lSelFor) lC++;
				else lB++;
				
				break;
			}
		}
	}

	lLayer[0] = lA;
	lLayer[1] = lB;
	lLayer[2] = lC;
}

int CStatHelper::MyCmpForRate(long *pl1, long *pl2)
{
	return ((*pl2) / 100 - (*pl1) / 100);
}

LTSH_ETCHPERIOD_RATE *CStatHelper::GetProbability(long &lNum)
{
	lNum = m_lEtchPeriodRateNum;
	return m_pEtchPeriodRate;
}

void CStatHelper::NormalStat(LTSH_NORMALSTAT_RET *pNormalStat)
{
	if (pNormalStat)
	{
		double dAve, dAveUp, dAveDn, dRetUp, dRetDn, dUpPercent, dDnPercent;
		dAve = dAveUp = dAveDn = dRetUp = dRetDn = dUpPercent = dDnPercent  = 0.0;

		long a, lTotal, lRetUp, lRetDn, lUpNum, lDnNum, lMax, lMin;
		lTotal = lRetUp = lRetDn = lUpNum = lDnNum = lMax = lMin = 0;

		long *plHistTemp = pNormalStat->plData;
		long lNum = pNormalStat->lNum;
		lMax = pNormalStat->lAllowMax;
		lMin = pNormalStat->lAllowMix;
		dUpPercent = (pNormalStat->lUpPercent == 0) ? 1.0 : (pNormalStat->lUpPercent * 0.01);
		dDnPercent = (pNormalStat->lDnPercent == 0) ? 1.0 : (pNormalStat->lDnPercent * 0.01);
		
		if (lNum > 0)
		{
			for(a = 0; a < lNum; a++, plHistTemp++)
			{
				lTotal += *plHistTemp;

				if(a)
				{
					long lDelta = *plHistTemp - *(plHistTemp-1);

					if(lDelta > 0)
					{
						lRetUp += lDelta;
						lUpNum++;
					}
					else if(lDelta < 0)
					{
						lRetDn -= lDelta;
						lDnNum++;
					}
				}
			}

			dAve = lTotal * 1.0 / lNum;
			if (lUpNum) dAveUp = lRetUp * 1.0 / lUpNum;
			if (lDnNum) dAveDn = lRetDn * 1.0 / lDnNum;
			dRetUp = (dAve + dAveUp * dUpPercent) > lMax ? lMax : (dAve + dAveUp * dUpPercent);
			dRetDn = (dAve - dAveDn * dDnPercent) < lMin ? lMin : (dAve - dAveDn * dDnPercent);
			lRetUp = (long)(dRetUp + 0.5);
			lRetDn = (long)(dRetDn + 0.5);
		}
			
		pNormalStat->fAve = (float)dAve;
		pNormalStat->fAveDown = (float)dAveDn;
		pNormalStat->fAveUp = (float)dAveUp;
		pNormalStat->fRetDown = (float)dRetDn;
		pNormalStat->fRetUp = (float)dRetUp;
		pNormalStat->lRetDown = lRetDn;
		pNormalStat->lRetUp = lRetUp;
	}
}

void CStatHelper::GetDataZoneInfo(long lHistData[LT_MAX_SELFOR], long lRet[LT_MAX_SELFROM])
{
	long a, lHZoneNum, lVZoneNum, lHZZNum, lHMax, lVZZNum, lVMax;
	long lHZone[LT_MAX_SELFROM] = {0};
	long lVZone[LT_MAX_SELFROM] = {0};

	g_DataInfo.GetZoneInfo(m_lSelFrom, lHZoneNum, lVZoneNum);

	long *pData = lRet;
	//Horizontal zone info	
	memset(lHZone, 0, sizeof(long) * LT_MAX_SELFROM);
	lHZZNum = lHMax = 0;
	for (a = 0; a < m_lSelFor; a++)
	{
		lHZone[(lHistData[a] - 1) / lVZoneNum]++;
	}

	for (a = 0; a < lHZoneNum; a++)
	{
		if (lHZone[a] == 0)
			lHZZNum++;
		else if (lHZone[a] > lHMax)
			lHMax = lHZone[a];
	}

	memcpy(pData, lHZone, sizeof(long) * lHZoneNum);
	pData += lHZoneNum;

	*pData++ = lHZZNum;
	*pData++ = lHMax;

	//Vertical zone info
	memset(lVZone, 0, sizeof(long) * LT_MAX_SELFROM);
	lVZZNum = lVMax = 0;
	for (a = 0; a < m_lSelFor; a++)
	{
		lVZone[(lHistData[a] - 1) % lVZoneNum]++;
	}

	for (a = 0; a < lVZoneNum; a++)
	{
		if (lVZone[a] == 0)
			lVZZNum++;
		else if (lVZone[a] > lVMax)
			lVMax = lVZone[a];
	}

	memcpy(pData, lVZone, sizeof(long) * lVZoneNum);
	pData += lVZoneNum;

	*pData++ = lVZZNum;
	*pData++ = lVMax;
}

void CStatHelper::SetZoneStatRet(LTSH_ZONESTAT_RET *pZoneStatRet)
{
	memcpy(&m_ZoneStatRet, pZoneStatRet, sizeof(LTSH_ZONESTAT_RET));
}

LTSH_ZONESTAT_RET *CStatHelper::GetZoneStatRet()
{
	return &m_ZoneStatRet;
}