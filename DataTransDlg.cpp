// DataTransDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "DataTransDlg.h"

#include "MyLTDoc.h"
#include "XmlHistData.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataTransDlg dialog

CDataTransDlg::CDataTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataTransDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataTransDlg)
	m_bHaveSpec = FALSE;
	m_strDesp = _T("");
	m_strFileName = _T("");
	m_bUpOrder = FALSE;
	m_strDataFileName = _T("");
	//}}AFX_DATA_INIT

	m_lSelFrom = m_lSelFor = 0;
	m_szSepa[0] = 0;
}


void CDataTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataTransDlg)
	DDX_Control(pDX, IDC_CB_PROVINCE, m_wndProvince);
	DDX_Control(pDX, IDC_STA_NO8, m_wndStaNo8);
	DDX_Control(pDX, IDC_STA_NO7, m_wndStaNo7);
	DDX_Control(pDX, IDC_STA_NO6, m_wndStaNo6);
	DDX_Control(pDX, IDC_STA_NO5, m_wndStaNo5);
	DDX_Control(pDX, IDC_STA_NO4, m_wndStaNo4);
	DDX_Control(pDX, IDC_STA_NO3, m_wndStaNo3);
	DDX_Control(pDX, IDC_STA_NO2, m_wndStaNo2);
	DDX_Control(pDX, IDC_STA_NO1, m_wndStaNo1);
	DDX_Control(pDX, IDC_CB_TO, m_wndTo);
	DDX_Control(pDX, IDC_CB_SEPA, m_wndSepa);
	DDX_Control(pDX, IDC_CB_PERIOD, m_wndPeriod);
	DDX_Control(pDX, IDC_CB_NO8, m_wndNo8);
	DDX_Control(pDX, IDC_CB_NO7, m_wndNo7);
	DDX_Control(pDX, IDC_CB_NO6, m_wndNo6);
	DDX_Control(pDX, IDC_CB_NO5, m_wndNo5);
	DDX_Control(pDX, IDC_CB_NO4, m_wndNo4);
	DDX_Control(pDX, IDC_CB_NO3, m_wndNo3);
	DDX_Control(pDX, IDC_CB_NO2, m_wndNo2);
	DDX_Control(pDX, IDC_CB_NO1, m_wndNo1);
	DDX_Control(pDX, IDC_CB_FROM, m_wndFrom);
	DDX_Control(pDX, IDC_CB_DEVOTE, m_wndDevote);
	DDX_Control(pDX, IDC_CB_DATE, m_wndDate);
	DDX_Check(pDX, IDC_CK_HAVESPEC, m_bHaveSpec);
	DDX_Text(pDX, IDC_EDT_DESP, m_strDesp);
	DDX_Text(pDX, IDC_EDT_FILENAME, m_strFileName);
	DDX_Check(pDX, IDC_CK_UPORDER, m_bUpOrder);
	DDX_Text(pDX, IDC_EDT_FILENAME2, m_strDataFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataTransDlg, CDialog)
	//{{AFX_MSG_MAP(CDataTransDlg)
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_BROSER, OnBtnBroser)
	ON_CBN_SELCHANGE(IDC_CB_TO, OnSelchangeCbTo)
	ON_BN_CLICKED(IDC_CK_HAVESPEC, OnCkHavespec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataTransDlg message handlers
BOOL CDataTransDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	long a, b;
	CString strItem;
	
	for (a = 0; a < g_DataInfo.GetProvinceNum(); a++)
	{
		m_wndProvince.AddString(g_DataInfo.GetProvinceName(a));
	}
	long lCurSel = (theApp.m_SysCfgInfo.lDefProvince == -1) ? 1 : theApp.m_SysCfgInfo.lDefProvince;
	m_wndProvince.SetCurSel(lCurSel);

	for (a = LT_MIN_SELFROM; a <= LT_MAX_SELFROM; a++)
	{
		strItem.Format("%d", a);
		m_wndFrom.AddString(strItem);
	}
	m_wndFrom.SetCurSel(LT_DEF_SELFROM - LT_MIN_SELFROM);

	for (a = LT_MIN_SELFOR; a <= LT_MAX_SELFOR; a++)
	{
		strItem.Format("%d", a - 1);
		m_wndTo.AddString(strItem);
	}
	m_wndTo.SetCurSel(LT_DEF_SELFOR - LT_MIN_SELFOR);
	m_bHaveSpec = TRUE;

	m_wndSepa.AddString("空格");
	m_wndSepa.AddString("逗号");
	m_wndSepa.AddString("TAB键");
	m_wndSepa.AddString("空格/逗号");
	m_wndSepa.AddString("空格/TAB键");
	m_wndSepa.AddString("逗号/TAB键");
	m_wndSepa.AddString("空格/逗号/TAB键");
	m_wndSepa.SetCurSel(6);

	m_bUpOrder = FALSE;

	CComboBox *pCombox[] =
	{
		&m_wndPeriod, &m_wndDate, &m_wndDevote,
		&m_wndNo1, &m_wndNo2, &m_wndNo3, &m_wndNo4, &m_wndNo5, &m_wndNo6, &m_wndNo7, &m_wndNo8
	};
	for (a = 0; a < sizeof(pCombox) / sizeof(pCombox[0]); a++)
	{
		for (b = -1; b <= LT_DT_MAXFIELD; b++)
		{
			strItem.Format("%d", b);
			pCombox[a]->AddString(strItem);
		}

		if (a == 0)
			pCombox[a]->SetCurSel(2);
		else if (a == 1)
			pCombox[a]->SetCurSel(3);
		else if (a == 2)
			pCombox[a]->SetCurSel(0);
		else
			pCombox[a]->SetCurSel(a + 1);
	}

	UpdateData(FALSE);

	//must after updatedata, or no updata.
	OnSelchangeCbTo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataTransDlg::OnBtnBroser() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	char *pPathName = theApp.OpenFileDlg(TRUE, 
				g_DataInfo.GetFilter(CGloablData::LOF_HIST_TEXT), theApp.m_strHistTextPath);

	if (pPathName)
	{
		m_strFileName = pPathName;
		UpdateData(FALSE);					
	}
}

