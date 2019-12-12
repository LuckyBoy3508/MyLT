// MyStatView.cpp : implementation file
//

#include "stdafx.h"
#include "MyLT.h"
#include "MyStatView.h"
#include "DrawCtrl.h"
#include "math.h"
#include "NOFilterCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyStatView

IMPLEMENT_DYNCREATE(CMyStatView, CFormView)

CMyStatView::CMyStatView()
	: CFormView(CMyStatView::IDD)
{
	//{{AFX_DATA_INIT(CMyStatView)
	m_strAHot = _T("");
	m_strAMiddle = _T("");
	m_strACold = _T("");
	m_bCMultiSel = FALSE;
	m_strCMiniMsg = _T("");
	//}}AFX_DATA_INIT

	memset(&m_LayerStatRet, 0, sizeof(LTSV_LAYERSTAT_RET));
	memset(&m_InputParam, 0, sizeof(LTSV_INPUTPARAM_INFO));

	m_pCalculateThread = NULL;
	m_bThreadDoing = m_bTherminateThread = FALSE;
	m_pGroup = NULL;
	m_lGroupNum = 0;
	memset(m_CalTotalRetInfo, 0, sizeof(LTSV_CALTOTALRET_INFO) * 70);
	m_lCalTotalRetInfoNum = 0;
	
	m_pGetNextRetThread = NULL;
	m_pWeGotInfo = NULL;
	m_lWeGotInfoNum = 0;
}

CMyStatView::~CMyStatView()
{
	if (m_pGroup)
		delete m_pGroup;

	FreeRetInfoMem();

	if (m_pWeGotInfo)
		delete m_pWeGotInfo;
}

void CMyStatView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyStatView)
	DDX_Control(pDX, IDC_A_CB_STATFROM, m_wndAStatFrom);
	DDX_Text(pDX, IDC_A_STC_HOT, m_strAHot);
	DDX_Text(pDX, IDC_A_STC_MIDDLE, m_strAMiddle);
	DDX_Text(pDX, IDC_A_STC_CODE, m_strACold);
	DDX_Control(pDX, IDC_A_CB_STATHISRET, m_wndAStatHisRet);
	DDX_Control(pDX, IDC_A_CB_AVERET, m_wndAAveRet);

	DDX_Control(pDX, IDC_B_CB_HF, m_wndBHF);
	DDX_Control(pDX, IDC_B_CB_HT, m_wndBHT);
	DDX_Control(pDX, IDC_B_CB_MF, m_wndBMF);
	DDX_Control(pDX, IDC_B_CB_MT, m_wndBMT);
	DDX_Control(pDX, IDC_B_CB_CF, m_wndBCF);
	DDX_Control(pDX, IDC_B_CB_CT, m_wndBCT);

	DDX_Control(pDX, IDC_B_STC_NOFILTER, m_wndBNOFilter);
	DDX_Control(pDX, IDC_B_CB_HZMIN, m_wndBHZMin);
	DDX_Control(pDX, IDC_B_CB_HZMAX, m_wndBHZMax);
	DDX_Control(pDX, IDC_B_CB_VZMIN, m_wndBVZMin);
	DDX_Control(pDX, IDC_B_CB_VZMAX, m_wndBVZMax);

	DDX_Control(pDX, IDC_B_CB_HMIN, m_wndBHMin);
	DDX_Control(pDX, IDC_B_CB_HMAX, m_wndBHMax);	
	DDX_Control(pDX, IDC_B_CB_VMIN, m_wndBVMin);
	DDX_Control(pDX, IDC_B_CB_VMAX, m_wndBVMax);
		
	DDX_Control(pDX, IDC_B_LST_FILTER, m_wndBFilter);
	DDX_Control(pDX, IDC_B_CB_FILTERF, m_wndBFilterFrom);
	DDX_Control(pDX, IDC_B_CB_FILTERT, m_wndBFilterTo);

	DDX_Control(pDX, IDC_C_LST_MSG, m_wndCMsg);
	DDX_Control(pDX, IDC_C_PRO_PROCESS, m_wndCProcess);
	DDX_Control(pDX, IDC_C_CB_PERIODNUM, m_wndCPeriodNum);	
	DDX_Check(pDX, IDC_C_CK_MULTISEL, m_bCMultiSel);
	DDX_Text(pDX, IDC_C_STC_MINIMSG, m_strCMiniMsg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyStatView, CFormView)
	//{{AFX_MSG_MAP(CMyStatView)
	ON_CBN_SELCHANGE(IDC_A_CB_STATFROM, OnSelchangeACbStatfrom)
	ON_LBN_SELCHANGE(IDC_B_LST_FILTER, OnSelchangeBLstFilter)
	ON_CBN_SELCHANGE(IDC_B_CB_FILTERF, OnSelchangeBCbFilterf)
	ON_CBN_SELCHANGE(IDC_B_CB_FILTERT, OnSelchangeBCbFiltert)
	ON_BN_CLICKED(IDC_C_BTN_GO, OnCBtnGo)
	ON_BN_CLICKED(IDC_C_BTN_STOPALL, OnCBtnStopall)	
	ON_BN_CLICKED(IDC_C_BTN_NEXTPERIOD, OnCBtnNextperiod)
	ON_BN_CLICKED(IDC_C_BTN_SAVENEXT, OnCBtnSavenext)	
	ON_BN_CLICKED(IDC_C_BTN_CLEAR, OnCBtnClear)
	ON_BN_CLICKED(IDC_C_BTN_SAVE, OnCBtnSave)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyStatView diagnostics

#ifdef _DEBUG
void CMyStatView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMyStatView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyStatView message handlers

void CMyStatView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class		
}

void CMyStatView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class	
	//1. set init value of gui
	DoInit();

	//2. set three layer data
	long lDataNum = 0;
	LTSH_ETCHPERIOD_RATE *pRate = g_StatHelper.GetProbability(lDataNum);
	if (lDataNum > 0)
	{
		//get three layer data
		GetLayerStatRet();

		//set to gui control items
		SetLayerStatRet();

		//set cur select stat period.
		m_wndAStatFrom.SetCurSel(lDataNum - 1);
		OnSelchangeACbStatfrom();		
	}

	//3. update controls
	UpdateCtlStat(lDataNum > 0 ? TRUE: FALSE);
}

