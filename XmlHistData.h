
#ifndef __XMLHISTDATA_H
#define __XMLHISTDATA_H

#include "MyLt.h"

class CXmlHistData
{
public:
	CXmlHistData();
	virtual ~CXmlHistData();
	
public:
	static BOOL RWHistData(LT_FILE_INFO *pFileDataInfo,							
							BOOL bIsRead = TRUE,
							BOOL bOnlyHeader = FALSE);
private:
	static BOOL Node2FileInfo(DVXML_NODE *pRootNode, 
							LT_FILE_INFO *pFileDataInfo, 
							BOOL bOnlyHeader);
	static BOOL FileInfo2Node(LT_FILE_INFO *pFileDataInfo, 
							DVXML_NODE *pRootNode, 
							BOOL bOnlyHeader);
};

#endif