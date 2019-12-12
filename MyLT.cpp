// MyLT.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MyLT.h"

#include "MainFrm.h"
#include "MyLTDoc.h"
#include "MyLTView.h"
#include "SysInfoDlg.h"
#include "MyAboutCtrl.h"
#include "ReginsterDlg.h"
#include "HyperLinkCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyLTApp

BEGIN_MESSAGE_MAP(CMyLTApp, CWinApp)
	//{{AFX_MSG_MAP(CMyLTApp)
	ON_COMMAND(IDM_FILE_EXIT, OnFileExit)
	ON_COMMAND(IDM_OPT_SYSSETUP, OnOptSyssetup)
	ON_COMMAND(IDM_NET_UPDATE, OnNetUpdate)
	ON_COMMAND(IDM_NET_REGISTER, OnNetRegister)
	ON_COMMAND(IDM_HLP_CONTENT, OnHlpContent)
	ON_COMMAND(IDM_HLP_ABOUT, OnHlpAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyLTApp construction

CMyLTApp::CMyLTApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_strMainPath = "";
	m_strHistDataPath = "";
	m_strHistTextPath = "";
	m_strUserDataPath = "";
	m_strLogFilePath = "";
	m_strCfgFilePath = "";
	m_strDownLoadPath = "";

	memset(&m_SysCfgInfo, 0, sizeof(LT_SYSCFG_INFO));
	m_SysCfgInfo.lDefProvince = 1;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyLTApp object

CMyLTApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMyLTApp initialization

BOOL CMyLTApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
    
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMyLTDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMyLTView));
	AddDocTemplate(pDocTemplate);

	//begin processing...
	{//get all dirs.
		char szPath[MAX_PATH] = {0};
		GetModuleFileName(NULL, szPath, MAX_PATH);
		*(strrchr(szPath, '\\')) = 0;

		//1. init all path
		m_strMainPath = szPath;
		m_strDownLoadPath.Format("%s\\%s", m_strMainPath, LT_DOWNLOAD_DIR);
		m_strHistDataPath.Format("%s\\%s", m_strMainPath, LT_HISTDATA_DIR);
		m_strHistTextPath.Format("%s\\%s", m_strMainPath, LT_TEXTDATA_DIR);
		m_strUserDataPath.Format("%s\\%s", m_strMainPath, LT_USERDATA_DIR);
		m_strLogFilePath.Format("%s\\%s", m_strMainPath, LT_LOGFILE_DIR);

		m_strCfgFilePath.Format("%s\\%s", m_strMainPath, LT_CFGFILE_NAME);		

		//2. if dir not exist, create it.
		CDvDir::CreateDir(m_strDownLoadPath);
		CDvDir::CreateDir(m_strHistDataPath);
		CDvDir::CreateDir(m_strHistTextPath);
		CDvDir::CreateDir(m_strUserDataPath);
		CDvDir::CreateDir(m_strLogFilePath);		
	}

	{//read config file
		if (!RWCfg())
			return FALSE;
	}

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	
	{//must after create doc can add view.
		((CMainFrame *)m_pMainWnd)->AddAllView();
	}	
	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	{//auto load data
		if (m_SysCfgInfo.bIsAutoLoad)
		{
			CMyLTDoc *pDoc = (CMyLTDoc *)((CFrameWnd *)m_pMainWnd)->GetActiveDocument();
			pDoc->OnGetData(m_SysCfgInfo.szFileName);
		}
	}

	{//show register message.
		ShowRegMsg();
	}

	return TRUE;
}

void CMyLTApp::ShowRegMsg()
{
#ifndef LT_REGISTERED_VERSION
	CReginsterDlg dlg;
	dlg.DoModal();

	OnNetUpdate();
	OnNetRegister();
#endif
}

void CMyLTApp::OnNetUpdate() 
{
	// TODO: Add your command handler code here
	ShellExecute(m_pMainWnd->GetSafeHwnd(), "open", LT_NET_HOSTADDR, NULL, NULL, SW_SHOW);	
}

void CMyLTApp::OnNetRegister() 
{
	// TODO: Add your command handler code here
	ShellExecute(m_pMainWnd->GetSafeHwnd(), "open", LT_NET_PROD_REG, NULL, NULL, SW_SHOW);	
}

BOOL CMyLTApp::CheckReg()
{
#ifndef LT_REGISTERED_VERSION
	theApp.MsgBox("抱歉，非注册版本不提供此功能！\n请选择菜单：网络->软件注册查看详细信息！");
	OnNetRegister();
	return FALSE;
#endif

	return TRUE;
}

int CMyLTApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class	
	RWCfg(FALSE);

	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_DLG_ABOUT };
	CHyperLinkCtrl	m_wndHttpHost;
	CMyAboutCtrl m_wndCredit;
	CButton m_btnOK;
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STA_HTTPHOST, m_wndHttpHost);
	DDX_Control(pDX, IDC_STATIC_CREDIT, m_wndCredit);
	DDX_Control(pDX, IDOK, m_btnOK);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_wndHttpHost.SetHyperLink(LT_NET_HOSTADDR);

	m_wndCredit.SetStarSpeed(30);
	m_wndCredit.SetScrollSpeed(2);

	CString strVer;
	strVer.Format("%s v%.2f", LT_PRODUCTION_NAME, LT_PRODUTTION_VER * 0.01);
	m_wndCredit.AddTextLine(strVer);
	m_wndCredit.AddTextLine("版权所有 (C) 2003-2005");
	m_wndCredit.AddTextLine("程序: David");
	m_wndCredit.AddTextLine("E-mail:dfliu@163.com ");
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// CMyLTApp message handlers
BOOL CMyLTApp::RWCfg(BOOL bIsRead /*= TRUE*/)
{
	DVXML_NODE *pTmpNode = NULL;
	DVXML_NODE *pRoot = m_DvXml.GetRoot();

	if (bIsRead)
	{
		//first load file
		if (!m_DvXml.Load(m_strCfgFilePath, "MyLT"))
		{
			MsgBox(m_DvXml.GetError());
			return FALSE;
		}
	
		//app info		
		pTmpNode = m_DvXml.OperateNode(pRoot, "SysCfg", CDvXML::CHECKONLY);
		if (pTmpNode)
		{
			m_DvXml.GetAttrib(pTmpNode, "AutoLoad", m_SysCfgInfo.bIsAutoLoad);
			m_DvXml.GetAttrib(pTmpNode, "FileName", m_SysCfgInfo.szFileName);
			m_DvXml.GetAttrib(pTmpNode, "DefProvince", m_SysCfgInfo.lDefProvince);
		}

		//global info
		g_DataInfo.RWCfg(bIsRead);
	}
	else
	{
		pTmpNode = m_DvXml.OperateNode(pRoot, "SysCfg", CDvXML::CHECKCREATE);
		
		m_DvXml.SetAttrib(pTmpNode, "AutoLoad", m_SysCfgInfo.bIsAutoLoad);
		m_DvXml.SetAttrib(pTmpNode, "FileName", m_SysCfgInfo.szFileName);
		m_DvXml.SetAttrib(pTmpNode, "DefProvince", m_SysCfgInfo.lDefProvince);

		//global info
		g_DataInfo.RWCfg(bIsRead);

		//final save all info
		m_DvXml.Save();
	}

	return TRUE;
}

BOOL CMyLTApp::MsgBox(LPCTSTR lpszMsg, UINT uType/* = MB_OK | MB_ICONINFORMATION*/, 
					  CWnd *pParent/* = NULL*/)
{
	HWND hParent = theApp.m_pMainWnd->GetSafeHwnd();
	if (pParent)
		hParent = pParent->GetSafeHwnd();

	return ::MessageBox(hParent, lpszMsg, LT_PRODUCTION_NAME, uType);
}

char *CMyLTApp::OpenFileDlg(BOOL bIsOpen, const char *pFilter, 
							const char *pInitDir, const char *pDefExt/* = NULL*/)
{
	static char szPathName[MAX_PATH] = {0};
	
	CFileDialog dlg(bIsOpen, pDefExt, NULL,
		OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, pFilter, m_pMainWnd);
	dlg.m_ofn.lpstrInitialDir = pInitDir;

	if(dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		strcpy(szPathName, strPathName);

		return szPathName;
	}
	
	return NULL;
}

void CMyLTApp::OnFileExit()
{
	// TODO: Add your command handler code here
	::PostMessage(m_pMainWnd->GetSafeHwnd(), WM_CLOSE, 0, 0);
}

void CMyLTApp::OnOptSyssetup() 
{
	// TODO: Add your command handler code here
	CSysInfoDlg dlg;
	dlg.SetParam(&m_SysCfgInfo);

	if (dlg.DoModal() == IDOK)
		memcpy(&m_SysCfgInfo, &dlg.m_SysCfgInfo, sizeof(LT_SYSCFG_INFO));
	
}

void CMyLTApp::OnHlpContent() 
{
	// TODO: Add your command handler code here
	::ShellExecute(AfxGetMainWnd()->m_hWnd, 
					"open", 
					"MyLT.chm",
					NULL, 
					m_strMainPath, 
					SW_SHOWNORMAL);
}

void CMyLTApp::OnHlpAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}