void CMyStatView::DoInit()
{
	//1. empty control data.
	//A Zone init...
	m_wndAStatFrom.ResetContent();
	m_strAHot = "";
	m_strAMiddle = "";
	m_strACold = "";	
	m_wndAStatHisRet.ResetContent();
	m_wndAAveRet.ResetContent();

	//B Zone init...
	m_wndBHF.ResetContent();
	m_wndBHT.ResetContent();
	m_wndBMF.ResetContent();
	m_wndBMT.ResetContent();
	m_wndBCF.ResetContent();
	m_wndBCT.ResetContent();

	m_wndBHZMin.ResetContent();
	m_wndBHZMax.ResetContent();
	m_wndBVZMin.ResetContent();
	m_wndBVZMax.ResetContent();	
	m_wndBHMin.ResetContent();
	m_wndBHMax.ResetContent();
	m_wndBVMin.ResetContent();
	m_wndBVMax.ResetContent();

	m_wndBFilter.ResetContent();
	m_wndBFilterFrom.ResetContent();
	m_wndBFilterTo.ResetContent();

	//C Zone init...
	m_wndCMsg.ResetContent();
	m_wndCPeriodNum.ResetContent();
	((CButton *)GetDlgItem(IDC_C_CK_MULTISEL))->EnableWindow(g_StatHelper.m_bHaveSpec ? SW_SHOW : SW_HIDE);
	m_strCMiniMsg = "";	
	
	//2. set ctrl data range.
	CString strItem;
	long a;

	long lDataNum = 0;
	LTSH_ETCHPERIOD_RATE *pRate = g_StatHelper.GetProbability(lDataNum);
	if (lDataNum > 0)
	{
		//1. stat period
		for (a = 0; a < lDataNum; a++)
		{			
			strItem.Format("%d", (pRate + a)->lPeriod);
			m_wndAStatFrom.AddString(strItem);
		}

		//2. three layer
		long lMaxCalSel = g_StatHelper.m_lSelFor;
		if (lMaxCalSel > 6) lMaxCalSel = 6;
		for (a = 0; a <= lMaxCalSel; a++)
		{
			strItem.Format("%d", a);

			m_wndBHF.AddString(strItem);
			m_wndBHT.AddString(strItem);
			m_wndBMF.AddString(strItem);
			m_wndBMT.AddString(strItem);
			m_wndBCF.AddString(strItem);
			m_wndBCT.AddString(strItem);
		}

		//3. NO filter setting
		{
			long lHZoneNum, lVZoneNum;
			g_DataInfo.GetZoneInfo(g_StatHelper.m_lSelFrom, lHZoneNum, lVZoneNum);

			m_wndBNOFilter.SetParam(g_StatHelper.m_lSelFrom, 
									g_StatHelper.m_lSelFor, 
									lHZoneNum, lVZoneNum);

			
			for (a = 0; a < lHZoneNum; a++)
			{
				strItem.Format("%d", a);

				m_wndBHZMin.AddString(strItem);
				m_wndBHZMax.AddString(strItem);
			}

			for (a = 0; a < lVZoneNum; a++)
			{
				strItem.Format("%d", a);

				m_wndBVZMin.AddString(strItem);
				m_wndBVZMax.AddString(strItem);
			}

			for (a = 1; a <= g_StatHelper.m_lSelFor; a++)
			{
				strItem.Format("%d",a);

				m_wndBHMin.AddString(strItem);
				m_wndBHMax.AddString(strItem);
				m_wndBVMin.AddString(strItem);
				m_wndBVMax.AddString(strItem);
			}
		}

		//4. add user sel data range
		long lUseSel[7] = { 1, 2, 5, 10, 20, 50, 100 };
		for (a = 0; a < 7; a++)
		{
			strItem.Format("%d", lUseSel[a]);

			m_wndCPeriodNum.AddString(strItem);
		}		
	}

	UpdateData(FALSE);
}

void CMyStatView::GetLayerStatRet()
{
	long a, b;

	long lEPNum = 0;
	LTSH_ETCHPERIOD_RATE *pEP = g_StatHelper.GetProbability(lEPNum);

	//1. get hot/middle/cold number range
	for (a = 0; a < 3; a++)
	{
		//a. stat layer number.
		LTSH_NORMALSTAT_RET ns;
		memset(&ns, 0, sizeof(LTSH_NORMALSTAT_RET));
		ns.plData = new long[lEPNum];
		memset(ns.plData, 0, sizeof(long) * lEPNum);
		ns.lNum = lEPNum;
		ns.lAllowMix = 0;
		ns.lAllowMax = g_StatHelper.m_lSelFor;
		long *plDataTmp = ns.plData;
		
		LTSH_ETCHPERIOD_RATE *pEPTmp = pEP;
		for (b = 0; b < lEPNum; b++, pEPTmp++, plDataTmp++)
		{
			*plDataTmp = pEPTmp->lLayer[a];
		}

		g_StatHelper.NormalStat(&ns);

		//b. set to result.
		if (a == 0)
		{
			m_LayerStatRet.lHot[1] = ns.lRetUp;
			m_LayerStatRet.lHot[0] = ns.lRetDown;
		}
		else if (a == 1)
		{
			m_LayerStatRet.lMiddle[1] = ns.lRetUp;
			m_LayerStatRet.lMiddle[0] = ns.lRetDown;
		}
		else
		{
			m_LayerStatRet.lCold[1] = ns.lRetUp;
			m_LayerStatRet.lCold[0] = ns.lRetDown;
		}

		delete ns.plData;
	}

	//2. get rate numbers.
	for (a = 0; a < g_StatHelper.m_lSelFrom; a++)
	{
		m_LayerStatRet.lNO[a] = (pEP + lEPNum - 1)->lSortedProb[a] % 100;
	}
}

