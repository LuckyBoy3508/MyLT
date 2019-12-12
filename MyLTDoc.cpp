// MyLTDoc.cpp : implementation of the CMyLTDoc class
//

#include "stdafx.h"
#include "MyLT.h"

#include "MyLTDoc.h"
#include "MainFrm.h"
#include "MyLTView.h"
#include "NewFileDlg.h"
#include "OpenFileDlg.h"
#include "SelPeriodDlg.h"
#include "DataTransDlg.h"
#include "FileDownLoadDlg.h"
#include "XmlHistData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyLTDoc

IMPLEMENT_DYNCREATE(CMyLTDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyLTDoc, CDocument)
	//{{AFX_MSG_MAP(CMyLTDoc)
	ON_COMMAND(IDM_FILE_NEW, OnFileNew)
	ON_COMMAND(IDM_FILE_OPEN, OnFileOpen)
	ON_COMMAND(IDM_FILE_SAVE, OnFileSave)
	ON_COMMAND(IDM_DATA_SELECT, OnDataSelect)
	ON_COMMAND(IDM_DATA_TRANSDATA, OnDataTransdata)
	ON_COMMAND(IDM_NET_DOWNLOAD, OnNetDownload)	
	ON_UPDATE_COMMAND_UI(IDM_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(IDM_DATA_SELECT, OnUpdateDataSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyLTDoc construction/destruction

CMyLTDoc::CMyLTDoc()
{
	// TODO: add one-time construction code here
	m_pDataInfo = NULL;
	m_lDataNum = 0;
	m_lSelFrom = 0;
	m_lSelFor = 0;
	m_bHaveSpec = FALSE;
	m_bIsDataChanged = FALSE;

	m_strFileName = "";
	m_strDesp = "¿ÕÎÄµµ";
	m_lProvince = 0;
	m_lFromPeriod = m_lToPeriod = 0;
	m_bIsApply = FALSE;
}

CMyLTDoc::~CMyLTDoc()
{
	if (m_pDataInfo)
		delete m_pDataInfo;

	FreeFileList();
}

BOOL CMyLTDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)		
	InitFileList();
	SetWindowTitle();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMyLTDoc serialization

void CMyLTDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyLTDoc diagnostics

#ifdef _DEBUG
void CMyLTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyLTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyLTDoc commands

void CMyLTDoc::OnFileNew() 
{
	// TODO: Add your command handler code here
	CNewFileDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		SaveDataInfo();

		m_strFileName = dlg.m_strRetName;
		m_strDesp = dlg.m_strDesp;
		m_lVersion = LT_PRODUTTION_VER;
		m_lProvince = dlg.m_lProvince;

		delete m_pDataInfo;
		m_pDataInfo = NULL;
		m_lDataNum = 0;
		m_lSelFrom = dlg.m_lSelFrom;
		m_lSelFor = dlg.m_lSelFor;
		m_bHaveSpec = dlg.m_bIsHaveSpec;

		m_lFromPeriod = m_lToPeriod = 0;
		m_bIsApply = FALSE;
		
		RWData(FALSE);
		
		SetWindowTitle();
		UpdateAllViews(NULL);
	}
}

void CMyLTDoc::OnFileOpen()
{
	// TODO: Add your command handler code here
	COpenFileDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		SaveDataInfo();
		OnGetData(dlg.m_RetFileInfo->szName);
	}
}

void CMyLTDoc::OnFileSave()
{
	// TODO: Add your command handler code here
	if (m_bIsDataChanged)
	{
		SaveDataInfo();
		UpdateAllViews(GetLtView());
	}
}

void CMyLTDoc::OnCloseDocument()
{
	// TODO: Add your specialized code here and/or call the base class
	SaveDataInfo();
	CDocument::OnCloseDocument();
}

void CMyLTDoc::OnGetData(LPCTSTR lpszFileName)
{
	m_strFileName = lpszFileName;
	if (RWData())
	{
		m_lFromPeriod = m_lToPeriod = 0;
		m_bIsApply = FALSE;

		SetWindowTitle();
		UpdateAllViews(NULL);
	}
	else
		m_strFileName = "";
}