void CDataTransDlg::OnSelchangeCbTo() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	long lSelFor = m_wndTo.GetCurSel();

	CWnd *pCtrls[2][LT_MAX_SELFOR] =
	{
		{
			&m_wndStaNo1, &m_wndStaNo2, &m_wndStaNo3, &m_wndStaNo4, 
			&m_wndStaNo5, &m_wndStaNo6, &m_wndStaNo7, &m_wndStaNo8, 
		},
		{
			&m_wndNo1, &m_wndNo2, &m_wndNo3, &m_wndNo4, 
			&m_wndNo5, &m_wndNo6, &m_wndNo7, &m_wndNo8, 
		},
	};

	if (lSelFor != CB_ERR)
	{
		long lShowNum = lSelFor + LT_MIN_SELFOR - 1;

		for (long a = 0; a < LT_MAX_SELFOR - 1; a++)
		{
			if (a < lShowNum)
			{
				pCtrls[0][a]->ShowWindow(TRUE);
				pCtrls[1][a]->ShowWindow(TRUE);
			}
			else
			{
				pCtrls[0][a]->ShowWindow(FALSE);
				pCtrls[1][a]->ShowWindow(FALSE);
				((CComboBox *)pCtrls[1][a])->SetCurSel(1);
			}
		}
		
		pCtrls[0][LT_MAX_SELFOR - 1]->ShowWindow(m_bHaveSpec);
		pCtrls[1][LT_MAX_SELFOR - 1]->ShowWindow(m_bHaveSpec);

		if (!m_bHaveSpec)
			((CComboBox *)pCtrls[1][LT_MAX_SELFOR - 1])->SetCurSel(1);
	}
}

void CDataTransDlg::OnCkHavespec()
{
	// TODO: Add your control notification handler code here
	OnSelchangeCbTo();
}