void CMyStatView::SetLayerStatRet()
{
	long a;
	
	//1. init layer ret.
	CString strItem;
	long lEPNum = 0;
	LTSH_ETCHPERIOD_RATE *pEP = g_StatHelper.GetProbability(lEPNum);	
	for (a = 0; a < lEPNum; a++, pEP++)
	{
		strItem.Format("总第%03d期:热[%d] 温[%d] 冷[%d]", pEP->lPeriod, pEP->lLayer[0], pEP->lLayer[1], pEP->lLayer[2]);
		m_wndAStatHisRet.AddString(strItem);	
	}
	m_wndAStatHisRet.SetCurSel(lEPNum - 1);
	
	//2. init range for layer.
	CString strR[3];
	strR[0].Format("热号:从%d 到 %d", m_LayerStatRet.lHot[0], m_LayerStatRet.lHot[1]);
	strR[1].Format("温号:从%d 到 %d", m_LayerStatRet.lMiddle[0], m_LayerStatRet.lMiddle[1]);
	strR[2].Format("冷号:从%d 到 %d", m_LayerStatRet.lCold[0], m_LayerStatRet.lCold[1]);

	for(a = 0; a < 3; a++) 
		m_wndAAveRet.AddString(strR[a]);

	m_wndAAveRet.SetCurSel(0);

	//3. init ret sel control
	m_wndBHF.SetCurSel(m_LayerStatRet.lHot[0]);
	m_wndBHT.SetCurSel(m_LayerStatRet.lHot[1]);
	m_wndBMF.SetCurSel(m_LayerStatRet.lMiddle[0]);
	m_wndBMT.SetCurSel(m_LayerStatRet.lMiddle[1]);
	m_wndBCF.SetCurSel(m_LayerStatRet.lCold[0]);
	m_wndBCT.SetCurSel(m_LayerStatRet.lCold[1]);

	//4. init empyt zone range.	
	LTSH_ZONESTAT_RET *pZonnStatRet = g_StatHelper.GetZoneStatRet();
	m_wndBHZMin.SetCurSel(pZonnStatRet->lHEmptyZoneMin);
	m_wndBHZMax.SetCurSel(pZonnStatRet->lHEmptyZoneMax);
	m_wndBVZMin.SetCurSel(pZonnStatRet->lVEmptyZoneMin);
	m_wndBVZMax.SetCurSel(pZonnStatRet->lVEmptyZoneMax);
	m_wndBHMin.SetCurSel(pZonnStatRet->lHZoneMaxOutMin);
	m_wndBHMax.SetCurSel(pZonnStatRet->lHZoneMaxOutMax);	
	m_wndBVMin.SetCurSel(pZonnStatRet->lVZoneMaxOutMin);
	m_wndBVMax.SetCurSel(pZonnStatRet->lVZoneMaxOutMax);
	
	//5. add filter info
	FillFilter();
}

void CMyStatView::FillFilter()
{
	CString strText;
	long lStartVal, lEndVal;

	IFilterDrawCtrl *pFilter = NULL;
	for (long a = 0; a < g_lFilterCtrlNum; a++)
	{
		pFilter = g_StatHelper.m_pFilters[a];

		pFilter->GetFinalRet(lStartVal, lEndVal);
		strText.Format("%s: 从[%d]到[%d]", pFilter->GetName(), lStartVal, lEndVal);

		//set current value as (Max * 1000 + Min)
		DWORD dwVal = lEndVal * 1000 + lStartVal;
		m_wndBFilter.SetItemData(m_wndBFilter.AddString(strText), dwVal);
	}
}

void CMyStatView::OnSelchangeACbStatfrom() 
{
	// TODO: Add your control notification handler code here
	long lIndex = m_wndAStatFrom.GetCurSel();
	if (lIndex != CB_ERR)
	{
		char cLayer[500] = {0};
		char cNums[10] = {0};
		
		long lEPNum = 0;
		LTSH_ETCHPERIOD_RATE *pEP = g_StatHelper.GetProbability(lEPNum);
		LTSH_ETCHPERIOD_RATE *pEPTmp = pEP + lIndex;
		for (long a = 0; a < g_StatHelper.m_lSelFrom; a++)
		{
			long lNO = pEPTmp->lSortedProb[a] % 100;

			if (a < g_StatHelper.m_lSelFor)
			{
				sprintf(cNums, "%02d,", lNO);
				strcat(cLayer, cNums);
			}
			else if (a >= g_StatHelper.m_lSelFrom - g_StatHelper.m_lSelFor)
			{
				if(a == g_StatHelper.m_lSelFrom - g_StatHelper.m_lSelFor)
				{
					m_strAMiddle = cLayer;
					cLayer[0] = 0;
				}

				sprintf(cNums, "%02d,", lNO);
				strcat(cLayer, cNums);
			}
			else
			{
				if (a == g_StatHelper.m_lSelFor)
				{
					m_strAHot = cLayer;
					cLayer[0] = 0;
				}

				sprintf(cNums, "%02d,", lNO);
				strcat(cLayer, cNums);

				if (a == (g_StatHelper.m_lSelFor + (g_StatHelper.m_lSelFrom - 2 * g_StatHelper.m_lSelFor) / 2 ))
					strcat(cLayer, "\n");
			}
		}

		m_strACold = cLayer;

		UpdateData(FALSE);
	}
}

void CMyStatView::OnSelchangeBLstFilter() 
{
	// TODO: Add your control notification handler code here
	long lCurSel = m_wndBFilter.GetCurSel();
	if (lCurSel != LB_ERR)
	{
		m_wndBFilterFrom.ResetContent();
		m_wndBFilterTo.ResetContent();

		long lMin, lMax;
		CString strItem;
		IFilterDrawCtrl *pFilter = g_StatHelper.m_pFilters[lCurSel];
		pFilter->GetMinMaxVal(lMin, lMax);		
		for (long a = lMin; a <= lMax; a++)
		{
			strItem.Format("%d", a);

			m_wndBFilterFrom.AddString(strItem);
			m_wndBFilterTo.AddString(strItem);
		}

		DWORD dwCurrVal = m_wndBFilter.GetItemData(lCurSel);
		long lCurrMin, lCurrMax;
		lCurrMin = dwCurrVal % 1000;
		lCurrMax = dwCurrVal / 1000;

		m_wndBFilterFrom.SetCurSel(lCurrMin - lMin);
		m_wndBFilterTo.SetCurSel(lCurrMax - lMin);

		UpdateData(FALSE);
	}
}

void CMyStatView::OnSelchangeBCbFilterf() 
{
	// TODO: Add your control notification handler code here
	long lCurSel = m_wndBFilter.GetCurSel();
	if (lCurSel != LB_ERR)
	{
		CString strVal;

		m_wndBFilterFrom.GetWindowText(strVal);
		long lUserVal = atol(strVal);
		
		DWORD dwCurrVal = m_wndBFilter.GetItemData(lCurSel);
		long lCurrMin, lCurrMax;
		lCurrMin = dwCurrVal % 1000;
		lCurrMax = dwCurrVal / 1000;

		if (lUserVal != lCurrMin)
		{
			lCurrMin = lUserVal;

			CString strText;
			IFilterDrawCtrl *pFilter = g_StatHelper.m_pFilters[lCurSel];
			strText.Format("%s: 从[%d]到[%d]", pFilter->GetName(), lCurrMin, lCurrMax);

			long lNewIndex = m_wndBFilter.InsertString(lCurSel, strText);
			DWORD dwVal = lCurrMax * 1000 + lCurrMin;
			m_wndBFilter.SetItemData(lNewIndex, dwVal);

			m_wndBFilter.DeleteString(lCurSel + 1);
		}
	}
}

