
#ifndef __DVNET_H
#define __DVNET_H

#include "DvDef.h"

typedef void (PASCAL *DOWNLOADFILE)(LPCTSTR lpszMsg);

class AFX_CLASS_EXPORT CDvNet
{
public:
	CDvNet();
	~CDvNet();
	
public:
	static BOOL DownLoadFile(LPCTSTR lpszURL, LPCTSTR lpszDst,
							DOWNLOADFILE pfnMsgCallBK = NULL);

private:
	static void ReturnMsg(LPCTSTR lpszMsg, DOWNLOADFILE pfnMsgCallBK);
};

inline void CDvNet::ReturnMsg(LPCTSTR lpszMsg, DOWNLOADFILE pfnMsgCallBK)
{
	if (pfnMsgCallBK && lpszMsg)
	{
		pfnMsgCallBK(lpszMsg);
	}
}

#endif