void CDataTransDlg::OnBtnStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	long lIndex[LT_DT_USEDFIELD] = {0};//{1, 2, -1, 3, 4, 5, 6, 7, 8, 9, 10};
	if (IsDataValid() && GetInputParam() && GetDataIndex(lIndex))
	{	
		if (CreateTempFile())
		{
			long lMaybeNum = GetMaybeDataNum();
			
			LT_DT_DATAINFO *pDtDataInfo = new LT_DT_DATAINFO[lMaybeNum];
			memset(pDtDataInfo, 0, sizeof(LT_DT_DATAINFO) * lMaybeNum);
			
			GetMaybeData(pDtDataInfo, lMaybeNum);

			LT_DATA_INFO *pDataInfo = new LT_DATA_INFO[lMaybeNum];
			memset(pDataInfo, 0, sizeof(LT_DATA_INFO) * lMaybeNum);

			TransData(pDtDataInfo, pDataInfo, lMaybeNum, lIndex);
			
			ChangeSpecNoPos(pDataInfo, lMaybeNum);

			if (IsDateValid(pDataInfo, lMaybeNum))
			{
				LT_FILE_INFO fileDataInfo;
				memset(&fileDataInfo, 0, sizeof(LT_FILE_INFO));
				
				strcpy(fileDataInfo.szName, m_strDataFileName);
				fileDataInfo.lVersion = LT_PRODUTTION_VER;
				strcpy(fileDataInfo.szDesp, m_strDesp);
				fileDataInfo.lProvince = m_wndProvince.GetCurSel();
				fileDataInfo.pDataInfo = pDataInfo;
				fileDataInfo.lDataNum = lMaybeNum;
				fileDataInfo.lSelFrom = m_lSelFrom;
				fileDataInfo.lSelFor = m_lSelFor;
				fileDataInfo.bHaveSpec = m_bHaveSpec;

				if (CXmlHistData::RWHistData(&fileDataInfo, FALSE))
				{
					theApp.MsgBox("文件转换成功^-^!", MB_OK | MB_ICONINFORMATION, this);

					CMyLTDoc *pDoc = (CMyLTDoc *)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();

					LT_FILE_INFO *pNewFile = new LT_FILE_INFO;
					memset(pNewFile, 0, sizeof(LT_FILE_INFO));
					strcpy(pNewFile->szName, fileDataInfo.szName);
					strcpy(pNewFile->szDesp, fileDataInfo.szDesp);
					pNewFile->lProvince = fileDataInfo.lProvince;
					pDoc->GetFileList()->AddTail(pNewFile);
				}
				else
					theApp.MsgBox("文件转换失败!", MB_OK | MB_ICONINFORMATION, this);
			}

			delete pDtDataInfo;
			delete pDataInfo;
			
			DeleteTempFile();
		}
	}
}