void CMyStatView::OnSelchangeBCbFiltert() 
{
	// TODO: Add your control notification handler code here
	long lCurSel = m_wndBFilter.GetCurSel();
	if (lCurSel != LB_ERR)
	{
		CString strVal;

		m_wndBFilterTo.GetWindowText(strVal);
		long lUserVal = atol(strVal);
		
		DWORD dwCurrVal = m_wndBFilter.GetItemData(lCurSel);
		long lCurrMin, lCurrMax;
		lCurrMin = dwCurrVal % 1000;
		lCurrMax = dwCurrVal / 1000;

		if (lUserVal != lCurrMax)
		{
			lCurrMax = lUserVal;

			CString strText;
			IFilterDrawCtrl *pFilter = g_StatHelper.m_pFilters[lCurSel];
			strText.Format("%s: 从[%d]到[%d]", pFilter->GetName(), lCurrMin, lCurrMax);

			long lNewIndex = m_wndBFilter.InsertString(lCurSel, strText);
			DWORD dwVal = lCurrMax * 1000 + lCurrMin;
			m_wndBFilter.SetItemData(lNewIndex, dwVal);

			m_wndBFilter.DeleteString(lCurSel + 1);
		}
	}	
}

//>>>>>>>>>>>>>>>>>>> the following is calculate data by condition <<<<<<<<<<<<<<
//
void CMyStatView::OnCBtnGo() 
{
	// TODO: Add your control notification handler code here
	if (GetInputParam())
	{
		SetDlgItemText(IDC_C_STC_MINIMSG,"");

		m_pCalculateThread = AfxBeginThread(CalculateThread, this);
	}
}

BOOL CMyStatView::GetInputParam()
{
	UpdateData();

	memset(&m_InputParam, 0, sizeof(LTSV_INPUTPARAM_INFO));

	long a;
	//1. get three layer val
	{
		CString strLayerVal;
		CWnd *pWnd[] = 
		{ 
			&m_wndBHF, &m_wndBHT,
			&m_wndBMF, &m_wndBMT,
			&m_wndBCF, &m_wndBCT,
		};

		for (a = 0; a < 6; a++)
		{
			pWnd[a]->GetWindowText(strLayerVal);
			m_InputParam.lLayerRet[a] = atol(strLayerVal);
		}
	}

	//2. get NO filter selection.
	{
		m_InputParam.lHaveNum = m_wndBNOFilter.GetHaveNO(m_InputParam.lHave);
		m_InputParam.lNotNum = m_wndBNOFilter.GetHaveNO(m_InputParam.lNot);

		CString strZoneVal;
		CWnd *pWnd[] = 
		{ 
			&m_wndBHZMin, &m_wndBHZMax,
			&m_wndBVZMin, &m_wndBVZMax,
			&m_wndBHMin, &m_wndBHMax,
			&m_wndBVMin, &m_wndBVMax,
		};
		
		LTSH_ZONESTAT_RET &zoneStat = m_InputParam.zoneStatRet;
		long *plZoneVal[] = 
		{
			&zoneStat.lHEmptyZoneMin, &zoneStat.lHEmptyZoneMax,
			&zoneStat.lVEmptyZoneMin, &zoneStat.lVEmptyZoneMax,
			&zoneStat.lHZoneMaxOutMin, &zoneStat.lHZoneMaxOutMax,
			&zoneStat.lVZoneMaxOutMin, &zoneStat.lVZoneMaxOutMax,
		};

		for (a = 0; a < 8; a++)
		{
			pWnd[a]->GetWindowText(strZoneVal);
			*plZoneVal[a] = atol(strZoneVal);
		}
	}

	//3. get filter
	{		
		for (long a = 0; a < g_lFilterCtrlNum; a++)
		{
			DWORD dwCurrVal = m_wndBFilter.GetItemData(a);

			long lCurrMin, lCurrMax;
			lCurrMin = dwCurrVal % 1000;
			lCurrMax = dwCurrVal / 1000;

			m_InputParam.lFilterRet[a][0] = lCurrMin;
			m_InputParam.lFilterRet[a][1] = lCurrMax;
		}
	}

	//4. verify data
	if (!VerifyInputParam())
		return FALSE;

	return TRUE;
}

BOOL CMyStatView::VerifyInputParam()
{
	//check NO filter selection.
	CString strErr;
	CWnd *pCtrl = NULL;
	{		
		long lZoneRet[LT_MAX_SELFROM] = {0};
		g_StatHelper.GetDataZoneInfo(m_InputParam.lHave, lZoneRet);

		long lHZNum, lVZNum, lHZMaxOut, lVZMaxOut;
		lHZNum = lVZNum = lHZMaxOut = lVZMaxOut = 0;

		long lHZoneNum, lVZoneNum;
		g_DataInfo.GetZoneInfo(g_StatHelper.m_lSelFrom, lHZoneNum, lVZoneNum);
		lHZNum = lZoneRet[lHZoneNum - 1];
		lHZMaxOut = lZoneRet[lHZoneNum];
		lVZNum = lZoneRet[lHZoneNum + 2 + lVZoneNum - 1];
		lVZMaxOut = lZoneRet[lHZoneNum + 2 + lVZoneNum];

		LTSH_ZONESTAT_RET &zoneRet = m_InputParam.zoneStatRet;
		if (lHZNum < zoneRet.lHEmptyZoneMin)
		{
			strErr = "横向空区最小值设置太小！";
			pCtrl = &m_wndBHZMin;
		}
		else if (lVZNum < zoneRet.lVEmptyZoneMin)
		{
			strErr = "纵向空区最小值设置太小！";
			pCtrl = &m_wndBVZMin;
		}
		else if (lHZMaxOut > zoneRet.lHZoneMaxOutMax)
		{
			strErr = "横向区最大出数最大值设置太小！";
			pCtrl = &m_wndBHMax;
		}
		else if (lVZMaxOut > zoneRet.lVZoneMaxOutMax)
		{
			strErr = "纵向区最大出数最大值设置太小！";
			pCtrl = &m_wndBVMax;
		}
	}

	if (strErr.GetLength() > 0)
	{
		MsgHelper(strErr, FALSE);
		pCtrl->SetFocus();
	}

	return TRUE;
}

