
#include "stdafx.h"
#include "DvDir.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
//CDvFindRet
CDvFindRet::CDvFindRet()
{
	;
}

CDvFindRet::~CDvFindRet()
{
	FreeFileListMem();
}

void CDvFindRet::FreeFileListMem()
{
	POSITION pos = m_lstFile.GetHeadPosition();
	while (pos)
	{
		DVDIR_FILE *pTmp = m_lstFile.GetNext(pos);
		delete pTmp;
	}
	m_lstFile.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// CDvDir

CDvDir::CDvDir()
{
	;
}

CDvDir::~CDvDir()
{
	;
}

BOOL CDvDir::IsFileExist(LPCTSTR lpszFileName)
{
	BOOL bRet = FALSE;

	if (lpszFileName && !IsHaveWildcard(lpszFileName))
	{
		CDvFindRet dvFindRet;
		FindAllFiles(lpszFileName, dvFindRet);
		if (dvFindRet.m_lstFile.GetCount() == 1)
			bRet = TRUE;
	}

	return bRet;
}

BOOL CDvDir::CopyFile(LPCTSTR lpszSrcFileName, LPCTSTR lpszDestFileName)
{
	return ::CopyFile(lpszSrcFileName, lpszDestFileName, FALSE);
}

BOOL CDvDir::MoveFile(LPCTSTR lpszSrcFileName, LPCTSTR lpszDestFileName)
{
	return ::MoveFile(lpszSrcFileName, lpszDestFileName);
}

BOOL CDvDir::DeleteFile(LPCTSTR lpszFileName)
{
	BOOL bRet = FALSE;

	if (lpszFileName)
	{
		if (!IsHaveWildcard(lpszFileName))
			bRet = ::DeleteFile(lpszFileName);
		else
		{
			CString strPath;
			if (SplitPathName(lpszFileName, 4, strPath))
			{
				CDvFindRet dvFindRet;
				FindAllFiles(lpszFileName, dvFindRet);

				POSITION pos = dvFindRet.m_lstFile.GetHeadPosition();
				while (pos)
				{
					DVDIR_FILE *pFile = dvFindRet.m_lstFile.GetNext(pos);

					CString strFile;					
					strFile.Format(L"%s%s", strPath, pFile->strFileName);
					bRet = ::DeleteFile(strFile);

					if (!bRet)
						break;
				}
			}
		}
	}
	
	return bRet;
}

void CDvDir::FindAllFiles(LPCTSTR lpszPathName, 
						  CDvFindRet &dvFindRet, long lFindType/* = 0*/)
{
	dvFindRet.FreeFileListMem();

	if (lpszPathName)
	{
		WIN32_FIND_DATA wfd;
		HANDLE hf = ::FindFirstFile(lpszPathName, &wfd);
		if (hf != INVALID_HANDLE_VALUE)
		{
			BOOL bContinue = TRUE;
			while (bContinue)
			{
				BOOL bAdd = FALSE;

				if (lFindType == 0 && wfd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
					bAdd = TRUE;
				else if (lFindType == 1 && wfd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
					bAdd = TRUE;
				else if (lFindType == 2)
					bAdd = TRUE;
				
				if (bAdd)
				{
					DVDIR_FILE *pNew = new DVDIR_FILE;
					dvFindRet.m_lstFile.AddTail(pNew);

					pNew->strFileName = wfd.cFileName;
				}
		
				bContinue = ::FindNextFile(hf, &wfd);
			}

			::FindClose(hf);
		}
	}
}

BOOL CDvDir::SplitPathName(LPCTSTR lpszPathName, long lType, CString &strRet)
{
	BOOL bRet = FALSE;

	if (lpszPathName && lType >= 0 && lType <= 5)
	{
		wchar_t szDriver[_MAX_PATH];
		wchar_t szDir[_MAX_PATH];
		wchar_t szFName[_MAX_PATH];
		wchar_t szExt[_MAX_PATH];

		_wsplitpath_s(lpszPathName, szDriver, _MAX_PATH, szDir, _MAX_PATH, szFName, _MAX_PATH, szExt, _MAX_PATH);

		if (lType == 0)
			strRet = szDriver;
		else if (lType == 1)
			strRet = szDir;
		else if (lType == 2)
			strRet = szFName;
		else if (lType == 3)
			strRet = szExt;
		else if (lType == 4)
			strRet.Format(L"%s%s", szDriver, szDir);
		else if (lType == 5)
			strRet.Format(L"%s%s", szFName, szExt);

		bRet = TRUE;
	}

	return bRet;
}

BOOL CDvDir::IsHaveWildcard(LPCTSTR lpszPathName)
{
	BOOL bRet = FALSE;

	if (lpszPathName)
	{
		CString strFileName;
		if (SplitPathName(lpszPathName, 5, strFileName))
		{
			if (strFileName.Find('?') != -1 || strFileName.Find('*') != -1)
				bRet = TRUE;
		}
	}

	return bRet;
}

BOOL CDvDir::IsDirExist(LPCTSTR lpszDirName)
{
	BOOL bRet = FALSE;

	if (lpszDirName && !IsHaveWildcard(lpszDirName))
	{
		CDvFindRet dvFindRet;
		FindAllFiles(lpszDirName, dvFindRet, 1);
		if (dvFindRet.m_lstFile.GetCount() == 1)
			bRet = TRUE;
	}

	return bRet;
}

BOOL CDvDir::CreateDir(LPCTSTR lpszDirName)
{
	BOOL bRet = FALSE;

	if (!IsDirExist(lpszDirName))
		bRet = ::CreateDirectory(lpszDirName, NULL);

	return bRet;
}

BOOL CDvDir::CopyDir(LPCTSTR lpszSrcDirName, LPCTSTR lpszDestDirName, BOOL bWithSubDir/* = TRUE*/)
{
	BOOL bRet = FALSE;

	return bRet;
}

BOOL CDvDir::MoveDir(LPCTSTR lpszSrcDirName, LPCTSTR lpszDestDirName, BOOL bWithSubDir/* = TRUE*/)
{
	BOOL bRet = FALSE;

	return bRet;
}

BOOL CDvDir::DeleteDir(LPCTSTR lpszDirName)
{
	BOOL bRet = FALSE;

	return bRet;
}
