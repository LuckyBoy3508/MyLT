// FileDownLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mylt.h"
#include "FileDownLoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDownLoadDlg dialog


CFileDownLoadDlg::CFileDownLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileDownLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileDownLoadDlg)
	//}}AFX_DATA_INIT
	m_pFileInfo = NULL;
	m_lFileInfoNum = 0;
}


void CFileDownLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileDownLoadDlg)
	DDX_Control(pDX, IDC_STA_HOMEPAGE, m_wndHomepage);
	DDX_Control(pDX, IDC_ANI_DOWNLOAD, m_wndAniDownLoad);
	DDX_Control(pDX, IDC_PRO_DOWNSTATE, m_wndDLProcess);
	DDX_Control(pDX, IDC_CB_PROVINCE, m_wndProvince);
	DDX_Control(pDX, IDC_LST_PROVINCE, m_wndListProvince);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileDownLoadDlg, CDialog)
	//{{AFX_MSG_MAP(CFileDownLoadDlg)
	ON_CBN_SELCHANGE(IDC_CB_PROVINCE, OnSelchangeCbProvince)
	ON_BN_CLICKED(IDC_BTN_GETFILELIST, OnBtnGetfilelist)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDownLoadDlg message handlers

BOOL CFileDownLoadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndListProvince.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_wndListProvince.InsertColumn(0, "序号", LVCFMT_LEFT, 50, 0);
	m_wndListProvince.InsertColumn(1, "省份", LVCFMT_LEFT, 50, 1);
	m_wndListProvince.InsertColumn(2, "描述", LVCFMT_LEFT, 240, 2);
	m_wndListProvince.InsertColumn(3, "文件名", LVCFMT_LEFT, 80, 3);

	m_wndProvince.AddString("所有");
	for (long a = 0; a < g_DataInfo.GetProvinceNum(); a++)
	{		
		m_wndProvince.AddString(g_DataInfo.GetProvinceName(a));
	}
	m_wndProvince.SetCurSel(theApp.m_SysCfgInfo.lDefProvince + 1);
	
	m_strLocalFile = "";
	m_strRemoteFile = "";

	m_wndAniDownLoad.Open(IDR_AVI_DOWNLOAD);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileDownLoadDlg::OnBtnGetfilelist() 
{
	// TODO: Add your control notification handler code here
	m_strLocalFile.Format("%s\\%s", theApp.m_strDownLoadPath, LT_FILELIST_NAME);
	m_strRemoteFile.Format("%s/%s/%s", LT_NET_HOSTADDR, LT_FILELIST_DIR, LT_FILELIST_NAME);

	if (DownLoadFile())
	{
		ReadFileList();
		OnSelchangeCbProvince();
	}
	else
	{
		theApp.MsgBox("下载文件列表出错，请检测网络连接是否正确！");
	}
}

BOOL CFileDownLoadDlg::DownLoadFile()
{
	CDvDir::DeleteFile(m_strLocalFile);

	m_wndAniDownLoad.Play(0, -1, -1);
	BOOL hRet = CDvNet::DownLoadFile(m_strRemoteFile, m_strLocalFile, NULL);
	m_wndAniDownLoad.Close();

	return hRet;
}

void CFileDownLoadDlg::ReadFileList()
{
	delete m_pFileInfo;
	m_lFileInfoNum = 0;

	CString strFileName, strAppName, strItem;
	strFileName = m_strLocalFile;
	char *pSecName[] = {"数据数", "省名", "描述", "文件名"}; 
	
	m_lFileInfoNum = ::GetPrivateProfileInt("总数据数", pSecName[0], 0, strFileName);

	m_pFileInfo = new LT_FILE_INFO[m_lFileInfoNum];
	memset(m_pFileInfo, 0, sizeof(LT_FILE_INFO) * m_lFileInfoNum);
	LT_FILE_INFO *pFileTmp = m_pFileInfo;

	for (long a = 0; a < m_lFileInfoNum; a++, pFileTmp++)
	{
		strAppName.Format("%s%03d", "数据", a);

		::GetPrivateProfileString(strAppName, pSecName[1], "", 
				strItem.GetBuffer(LT_MAX_FILENAMELEN), LT_MAX_FILENAMELEN, strFileName);
		strItem.ReleaseBuffer();

		pFileTmp->lProvince = GetProvinceIndex(strItem);
		
		::GetPrivateProfileString(strAppName, pSecName[2], "", pFileTmp->szDesp, LT_MAX_DESCRIPTIONLEN, strFileName);
		::GetPrivateProfileString(strAppName, pSecName[3], "", pFileTmp->szName, LT_MAX_FILENAMELEN, strFileName);
	}
}