UINT CMyStatView::CalculateThread(LPVOID lpVoid)
{
	CMyStatView *pStatView = (CMyStatView *)lpVoid;

	pStatView->m_bThreadDoing = TRUE;
	pStatView->MsgHelper("计算有效组号开始...");
	pStatView->UpdateCtlStat(FALSE);
	pStatView->FreeRetInfoMem();
	pStatView->CalAveProb();
	
	long lDataNum = pStatView->m_lCalTotalRetInfoNum;
	if (lDataNum > 0)
	{
		pStatView->m_wndCProcess.SetRange32(0, lDataNum);
		pStatView->m_wndCProcess.SetStep(1);
		pStatView->m_wndCProcess.SetPos(0);

		long lTotalNum = 0, lValidateNum = 0;
		CString strMsg;
		for (long a = 0; a < lDataNum && !pStatView->m_bTherminateThread; a++)
		{
			pStatView->m_wndCProcess.StepIt();

			strMsg.Format("  开始校验第%d群组号码", a + 1);
			pStatView->MsgHelper(strMsg);

			long lN1 = 0, lN2 = 0;
			pStatView->CalValidateRet(
						&pStatView->m_CalTotalRetInfo[a].CbA,
						&pStatView->m_CalTotalRetInfo[a].CbB,
						&pStatView->m_CalTotalRetInfo[a].CbC,
						lN1, lN2);

			pStatView->m_CalTotalRetInfo[a].lTotalProducts = lN1;
			pStatView->m_CalTotalRetInfo[a].lValidateProducts = lN2;

			lTotalNum += lN1;
			lValidateNum += lN2;
		}

		pStatView->m_wndCProcess.SetPos(0);

		strMsg.Format("  共有%d组号码, 有效组数为%d", lTotalNum, lValidateNum);
		pStatView->MsgHelper(strMsg);
	}

	pStatView->UpdateCtlStat(TRUE);
	pStatView->m_bThreadDoing = FALSE;
	pStatView->m_bTherminateThread = FALSE;
	pStatView->MsgHelper("计算有效组号结束");

	return 1;
}

void CMyStatView::FreeRetInfoMem()
{
	for (long a = 0; a < m_lCalTotalRetInfoNum; a++)
	{
		delete m_CalTotalRetInfo[a].CbA.plData;
		delete m_CalTotalRetInfo[a].CbB.plData;
		delete m_CalTotalRetInfo[a].CbC.plData;
	}

	m_lCalTotalRetInfoNum = 0;
}

void CMyStatView::CalAveProb()
{
	MsgHelper("  生成群组号开始...");

	//1. get group number
	CalGetTotalGroup();

	CString strGroup;
	strGroup.Format("    共有%d个群组号", m_lGroupNum);
	MsgHelper(strGroup);

	//2. get each group number combin
	memset(m_CalTotalRetInfo, 0, sizeof(LTSV_CALTOTALRET_INFO) * 70);

	m_wndCProcess.SetRange32(0, m_lGroupNum);
	m_wndCProcess.SetStep(1);
	m_wndCProcess.SetPos(0);

	LTSV_CALGROUP_INFO *pGroupTemp = m_pGroup;
	for (long a = 0; a < m_lGroupNum; a++, pGroupTemp++)
	{
		m_CalTotalRetInfo[a].lGroupNo = a + 1;
		
		long *plData = NULL;
		long lDataNum = 0;
		long lSelNum = 0;

		if (pGroupTemp->lFirst > 0)
		{
			plData = m_LayerStatRet.lNO;
			lDataNum = g_StatHelper.m_lSelFor;
			lSelNum = pGroupTemp->lFirst;

			CalGetCombin(plData, lDataNum, lSelNum, &m_CalTotalRetInfo[a].CbA);
		}
								
		if (pGroupTemp->lSecond > 0)
		{
			plData = m_LayerStatRet.lNO + g_StatHelper.m_lSelFor;
			lDataNum = g_StatHelper.m_lSelFrom - 2 * g_StatHelper.m_lSelFor;
			lSelNum = pGroupTemp->lSecond;

			CalGetCombin(plData, lDataNum, lSelNum, &m_CalTotalRetInfo[a].CbB);
		}

		if (pGroupTemp->lThird > 0)
		{
			plData = m_LayerStatRet.lNO + g_StatHelper.m_lSelFrom - g_StatHelper.m_lSelFor;
			lDataNum = g_StatHelper.m_lSelFor;
			lSelNum = pGroupTemp->lThird;

			CalGetCombin(plData, lDataNum, lSelNum, &m_CalTotalRetInfo[a].CbC);
		}
		
		m_wndCProcess.StepIt();
	}

	m_wndCProcess.SetPos(0);
	
	m_lCalTotalRetInfoNum = m_lGroupNum;

	MsgHelper("  生成群组号结束");
}

void CMyStatView::CalGetTotalGroup()
{//cal group nums
	long lFF, lFT, lSF, lST, lTF, lTT;

	lFF = m_InputParam.lLayerRet[0];
	lFT = m_InputParam.lLayerRet[1];
	lSF = m_InputParam.lLayerRet[2];
	lST = m_InputParam.lLayerRet[3];
	lTF = m_InputParam.lLayerRet[4];
	lTT = m_InputParam.lLayerRet[5];

	LTSV_CALGROUP_INFO *pFinal = NULL;
	long lRemainNum = 0;
	long lTotalNum = (lFT - lFF + 1) * (lST - lSF + 1);
	if (lTotalNum > 0)
	{
		pFinal = new LTSV_CALGROUP_INFO[lTotalNum];
		memset(pFinal, 0, sizeof(LTSV_CALGROUP_INFO) * lTotalNum);
		LTSV_CALGROUP_INFO *pFinalTemp = pFinal;

		for (long a = lFF; a <= lFT; a++)
		{
			for(long b = lSF; b <= lST; b++)
			{
				long c = g_StatHelper.m_lSelFor - a - b;

				if (c >= lTF && c <= lTT)
				{
					pFinalTemp->lFirst = a;
					pFinalTemp->lSecond = b;
					pFinalTemp->lThird = c;

					pFinalTemp++;
					lRemainNum++;
				}
			}
		}
	}

	delete m_pGroup;
	m_pGroup = pFinal;
	m_lGroupNum = lRemainNum;
}