void CMyLTDoc::SaveDataInfo()
{
	if (m_bIsDataChanged)
	{
		CMyLTView *pView = GetLtView();

		long lNewDataNum = 0;
		LT_DATA_INFO *pNewDataInfo = pView->GetNewDataInfo(lNewDataNum);
		m_lDataNum = lNewDataNum;

		delete m_pDataInfo;
		m_pDataInfo = new LT_DATA_INFO[m_lDataNum];
		memcpy(m_pDataInfo, pNewDataInfo, sizeof(LT_DATA_INFO) * m_lDataNum);

		RWData(FALSE);
		m_bIsDataChanged = FALSE;
	}
}

BOOL CMyLTDoc::RWData(BOOL bIsRead/* = TRUE*/)
{
	BOOL bRet = FALSE;

	LT_FILE_INFO *pDataFileInfo = GetFile(m_strFileName);

	if (pDataFileInfo)
	{
		if (bIsRead)
		{			
			if (CXmlHistData::RWHistData(pDataFileInfo, bIsRead))
			{
				delete m_pDataInfo;

				m_strDesp = pDataFileInfo->szDesp;
				m_lVersion = pDataFileInfo->lVersion; 			
				m_lProvince = pDataFileInfo->lProvince;

				m_pDataInfo = pDataFileInfo->pDataInfo;
				m_lDataNum = pDataFileInfo->lDataNum;
				m_lSelFrom = pDataFileInfo->lSelFrom;
				m_lSelFor = pDataFileInfo->lSelFor;
				m_bHaveSpec = pDataFileInfo->bHaveSpec;

				bRet = TRUE;
			}
		}
		else
		{
			strcpy(pDataFileInfo->szDesp, m_strDesp);
			pDataFileInfo->lVersion = LT_PRODUTTION_VER;		
			pDataFileInfo->lProvince = m_lProvince;

			pDataFileInfo->pDataInfo = m_pDataInfo;
			pDataFileInfo->lDataNum = m_lDataNum;
			pDataFileInfo->lSelFrom = m_lSelFrom;
			pDataFileInfo->lSelFor = m_lSelFor;
			pDataFileInfo->bHaveSpec = m_bHaveSpec;

			if (CXmlHistData::RWHistData(pDataFileInfo, bIsRead))
				bRet = TRUE;
		}
	}

	return bRet;
}

LT_DATA_INFO *CMyLTDoc::GetDataInfo(long &lDataNum)
{
	if (m_bIsApply)
	{
		if (m_lFromPeriod > m_lDataNum - 1)
			m_lFromPeriod = m_lDataNum - 1;
		if (m_lToPeriod > m_lDataNum - 1)
			m_lToPeriod = m_lDataNum - 1;

		lDataNum = m_lToPeriod - m_lFromPeriod + 1;
		return (m_pDataInfo + m_lFromPeriod);
	}
	
	lDataNum = m_lDataNum;
	return m_pDataInfo;
}

void CMyLTDoc::SetWindowTitle()
{
	if (theApp.m_pMainWnd)
	{
		CString strTitle;

#ifdef LT_REGISTERED_VERSION
		strTitle.Format("%s-%s[v%.2f ×¢²áÐòÁÐºÅ%05d]", 
			m_strDesp, LT_PRODUCTION_NAME, LT_PRODUTTION_VER * 0.01, LT_REGISTERED_VERSION);
#else
		strTitle.Format("%s-%s[v%.2f ·Ç×¢²á°æ]", m_strDesp, LT_PRODUCTION_NAME, LT_PRODUTTION_VER * 0.01);
#endif
		theApp.m_pMainWnd->SetWindowText(strTitle);
	}
}

LPCTSTR CMyLTDoc::IsDataValid(LT_DATA_INFO *pData, long lSelFrom, long lSelFor)
{
	long a;

	for (a = 0; a < lSelFor; a++)
	{
		if (pData->lData[a] < 1 || pData->lData[a] > lSelFrom)
			return "ºÅÂë³¬³ö·¶Î§!";
	}

	long lTstBuf[LT_MAX_SELFROM] = {0};
	for (a = 0; a < lSelFor; a++)
	{
		lTstBuf[pData->lData[a] - 1]++;

		if (lTstBuf[pData->lData[a] - 1] > 1)
			return "ºÅÂëÖØ¸´!";
	}

	return NULL;
}