BOOL CDataTransDlg::IsDataValid()
{
	if (m_strFileName.IsEmpty())
	{
		theApp.MsgBox("请选择待转换的文本文件!");
		return FALSE;
	}

	if (m_strDataFileName.IsEmpty())
	{
		theApp.MsgBox("请选择转换后的数据文件!");
		return FALSE;
	}
	else
	{
		m_strDataFileName += ".xml";

		CMyLTDoc *pDoc = (CMyLTDoc *)((CFrameWnd *)theApp.m_pMainWnd)->GetActiveDocument();
		if (pDoc->IsHaveFile(m_strDataFileName))
		{
			if (IDOK == theApp.MsgBox("已经存在该文件，是否覆盖?", MB_OKCANCEL | MB_ICONINFORMATION))
			{
				pDoc->DeleteFile(m_strDataFileName);
			}
			else
			{
				GetDlgItem(IDC_EDT_FILENAME2)->SetFocus();
				return FALSE;
			}
		}
	}

	if (m_strDesp.IsEmpty())
	{
		theApp.MsgBox("请输入历史数据描述信息!");
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDataTransDlg::GetInputParam()
{
/*
	m_wndSepa.AddString("空格");
	m_wndSepa.AddString("逗号");
	m_wndSepa.AddString("TAB键");
	m_wndSepa.AddString("空格/逗号");
	m_wndSepa.AddString("空格/TAB键");
	m_wndSepa.AddString("逗号/TAB键");
	m_wndSepa.AddString("空格/逗号/TAB键");
*/
	char *pSepa[] = {" ", ",", "\t", " ,", " \t", ",\t", " ,\t", };

	m_lSelFrom = m_wndFrom.GetCurSel() + LT_MIN_SELFROM;
	m_lSelFor = m_wndTo.GetCurSel() + LT_MIN_SELFOR;
	if (!m_bHaveSpec) m_lSelFor--;
	
	strcpy(m_szSepa, pSepa[m_wndSepa.GetCurSel()]);

	return TRUE;
}

BOOL CDataTransDlg::GetDataIndex(long lIndex[LT_DT_USEDFIELD])
{
	long *plIndex = lIndex;
	memset(plIndex, 0, sizeof(long) * LT_DT_USEDFIELD);
	CString strItem;

	CComboBox *pCombox[] = 
	{
		&m_wndPeriod, &m_wndDate, &m_wndDevote,
		&m_wndNo1, &m_wndNo2, &m_wndNo3, &m_wndNo4, &m_wndNo5, &m_wndNo6, &m_wndNo7, &m_wndNo8
	};

	for (long a = 0; a < sizeof(pCombox) / sizeof(pCombox[0]); a++)
	{
		pCombox[a]->GetWindowText(strItem);
		*(plIndex + a) = atol(strItem);
	}
	
	return TRUE;
}

BOOL CDataTransDlg::CreateTempFile()
{
	FILE *pFile = fopen(m_strFileName, "r");
	char *pBuf = NULL;
	long lBufLen = 0;

	if (pFile)
	{        
		// lBufLen = _filelength(pFile->_file);
        lBufLen = ftell(pFile);
		pBuf = new char[lBufLen];
		memset(pBuf, 0, lBufLen);
		fread(pBuf, lBufLen, 1, pFile);
		fclose(pFile);
	}
	else
	{
		theApp.MsgBox("打开待转换的文本文件失败!");
		return FALSE;
	}


	char szTempFile[MAX_PATH] = {0};
	strcpy(szTempFile, m_strFileName);
	*(strrchr(szTempFile, '.') + 1) = 0;
	strcat(szTempFile, "tmp");
	
	m_strFileName = szTempFile;
	pFile = fopen(m_strFileName, "w");
	if (pFile)
	{
		ReplaceChinaSpace(pBuf, lBufLen);
		fwrite(pBuf, lBufLen, 1, pFile);
		fclose(pFile);
	}
	else
	{
		delete pBuf;
		theApp.MsgBox("创建临时文件失败!");
		return FALSE;
	}

	delete pBuf;

	return TRUE;
}

void CDataTransDlg::ReplaceChinaSpace(char *pBuf, long lBufLen)
{
	char *pBufTemp = pBuf;
	for (long a = 0; a < lBufLen; a++, pBufTemp++)
	{
		if (*pBufTemp == -95)
		{
			*pBufTemp = ' ';
		}
	}
}

void CDataTransDlg::DeleteTempFile()
{
	remove(m_strFileName);
}

long CDataTransDlg::GetMaybeDataNum()
{
	long lMaybeNum = 0;

	TRY
	{
		CStdioFile file(m_strFileName, CFile::modeRead | CFile::typeText);
		
		CString strItem;
		while (file.ReadString(strItem))
		{
			strItem.TrimLeft();
			strItem.TrimRight();

			if (strItem.GetLength() > 1)
				lMaybeNum++;
		}
	}
	CATCH(CFileException, e)
	{
		theApp.MsgBox("打开文件失败!");
	}
	END_CATCH

	return lMaybeNum;
}

void CDataTransDlg::GetMaybeData(LT_DT_DATAINFO *pDtDataInfo, long lDataNum)
{
	LT_DT_DATAINFO *pTemp = pDtDataInfo;
	CString strItem;
	char szLine[200];

	CStdioFile file(m_strFileName, CFile::modeRead | CFile::typeText);
	
	while(file.ReadString(strItem))
	{
		strItem.TrimLeft();
		strItem.TrimRight();

		if (strItem.GetLength() > 1)
		{
			strcpy(szLine, strItem);

			long i = 0;
			char *pToken = strtok(szLine, m_szSepa);

			while (pToken && i < LT_DT_MAXFIELD)
			{
				strItem = pToken;
				strcpy(pTemp->szField[i], strItem);

				i++;
				pToken = strtok(NULL, m_szSepa);
			}

			pTemp++;
		}
	}
}

void CDataTransDlg::TransData(LT_DT_DATAINFO *pDtDataInfo, LT_DATA_INFO *pDataInfo, 
							  long lMaybeNum, long lIndex[LT_DT_USEDFIELD])
{
	long a, b, lLastIndex = 0;
	LT_DT_DATAINFO *pDtTemp = pDtDataInfo;

	LT_DATA_INFO *pTemp = pDataInfo;
	if (!m_bUpOrder)
		 pTemp = pDataInfo + lMaybeNum - 1;

	for (a = 0; a < lMaybeNum; a++, pDtTemp++)
	{
		lLastIndex = 0;
		for (b = 0; b < LT_DT_MAXFIELD; b++, lLastIndex++)
		{
			if (pDtTemp->szField[b][0] == '\0')
			{
				lLastIndex--;
				break;
			}
		}

		for (b = 0; b < LT_DT_USEDFIELD; b++)
		{
			if (!lIndex[b]) continue;

			switch (b)
			{
			case 0:
				{
					if (lIndex[b] != -1)
						strcpy(pTemp->szPeriod, pDtTemp->szField[lIndex[b] - 1]);
					else
						strcpy(pTemp->szPeriod, pDtTemp->szField[lLastIndex]);
				}
				break;
			case 1:
				{
					if (lIndex[b] != -1)
						strcpy(pTemp->szDate, pDtTemp->szField[lIndex[b] - 1]);
					else
						strcpy(pTemp->szDate, pDtTemp->szField[lLastIndex]);
				}
				break;
			case 2:
				{
					if (lIndex[b] != -1)
						pTemp->lTotalDev = atol(pDtTemp->szField[lIndex[b] - 1]);
					else
						pTemp->lTotalDev = atol(pDtTemp->szField[lLastIndex]);
				}
				break;
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				{
					if (lIndex[b] != -1)
						pTemp->lData[b - 3] = atol(pDtTemp->szField[lIndex[b] - 1]);
					else
						pTemp->lData[b - 3] = atol(pDtTemp->szField[lLastIndex]);
				}
				break;
			default:
				break;
			}
		}

		if (m_bUpOrder)
		{
			pTemp->lPeriodID = a;
			pTemp++;
		}
		else
		{
			pTemp->lPeriodID = lMaybeNum - a - 1;
			pTemp--;
		}
	}
}

void CDataTransDlg::ChangeSpecNoPos(LT_DATA_INFO *pDataInfo, long lMaybeNum)
{
	if (m_bHaveSpec && m_lSelFor < LT_MAX_SELFOR)
	{
		LT_DATA_INFO *pTemp = pDataInfo;
		for (long a = 0; a < lMaybeNum; pTemp++)
		{
			pTemp->lData[m_lSelFor - 1] = pTemp->lData[LT_MAX_SELFOR - 1];
		}
	}
}

BOOL CDataTransDlg::IsDateValid(LT_DATA_INFO *pDataInfo, long lMaybeNum)
{
	LT_DATA_INFO *pTemp = pDataInfo;
	for (long a = 0; a < lMaybeNum; a++, pTemp++)
	{
		LPCTSTR lpszErr = CMyLTDoc::IsDataValid(pTemp, m_lSelFrom, m_lSelFor);
		if (lpszErr)
		{
			CString strErr;
			strErr.Format("数据有误:%s\n期数:%s", lpszErr, pTemp->szPeriod);
			theApp.MsgBox(strErr);
			return FALSE;
		}
	}

	return TRUE;
}