void CMyStatView::CalGetCombin(long *plInput, long lInputNum, long lSelNum, 
							   LTSV_CALCOMBIN_INFO *pCombinInfo)
{//calculate combination
	ASSERT(lInputNum > 0 && lSelNum > 0 && lInputNum >= lSelNum);

	long a, b, lRetNum;
	lRetNum = 0;

	//1. calculate ret num.
	{
		//formula: 
		//  C(x, y) = (x * (x-1) * (x-2) * ... * (x - y + 1)) / (y * (y - 1) * (y - 2) * .... * 1);
		long lTop = 1, lBottom = 1;
		long lTempNum1 = lInputNum;
		long lTempNum2 = lSelNum;

		for (a = 0; a < lSelNum; a++)
		{
			lTop *= lTempNum1--;
			lBottom *= lTempNum2--;
		}

		lRetNum = lTop / lBottom;
	}

	//2. get data number.
	pCombinInfo->plData = new long[lRetNum * lSelNum];
	pCombinInfo->lBlockNum = lRetNum;
	pCombinInfo->lBlockSize = lSelNum;

	long *plRetTemp = pCombinInfo->plData;
	long lDebugNum = 0;

	if (lSelNum == 1 || lInputNum == lSelNum)
	{//the only way to exit recrusion.
		memcpy(plRetTemp, plInput, sizeof(long) * lRetNum * lSelNum);

		lDebugNum = (lSelNum == 1) ? lInputNum : 1;
	}
	else
	{
		for (a = 0; a < lInputNum - lSelNum + 1; a++)
		{//need recruse times
			long lFirstNum = *(plInput + a);

			//recrusion...calling.
			LTSV_CALCOMBIN_INFO Cb;
			memset(&Cb, 0, sizeof(LTSV_CALCOMBIN_INFO));
			CalGetCombin(plInput + a + 1, lInputNum - a - 1, lSelNum - 1, &Cb);

			lDebugNum += Cb.lBlockNum;

			for (b = 0; b < Cb.lBlockNum; b++)
			{//get one layer.
				*plRetTemp = lFirstNum;

				memcpy(plRetTemp + 1, Cb.plData + b * Cb.lBlockSize, sizeof(long) * Cb.lBlockSize);
				plRetTemp += 1 + Cb.lBlockSize;
			}

			delete Cb.plData;
		}
	}

	ASSERT(lDebugNum == lRetNum);
}

void CMyStatView::CalValidateRet(LTSV_CALCOMBIN_INFO *pCA,
								 LTSV_CALCOMBIN_INFO *pCB,
								 LTSV_CALCOMBIN_INFO *pCC,
								 long &lTotalNum, long &lValidateNum)								 
{
	ASSERT(pCA->lBlockSize + pCB->lBlockSize + pCC->lBlockSize == g_StatHelper.m_lSelFor);

	long lBlockNum = 1;

	if (pCA->lBlockNum > 0) lBlockNum *= pCA->lBlockNum;
	if (pCB->lBlockNum > 0) lBlockNum *= pCB->lBlockNum;
	if (pCC->lBlockNum > 0) lBlockNum *= pCC->lBlockNum;

	long lMyValidateNum = 0;
	if (lBlockNum > 0)
	{
		long lA = pCA->lBlockNum > 0 ? pCA->lBlockNum : 1;
		long lB = pCB->lBlockNum > 0 ? pCB->lBlockNum : 1;
		long lC = pCC->lBlockNum > 0 ? pCC->lBlockNum : 1;

		long lRetData[LT_MAX_SELFOR];
		long *plRetData = NULL;

		for (long a = 0; a < lA; a++)
		{
			for (long b = 0; b < lB; b++)
			{
				for (long c = 0; c < lC; c++)
				{
					plRetData = lRetData;

					if (pCA->lBlockNum > 0)
					{
						memcpy(plRetData, pCA->plData + a * pCA->lBlockSize, sizeof(long) * pCA->lBlockSize);
						plRetData += pCA->lBlockSize;
					}

					if (pCB->lBlockNum > 0)
					{
						memcpy(plRetData, pCB->plData + b * pCB->lBlockSize, sizeof(long) * pCB->lBlockSize);
						plRetData += pCB->lBlockSize;
					}

					if (pCC->lBlockNum > 0)
					{
						memcpy(plRetData, pCC->plData + c * pCC->lBlockSize, sizeof(long) * pCC->lBlockSize);
						plRetData += pCC->lBlockSize;
					}

					if (TestIsValidate(lRetData))
						lMyValidateNum++;
				}
			}
		}
	}

	lTotalNum = lBlockNum;
	lValidateNum = lMyValidateNum;
}

