#if !defined(AFX_MYSTATVIEW_H__73C2F968_4FC0_4E24_ADDE_BAD84A48D952__INCLUDED_)
#define AFX_MYSTATVIEW_H__73C2F968_4FC0_4E24_ADDE_BAD84A48D952__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyStatView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyStatView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "StatHelper.h"
#include "NOFilterCtrl.h"

typedef int (*LTSV_CMP_FUNC)(const void *,const void *);

class CMyLTDoc;
class CMyStatView : public CFormView
{
protected:
	CMyStatView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMyStatView)

// Form Data
public:
	//{{AFX_DATA(CMyStatView)
	enum { IDD = IDD_DLG_STATVIEW };
	CComboBox	m_wndAStatFrom;
	CString		m_strAHot;
	CString		m_strAMiddle;
	CString		m_strACold;
	CComboBox	m_wndAStatHisRet;
	CComboBox	m_wndAAveRet;

	CComboBox	m_wndBHF;
	CComboBox	m_wndBHT;
	CComboBox	m_wndBMF;
	CComboBox	m_wndBMT;
	CComboBox	m_wndBCF;
	CComboBox	m_wndBCT;
	
	CNOFilterCtrl	m_wndBNOFilter;
	CComboBox	m_wndBHZMin;
	CComboBox	m_wndBHZMax;
	CComboBox	m_wndBVZMin;
	CComboBox	m_wndBVZMax;
	CComboBox	m_wndBHMin;
	CComboBox	m_wndBHMax;
	CComboBox	m_wndBVMin;	
	CComboBox	m_wndBVMax;

	CListBox	m_wndBFilter;
	CComboBox	m_wndBFilterFrom;
	CComboBox	m_wndBFilterTo;	

	CListBox	m_wndCMsg;
	CProgressCtrl	m_wndCProcess;
	CComboBox	m_wndCPeriodNum;	
	BOOL	m_bCMultiSel;
	CString	m_strCMiniMsg;
	//}}AFX_DATA

// Attributes
public:
	CMyLTDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyStatView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyStatView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMyStatView)
	afx_msg void OnSelchangeACbStatfrom();
	afx_msg void OnSelchangeBLstFilter();
	afx_msg void OnSelchangeBCbFilterf();
	afx_msg void OnSelchangeBCbFiltert();
	afx_msg void OnCBtnGo();
	afx_msg void OnCBtnStopall();
	afx_msg void OnCBtnNextperiod();
	afx_msg void OnCBtnClear();
	afx_msg void OnCBtnSave();	
	afx_msg void OnCBtnSavenext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	struct LTSV_LAYERSTAT_RET
	{//rate calculate result.
		long lNO[LT_MAX_SELFROM];

		//0:from 1:to
		long lHot[2];		
		long lMiddle[2];
		long lCold[2];
	};

	struct LTSV_INPUTPARAM_INFO
	{
		long lLayerRet[6];	//Hot(min/max) Middle(min/max) Cold(min/max)

		long lHave[LT_MAX_SELFOR], lHaveNum;
		long lNot[LT_MAX_SELFROM], lNotNum;
		LTSH_ZONESTAT_RET zoneStatRet;

		long lFilterRet[20][2];
	};

	struct LTSV_CALGROUP_INFO
	{
		long lFirst;
		long lSecond;
		long lThird;
	};

	struct LTSV_CALCOMBIN_INFO
	{
		long *plData;
		long lBlockNum;
		long lBlockSize;
	};

	struct LTSV_CALTOTALRET_INFO
	{
		long lGroupNo;
		long lTotalProducts;
		long lValidateProducts;

		LTSV_CALCOMBIN_INFO CbA;
		LTSV_CALCOMBIN_INFO CbB;
		LTSV_CALCOMBIN_INFO CbC;
	};

	struct LTSV_WEGOT_INFO
	{
		long lGroupNo;
		long lData[LT_MAX_SELFOR];
		long lSelNum;
	};

	//init.
	void DoInit();	
	void GetLayerStatRet();
	void SetLayerStatRet();
	void FillFilter();
	LTSV_LAYERSTAT_RET m_LayerStatRet;
	
	//input param
	LTSV_INPUTPARAM_INFO m_InputParam;
	BOOL GetInputParam();
	BOOL VerifyInputParam();

	//calculate thread
	CWinThread *m_pCalculateThread;
	static UINT CalculateThread(LPVOID lpVoid);
	BOOL m_bThreadDoing, m_bTherminateThread;
	void FreeRetInfoMem();
	void CalAveProb();
	LTSV_CALGROUP_INFO *m_pGroup;
	long m_lGroupNum;
	void CalGetTotalGroup();
	LTSV_CALTOTALRET_INFO m_CalTotalRetInfo[70];
	long m_lCalTotalRetInfoNum;
	void CalGetCombin(long *plInput, long lInputNum, long lSelNum, LTSV_CALCOMBIN_INFO *pCombinInfo);
	void CalValidateRet(LTSV_CALCOMBIN_INFO *pCA, LTSV_CALCOMBIN_INFO *pCB, LTSV_CALCOMBIN_INFO *pCC,
						long &lTotalNum, long &lValidateNum);
	BOOL TestIsValidate(long lTestNum[LT_MAX_SELFOR]);	
	
	//get user select data
	CWinThread *m_pGetNextRetThread;
	static UINT GetNextRetThread(LPVOID lpVoid);
	long GetTotalValidateNum();
	LTSV_WEGOT_INFO *m_pWeGotInfo;	
	long m_lWeGotInfoNum;
	void GetValidateNumFromIndex(long lIndex,long lValNum[LT_MAX_SELFOR]);
	BOOL MyGetNextFromZH(LTSV_CALCOMBIN_INFO *pCA,LTSV_CALCOMBIN_INFO *pCB,LTSV_CALCOMBIN_INFO *pCC,
						BOOL bReset,long lNextVal[LT_MAX_SELFOR]);


	//helper function.
	void GetZoneInfo(long &lNoInZone, long &lZoneNum);
	void UpdateCtlStat(BOOL bIsEnable);
	void MsgHelper(LPCTSTR lpszMsg, BOOL bUseList = TRUE);
	static int MyCmpForRetSort(long *pl1,long *pl2);
};

inline CMyLTDoc* CMyStatView::GetDocument()
   { return (CMyLTDoc*)m_pDocument; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSTATVIEW_H__73C2F968_4FC0_4E24_ADDE_BAD84A48D952__INCLUDED_)