void CMyLTDoc::OnDataSelect() 
{
	// TODO: Add your command handler code here
	CMyLTView *pView = GetLtView();

	if (m_bIsDataChanged)
	{
		SaveDataInfo();
		UpdateAllViews(pView);
	}

	CSelPeriodDlg dlg;
	dlg.SetParam(m_lDataNum, m_lFromPeriod, m_lToPeriod, m_bIsApply);
	if (dlg.DoModal() == IDOK)
	{
		if (m_lFromPeriod != dlg.m_lFromPeriod || 
			m_lToPeriod != dlg.m_lToPeriod ||
			m_bIsApply != dlg.m_bIsApply)
		{
			m_lFromPeriod = dlg.m_lFromPeriod;
			m_lToPeriod = dlg.m_lToPeriod;
			m_bIsApply = dlg.m_bIsApply;

			UpdateAllViews(pView);
		}
	}
}

void CMyLTDoc::OnDataTransdata() 
{
	// TODO: Add your command handler code here
	CDataTransDlg dlg;
	dlg.DoModal();	
}

void CMyLTDoc::OnNetDownload() 
{
	// TODO: Add your command handler code here
	CFileDownLoadDlg dlg;
	dlg.DoModal();	
}

void CMyLTDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsDataChanged);
}

void CMyLTDoc::OnUpdateDataSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_strFileName.IsEmpty() && m_lDataNum > 1);
}

CMyLTView *CMyLTDoc::GetLtView()
{
	CMyLTView *pView = NULL;

	POSITION pos = GetFirstViewPosition();
	if (pos)
		pView = (CMyLTView *)GetNextView(pos);
	
	return pView;
}

void CMyLTDoc::InitFileList()
{
	FreeFileList();

	CString strPathName;
	strPathName.Format("%s\\*.xml", theApp.m_strHistDataPath);

	CDvFindRet dvFindRet;
	CDvDir::FindAllFiles(strPathName, dvFindRet);
	POSITION pos = dvFindRet.m_lstFile.GetHeadPosition();
	while (pos)
	{
		DVDIR_FILE *pFile = dvFindRet.m_lstFile.GetNext(pos); 
		
		LT_FILE_INFO *pDataFileInfo = new LT_FILE_INFO;
		memset(pDataFileInfo, 0, sizeof(LT_FILE_INFO));
		
		strcpy(pDataFileInfo->szName, pFile->strFileName);
		if (CXmlHistData::RWHistData(pDataFileInfo, TRUE, TRUE))
		{
			m_lstFile.AddTail(pDataFileInfo);
		}
		else
		{
			delete pDataFileInfo;
		}
	}
}

FILELIST *CMyLTDoc::GetFileList()
{
	return &m_lstFile;
}

BOOL CMyLTDoc::IsHaveFile(LPCTSTR lpszFileName)
{
	BOOL bRet = FALSE;

	LT_FILE_INFO *pFileInfo = NULL;

	POSITION pos = m_lstFile.GetHeadPosition();
	while (pos)
	{
		pFileInfo = m_lstFile.GetNext(pos);
		
		if (stricmp(pFileInfo->szName, lpszFileName) == 0)
		{
			bRet = TRUE;
			break;
		}
	}
	
	return bRet;
}

LT_FILE_INFO *CMyLTDoc::GetFile(LPCTSTR lpszFileName)
{
	LT_FILE_INFO *pRet = NULL;

	LT_FILE_INFO *pFileInfo = NULL;

	POSITION pos = m_lstFile.GetHeadPosition();
	while (pos)
	{
		pFileInfo = m_lstFile.GetNext(pos);
		
		if (stricmp(pFileInfo->szName, lpszFileName) == 0)
		{
			pRet = pFileInfo;
			break;
		}
	}
	
	return pRet;
}

void CMyLTDoc::DeleteFile(LPCTSTR lpszFileName)
{
	LT_FILE_INFO *pFileInfo = NULL;

	POSITION pos, pos1;
	pos = m_lstFile.GetHeadPosition();
	while (pos)
	{
		pos1 = pos;

		pFileInfo = m_lstFile.GetNext(pos);
		if (stricmp(pFileInfo->szName, lpszFileName) == 0)
		{
			//remove it from list.
			m_lstFile.RemoveAt(pos1);

			//truly delete file.
			CString strFullName;
			strFullName.Format("%s\\%s", theApp.m_strHistDataPath, lpszFileName);
			CFile::Remove(strFullName);

			break;
		}
	}
}
	
void CMyLTDoc::FreeFileList()
{
	POSITION pos = m_lstFile.GetHeadPosition();
	while (pos)
	{
		LT_FILE_INFO *pFileInfo = m_lstFile.GetNext(pos);
		delete pFileInfo;
	}

	m_lstFile.RemoveAll();
}