BOOL CMyStatView::TestIsValidate(long lTestNum[LT_MAX_SELFOR])
{
	long a, b;
	//1. verify NO filter
	{
		//1. have NO		
		for (a = 0; a < m_InputParam.lHaveNum; a++)
		{
			BOOL bFind = FALSE;
			for (b = 0; b < g_StatHelper.m_lSelFor; b++)
			{
				if (m_InputParam.lHave[a] == lTestNum[b])
				{
					bFind = TRUE;
					break;
				}
			}

			if (!bFind)
				return FALSE;
		}
		
		//2. not have NO.
		for (a = 0; a < m_InputParam.lNotNum; a++)
		{
			for (b = 0; b < g_StatHelper.m_lSelFor; b++)
			{
				if (m_InputParam.lNot[a] == lTestNum[b])
					return FALSE;
			}
		}

		//3. zone varify		
		long lZoneRet[LT_MAX_SELFROM] = {0};
		g_StatHelper.GetDataZoneInfo(lTestNum, lZoneRet);

		long lHZNum, lVZNum, lHZMaxOut, lVZMaxOut;
		lHZNum = lVZNum = lHZMaxOut = lVZMaxOut = 0;

		long lHZoneNum, lVZoneNum;
		g_DataInfo.GetZoneInfo(g_StatHelper.m_lSelFrom, lHZoneNum, lVZoneNum);
		lHZNum = lZoneRet[lHZoneNum - 1];
		lHZMaxOut = lZoneRet[lHZoneNum];
		lVZNum = lZoneRet[lHZoneNum + 2 + lVZoneNum - 1];
		lVZMaxOut = lZoneRet[lHZoneNum + 2 + lVZoneNum];

		BOOL bZoneOK = FALSE;
		LTSH_ZONESTAT_RET &zoneRet = m_InputParam.zoneStatRet;
		if (lHZNum >= zoneRet.lHEmptyZoneMin && lHZNum <= zoneRet.lHEmptyZoneMax &&
			lVZNum >= zoneRet.lVEmptyZoneMin && lVZNum <= zoneRet.lVEmptyZoneMax &&
			lHZMaxOut >= zoneRet.lHZoneMaxOutMin && lHZMaxOut <= zoneRet.lHZoneMaxOutMax &&
			lVZMaxOut >= zoneRet.lVZoneMaxOutMin && lVZMaxOut <= zoneRet.lVZoneMaxOutMax)
		{
			bZoneOK = TRUE;
		}

		if (!bZoneOK)
			return FALSE;
	}

	//2. filter
	for (a = 0; a < g_lFilterCtrlNum; a++)
	{
		long lS = m_InputParam.lFilterRet[a][0];
		long lE = m_InputParam.lFilterRet[a][1];

		if (!g_StatHelper.m_pFilters[a]->IsValidate(lTestNum, lS, lE))			
			return FALSE;
	}

	return TRUE;
}


//>>>>>>>>>>>>>>>>>>>>>>> User get result <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void CMyStatView::OnCBtnNextperiod() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_pGetNextRetThread = AfxBeginThread(GetNextRetThread, this);	
}

UINT CMyStatView::GetNextRetThread(LPVOID lpVoid)
{
	CMyStatView *pStatView = (CMyStatView *)lpVoid;

	pStatView->m_bThreadDoing = TRUE;
	pStatView->UpdateCtlStat(FALSE);
	
	srand((unsigned int)time(NULL));
	CString strRepeat;
	pStatView->m_wndCPeriodNum.GetWindowText(strRepeat);
	long lRepeat = atol(strRepeat);
	long lTotalVaildateNum = pStatView->GetTotalValidateNum();

	//1. get rand index
	long *pRepRank = new long[lRepeat];
	memset(pRepRank, 0, sizeof(long) * lRepeat);
	long *pRepRankTemp = pRepRank;

	long a, b, lIndex;
	for (a = 0; a < lRepeat && lTotalVaildateNum > 0; a++, pRepRankTemp++)
	{
		while(1)
		{			
			lIndex = (rand() * rand()) % lTotalVaildateNum;

			for (b = 0; b < a; b++)
			{
				if (pRepRank[b] == lIndex)
				{//if have this index, then re-get new one.
					continue;
				}
			}

			*pRepRankTemp = lIndex;

			break;
		}
	}
	
	//2. get real user select combo data.
	delete pStatView->m_pWeGotInfo;
	pStatView->m_pWeGotInfo = new LTSV_WEGOT_INFO[lRepeat];
	memset(pStatView->m_pWeGotInfo, 0, sizeof(LTSV_WEGOT_INFO) * lRepeat);
	pStatView->m_lWeGotInfoNum = lRepeat;
	
	pStatView->MsgHelper("选号开始...");

	char szItem[100], szItemNo[10];

	pStatView->m_wndCProcess.SetRange32(0, lRepeat);
	pStatView->m_wndCProcess.SetStep(1);
	pStatView->m_wndCProcess.SetPos(0);

	pRepRankTemp = pRepRank;
	LTSV_WEGOT_INFO *pWeGetTemp = pStatView->m_pWeGotInfo;
	for (a = 0; a < lRepeat && lTotalVaildateNum > 0 && !pStatView->m_bTherminateThread; a++, pWeGetTemp++, pRepRankTemp++)
	{
		pStatView->m_wndCProcess.StepIt();

		long lRetNum[LT_MAX_SELFOR] = {0};
		long lSelNum[LT_MAX_SELFOR] = {0};
		long lSelIndex = 0;

		pStatView->GetValidateNumFromIndex(*pRepRankTemp, lRetNum);

		ASSERT(lRetNum[0] > 0);

		long lNotI = 0;
		
		if (pStatView->m_bCMultiSel || !g_StatHelper.m_bHaveSpec)
		{
			if (g_StatHelper.m_bHaveSpec)
				sprintf(szItem, "  自选第%03d组号码(复式):", a + 1);
			else
				sprintf(szItem, "  自选第%03d组号码:", a + 1);

			for (b = 0; b < g_StatHelper.m_lSelFor; b++)
			{
				sprintf(szItemNo, "%02d,", lRetNum[b]);
				strcat(szItem, szItemNo);
			}
		}
		else 
		{
			lNotI = rand() % g_StatHelper.m_lSelFor;
			sprintf(szItem, "  自选第%03d组号码(单注):", a + 1);

			for (b = 0; b < g_StatHelper.m_lSelFor; b++)
			{
				if (b != lNotI)
				{
					sprintf(szItemNo, "%02d,", lRetNum[b]);
					strcat(szItem, szItemNo);

					lSelNum[lSelIndex] = lRetNum[b];
					lSelIndex++;
				}
			}
		}

		pStatView->MsgHelper(szItem);

		pWeGetTemp->lGroupNo = a + 1;
		if (pStatView->m_bCMultiSel || !g_StatHelper.m_bHaveSpec)
		{
			pWeGetTemp->lSelNum = g_StatHelper.m_lSelFor;
			memcpy(pWeGetTemp->lData, lRetNum, sizeof(long) * g_StatHelper.m_lSelFor);
		}
		else
		{
			pWeGetTemp->lSelNum = lSelIndex;
			memcpy(pWeGetTemp->lData, lSelNum, sizeof(long) * lSelIndex);
		}
	}

	//3. end work.
	delete pRepRank;
	pStatView->m_wndCProcess.SetPos(0);

	pStatView->MsgHelper("选号结束");

	pStatView->UpdateCtlStat(TRUE);
	pStatView->m_bThreadDoing = FALSE;
	pStatView->m_bTherminateThread = FALSE;

	return 1;
}

long CMyStatView::GetTotalValidateNum()
{
	long lRetNum = 0;

	for (long a = 0; a < m_lCalTotalRetInfoNum; a++)
	{
		lRetNum += m_CalTotalRetInfo[a].lValidateProducts;
	}

	return lRetNum;
}