void CFileDownLoadDlg::OnSelchangeCbProvince() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndProvince.GetCurSel();

	CString strItem;
	long lItem = 0;

	m_wndListProvince.DeleteAllItems();
	LT_FILE_INFO *pFileTmp = m_pFileInfo;
	for (long a = 0; a < m_lFileInfoNum; a++, pFileTmp++)
	{
		if (nIndex == 0 || (nIndex == pFileTmp->lProvince + 1))
		{
			strItem.Format("%d", lItem);

			m_wndListProvince.InsertItem(lItem, strItem);

			m_wndListProvince.SetItemText(lItem, 1, g_DataInfo.GetProvinceName(pFileTmp->lProvince));
			m_wndListProvince.SetItemText(lItem, 2, pFileTmp->szDesp);
			m_wndListProvince.SetItemText(lItem, 3, pFileTmp->szName);
			m_wndListProvince.SetItemData(lItem, (DWORD)pFileTmp);

			lItem++;
		}		
	}
}

void CFileDownLoadDlg::OnBtnDown() 
{
	// TODO: Add your control notification handler code here
	FILELIST tmplist;
	LT_FILE_INFO *pTmp;

	long lTotalNum, lHaveDownNum;
	lTotalNum = lHaveDownNum = 0;

	POSITION pos = m_wndListProvince.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nIndex = m_wndListProvince.GetNextSelectedItem(pos);
		pTmp = (LT_FILE_INFO *)m_wndListProvince.GetItemData(nIndex);

		tmplist.AddTail(pTmp);
	}

	lTotalNum = tmplist.GetCount();
	SetDlgItemInt(IDC_STA_TOTALNUM, lTotalNum);
	m_wndDLProcess.SetRange32(0, lTotalNum);
	m_wndDLProcess.SetPos(0);
	m_wndDLProcess.SetStep(1);

	if (lTotalNum > 0)
	{
		pos = tmplist.GetHeadPosition();
		while (pos)
		{
			pTmp = tmplist.GetNext(pos);

			m_strLocalFile.Format("%s\\%s", theApp.m_strDownLoadPath, pTmp->szName);
			m_strRemoteFile.Format("%s/%s/%s", LT_NET_HOSTADDR, LT_FILELIST_DIR, pTmp->szName);

			DownLoadFile();

			lHaveDownNum++;
			SetDlgItemInt(IDC_STA_HAVEDOWNNUM, lHaveDownNum);
			m_wndDLProcess.StepIt();

			CString strDstName;
			strDstName.Format("%s\\%s", theApp.m_strHistDataPath, pTmp->szName);
			CDvDir::DeleteFile(strDstName);
			CDvDir::MoveFile(m_strLocalFile, strDstName);
		}

		theApp.MsgBox("文件下载完成!");
	}
	else if (m_wndListProvince.GetItemCount() > 0)
	{
		theApp.MsgBox("请选择要下载的文件!");
	}

	m_wndDLProcess.SetPos(0);
}

void CFileDownLoadDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}

BOOL CFileDownLoadDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	DeleteAllTmpFile();
	delete m_pFileInfo;

	return CDialog::DestroyWindow();
}

long CFileDownLoadDlg::GetProvinceIndex(LPCTSTR lpszProvince)
{
	long lProvince = -1;
	
	for (long a = 0; a < g_DataInfo.GetProvinceNum(); a++)
	{
		if (strcmp(lpszProvince, g_DataInfo.GetProvinceName(a)) == 0)
			return a;
	}	

	return lProvince;
}

void CFileDownLoadDlg::DeleteAllTmpFile()
{
	char szPath[MAX_PATH] = {0};
	sprintf(szPath, "%s\\*.xml", theApp.m_strDownLoadPath);

	CDvDir::DeleteFile(szPath);
}
