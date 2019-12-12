
#ifndef __DVDIR_H
#define __DVDIR_H

#define DV_MAX_FILENAMELEN		DV_DEF_SMALLLEN
#define DV_MAX_DESCRIPTIONLEN	DV_DEF_MIDDLELEN
#define DV_MAX_DIRNAMELEN		DV_DEF_LONGLEN

struct DVDIR_FILE
{
	CString strFileName;
};
typedef CList<DVDIR_FILE *, DVDIR_FILE *> DVDIR_FILELIST;

class AFX_CLASS_EXPORT CDvFindRet
{
public:
	CDvFindRet();
	~CDvFindRet();

public:
	void FreeFileListMem();
	DVDIR_FILELIST m_lstFile;
};

class AFX_CLASS_EXPORT CDvDir
{
public:
	CDvDir();
	~CDvDir();
	
public:
	//file operate
	static BOOL IsFileExist(LPCTSTR lpszFileName);
	static BOOL CopyFile(LPCTSTR lpszSrcFileName, LPCTSTR lpszDestFileName);
	static BOOL MoveFile(LPCTSTR lpszSrcFileName, LPCTSTR lpszDestFileName);
	static BOOL DeleteFile(LPCTSTR lpszFileName);
	//lFindType: 0:File 1:Dir 2:File/Dir
	static void FindAllFiles(LPCTSTR lpszPathName, CDvFindRet &dvFindRet, long lFindType = 0);

	//c:\windows\system32\myabc.dll
	//lType: 0:driver(c:) 1:directory(\windows\system32\) 
	//       2:finename(myabc) 3:extenname(.dll) 
	//		 4:path-name(c:\windows\system32\) 
	//		 5:full-file-name(myabc.dll)
	static BOOL SplitPathName(LPCTSTR lpszPathName, long lType, CString &strRet);
	static BOOL IsHaveWildcard(LPCTSTR lpszPathName);

	//directory operate
	static BOOL IsDirExist(LPCTSTR lpszDirName);
	static BOOL CreateDir(LPCTSTR lpszDirName);
	static BOOL CopyDir(LPCTSTR lpszSrcDirName, LPCTSTR lpszDestDirName, BOOL bWithSubDir = TRUE);
	static BOOL MoveDir(LPCTSTR lpszSrcDirName, LPCTSTR lpszDestDirName, BOOL bWithSubDir = TRUE);
	static BOOL DeleteDir(LPCTSTR lpszDirName);
};


#endif