void CMyStatView::GetValidateNumFromIndex(long lIndex, long lValNum[LT_MAX_SELFOR])
{
	long lCounter = 0;
	BOOL bReset = TRUE;
	long lNextVal[LT_MAX_SELFOR] = {0};

	LTSV_CALTOTALRET_INFO *p = m_CalTotalRetInfo;
	long lNowVal = 0;
	for (long a = 0; a < m_lCalTotalRetInfoNum; a++, p++)
	{
		lNowVal += p->lValidateProducts;

		if (lNowVal > lIndex)
		{
			lCounter = lNowVal - p->lValidateProducts;
			break;
		}
	}
	
	bReset = TRUE;
	while (MyGetNextFromZH(&p->CbA, &p->CbB, &p->CbC, bReset, lNextVal))
	{
		bReset = FALSE;

		if (TestIsValidate(lNextVal))
		{
			if (lCounter == lIndex)
			{
				memcpy(lValNum, lNextVal, sizeof(long) * g_StatHelper.m_lSelFor);
				qsort(lValNum, g_StatHelper.m_lSelFor, sizeof(long), (LTSV_CMP_FUNC)MyCmpForRetSort);
				return;
			}

			lCounter++;
		}
	}
}

BOOL CMyStatView::MyGetNextFromZH(LTSV_CALCOMBIN_INFO *pCA, 
								  LTSV_CALCOMBIN_INFO *pCB, 
								  LTSV_CALCOMBIN_INFO *pCC,
									BOOL bReset, long lNextVal[LT_MAX_SELFOR])
{
	static long lConuter = 0;

	if (bReset)
		lConuter = 0;

	long lA, lB, lC, lIA, lIB, lIC;
	lA = lB = lC = lIA = lIB = lIC = 0;

	lA = pCA->lBlockNum > 0 ? pCA->lBlockNum : 1;
	lB = pCB->lBlockNum > 0 ? pCB->lBlockNum : 1;
	lC = pCC->lBlockNum > 0 ? pCC->lBlockNum : 1;

	lIA = lConuter / (lB * lC);
	lIB = (lConuter % (lB * lC)) / lC;
	lIC = lConuter % lC;

	long *plNextVal = lNextVal;
	if (pCA->lBlockNum > 0)
	{
		memcpy(plNextVal, pCA->plData + lIA * pCA->lBlockSize, sizeof(long) * pCA->lBlockSize);
		plNextVal += pCA->lBlockSize;
	}

	if (pCB->lBlockNum > 0)
	{
		memcpy(plNextVal, pCB->plData + lIB * pCB->lBlockSize, sizeof(long) * pCB->lBlockSize);
		plNextVal += pCB->lBlockSize;
	}

	if (pCC->lBlockNum > 0)
	{
		memcpy(plNextVal, pCC->plData + lIC * pCC->lBlockSize, sizeof(long) * pCC->lBlockSize);
		plNextVal += pCC->lBlockSize;
	}

	lConuter++;
	
	if (lConuter >= (lA * lB * lC))
		return FALSE;

	return TRUE;
}

//>>>>>>>>>>>>>>>>>>>>>>>>> helper function <<<<<<<<<<<<<<<<<<<<<<<<<<
void CMyStatView::UpdateCtlStat(BOOL bEnable)
{
	UINT uIDS[] =
	{
		IDC_C_BTN_GO,
		IDC_C_BTN_NEXTPERIOD,
	};

	for (long a = 0; a < sizeof(uIDS) / sizeof(uIDS[0]); a++)
	{
		GetDlgItem(uIDS[a])->EnableWindow(bEnable);
	}
}

void CMyStatView::MsgHelper(LPCTSTR lpszMsg, BOOL bUseList /*= TRUE*/)
{
	if (bUseList)
		m_wndCMsg.AddString(lpszMsg);
	else
		theApp.MsgBox(lpszMsg, MB_OK, theApp.m_pMainWnd);
}

int CMyStatView::MyCmpForRetSort(long *pl1, long *pl2)
{
	return ((*pl1) - (*pl2));
}

void CMyStatView::OnCBtnStopall()
{
	// TODO: Add your control notification handler code here
	if (m_bThreadDoing)
	{
		MsgHelper("请稍后...");
		m_bTherminateThread = TRUE;
	}
}

void CMyStatView::OnCBtnClear() 
{
	// TODO: Add your control notification handler code here
	m_wndCMsg.ResetContent();
}

void CMyStatView::OnCBtnSave() 
{
	// TODO: Add your control notification handler code here	
	char *pPathName = theApp.OpenFileDlg(FALSE, 
						g_DataInfo.GetFilter(CGloablData::LOF_LOG), 
						theApp.m_strLogFilePath, "txt");

	if (pPathName)
	{
		TRY
		{
			CStdioFile file(pPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeText);

			CString strItem;
			long lMsgCnt = m_wndCMsg.GetCount();
			
			for (long a = 0; a < lMsgCnt; a++)
			{
				m_wndCMsg.GetText(a, strItem);
				file.WriteString(strItem);
				file.WriteString("\n");
			}
		}
		CATCH(CFileException, e)
		{
			MsgHelper("打开文件失败!", FALSE);
		}
		END_CATCH
	}
}

void CMyStatView::OnCBtnSavenext() 
{
	// TODO: Add your control notification handler code here	
	char *pPathName = theApp.OpenFileDlg(FALSE, 
								g_DataInfo.GetFilter(CGloablData::LOF_USERGOT), 
								theApp.m_strUserDataPath, "txt");

	if (pPathName)
	{
		TRY
		{
			CStdioFile file(pPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeText);

			long a, b;
			char szItem[100], szVal[10];
			LTSV_WEGOT_INFO *pGot = m_pWeGotInfo;

			for (a = 0; a < m_lWeGotInfoNum; a++, pGot++)
			{
				sprintf(szItem, "第%d组号码:", pGot->lGroupNo);

				for (b = 0; b < pGot->lSelNum; b++)
				{
					sprintf(szVal, " %02d", pGot->lData[b]);
					strcat(szItem, szVal);
				}

				strcat(szItem, "\n");

				file.WriteString(szItem);
			}
		}
		CATCH(CFileException, e)
		{
			MsgHelper("打开文件失败!", FALSE);
		}
		END_CATCH
	}
}
