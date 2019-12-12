
#include "stdafx.h"
#include "DvNet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDvNet

CDvNet::CDvNet()
{
}

CDvNet::~CDvNet()
{
}

BOOL CDvNet::DownLoadFile(LPCTSTR lpszURL, LPCTSTR lpszDst, DOWNLOADFILE pfnMsgCallBK)
{
	CInternetSession InternetSession;
	CHttpConnection *pHttpConnection = NULL;
	CHttpFile *pHttpFile = NULL;

	DWORD dwServiceType = AFX_INET_SERVICE_HTTP;
	CString strServer, strObject;
	INTERNET_PORT nPort;
	if (!AfxParseURL(lpszURL, dwServiceType, strServer, strObject, nPort))
	{
		ReturnMsg("Bad URL!", pfnMsgCallBK);
		return FALSE;
	}
	
	TRY
	{
		ReturnMsg("Connect to server...", pfnMsgCallBK);

		pHttpConnection = InternetSession.GetHttpConnection(strServer, 
						INTERNET_FLAG_KEEP_CONNECTION,
						INTERNET_DEFAULT_HTTP_PORT, NULL, NULL);

		InternetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 1000);
		InternetSession.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 1000);
		InternetSession.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 1000);
		InternetSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 1000);
		InternetSession.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 1000);

		ReturnMsg("Connect to server OK!", pfnMsgCallBK);

		BOOL bRequest = TRUE;
		pHttpFile = pHttpConnection->OpenRequest("GET", strObject);
		if (pHttpFile->SendRequest(NULL))
		{
			DWORD dwStatusCode;
			pHttpFile->QueryInfoStatusCode(dwStatusCode);

			if (dwStatusCode >= 400 && dwStatusCode <= 499)
			{
				ReturnMsg("Request error!", pfnMsgCallBK);
				bRequest = FALSE;
			}
		}
		else
		{
			ReturnMsg("SendRequest failed!", pfnMsgCallBK);
			bRequest = FALSE;
		}
		
		if (bRequest)
		{//begin down load file to disk.
			CFile f;

			TRY
			{
				ReturnMsg("Begin down load file...", pfnMsgCallBK);

				f.Open(lpszDst, CFile::modeCreate | CFile::modeWrite);

				char szBuf[1000];
				long lReadLen = 0;

				while (1)
				{
					lReadLen = pHttpFile->Read(szBuf, 1000);					
					f.Write(szBuf, lReadLen);
					if (lReadLen < 1000)
						break;
				}

				f.Close();

				ReturnMsg("Down load file OK!", pfnMsgCallBK);
			}
			CATCH (CFileException, e)
			{
				CString strErr;
				e->GetErrorMessage(strErr.GetBuffer(1024), 1024);
				strErr.ReleaseBuffer();

				ReturnMsg(strErr, pfnMsgCallBK);
			}
			END_CATCH
		}
	}
	CATCH (CInternetException, e)
	{
		CString strErr;
		e->GetErrorMessage(strErr.GetBuffer(1024), 1024);
		strErr.ReleaseBuffer();

		ReturnMsg(strErr, pfnMsgCallBK);
	}
	END_CATCH

	pHttpFile->Close();
	delete pHttpFile;

	pHttpConnection->Close();
	delete pHttpConnection;

	InternetSession.Close();

	return TRUE;
}